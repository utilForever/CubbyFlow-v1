/*************************************************************************
> File Name: Parallel-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Parallel functions for CubbyFlow.
> Created Time: 2017/02/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PARALLEL_IMPL_H
#define CUBBYFLOW_PARALLEL_IMPL_H

#include <Core/Utils/Constants.h>
#include <Core/Utils/Parallel.h>

#if defined(CUBBYFLOW_TASKING_TBB)
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>
#include <tbb/task.h>
#elif defined(CUBBYFLOW_TASKING_CPP11THREAD)
#include <thread>
#endif

#include <algorithm>
#include <cmath>
#include <future>
#include <vector>

#undef max
#undef min

namespace CubbyFlow
{
	namespace Internal
	{
		// NOTE: This abstraction takes a lambda which should take captured
		//       variables by *value* to ensure no captured references race
		//       with the task itself.
		template <typename TASK>
		inline void Schedule(TASK&& fn)
		{
#if defined(CUBBYFLOW_TASKING_TBB)
			struct LocalTBBTask : public tbb::task
			{
				TASK func;

				LocalTBBTask(TASK&& f) : func(std::forward<TASK>(f))
				{
					// Do nothing
				}

				tbb::task* execute() override
				{
					func();
					return nullptr;
				}
			};

			auto* tbbNode = new(tbb::task::allocate_root()) LocalTBBTask(std::forward<TASK>(fn));
			tbb::task::enqueue(*tbbNode);
#elif defined(CUBBYFLOW_TASKING_CPP11THREAD)
			std::thread thread(fn);
			thread.detach();
#else	// OpenMP or Serial -> Synchronous!
			fn();
#endif
		}

		template <typename TASK>
		using operator_return_t = typename std::result_of<TASK()>::type;

		// NOTE: see above, same issues associated with Schedule()
		template <typename TASK>
		inline auto Async(TASK&& fn) -> std::future<operator_return_t<TASK>>
		{
			using package_t = std::packaged_task<operator_return_t<TASK>()>;

			auto task = new package_t(std::forward<TASK>(fn));
			auto future = task->get_future();

			Schedule([=]()
			{
				(*task)();
				delete task;
			});

			return future;
		}

		// Adopted from:
		// Radenski, A.
		// Shared Memory, Message Passing, and Hybrid Merge Sorts for Standalone and
		// Clustered SMPs. Proc PDPTA'11, the  2011 International Conference on Parallel
		// and Distributed Processing Techniques and Applications, CSREA Press
		// (H. Arabnia, Ed.), 2011, pp. 367 - 373.
		template <typename RandomIterator, typename RandomIterator2, typename CompareFunction>
		void Merge(RandomIterator a, size_t size, RandomIterator2 temp, CompareFunction compareFunction)
		{
			size_t i1 = 0;
			size_t i2 = size / 2;
			size_t tempi = 0;

			while (i1 < size / 2 && i2 < size)
			{
				if (compareFunction(a[i1], a[i2]))
				{
					temp[tempi] = a[i1];
					i1++;
				}
				else
				{
					temp[tempi] = a[i2];
					i2++;
				}

				tempi++;
			}

			while (i1 < size / 2)
			{
				temp[tempi] = a[i1];
				i1++;
				tempi++;
			}

			while (i2 < size) 
			{
				temp[tempi] = a[i2];
				i2++;
				tempi++;
			}

			// Copy sorted temp array into main array, a
			ParallelFor(ZERO_SIZE, size, [&](size_t i)
			{
				a[i] = temp[i];
			});
		}

		template <typename RandomIterator, typename RandomIterator2, typename CompareFunction>
		void ParallelMergeSort(RandomIterator a, size_t size, RandomIterator2 temp, unsigned int numThreads, CompareFunction compareFunction)
		{
			if (numThreads == 1)
			{
				std::sort(a, a + size, compareFunction);
			}
			else if (numThreads > 1)
			{
				std::vector<std::future<void>> pool;
				pool.reserve(2);

				auto launchRange = [compareFunction](RandomIterator begin, size_t k2, RandomIterator2 temp, unsigned int numThreads)
				{
					ParallelMergeSort(begin, k2, temp, numThreads, compareFunction);
				};

				pool.emplace_back(Internal::Async([=]()
				{
					launchRange(a, size / 2, temp, numThreads / 2);
				}));

				pool.emplace_back(Internal::Async([=]()
				{
					launchRange(a + size / 2, size - size / 2, temp + size / 2, numThreads - numThreads / 2);
				}));

				// Wait for jobs to finish
				for (auto& f : pool)
				{
					if (f.valid())
					{
						f.wait();
					}
				}

				Merge(a, size, temp, compareFunction);
			}
		}
	}

	template <typename RandomIterator, typename T>
	void ParallelFill(
		const RandomIterator& begin, const RandomIterator& end,
		const T& value, ExecutionPolicy policy)
	{
		auto diff = end - begin;
		if (diff <= 0)
		{
			return;
		}

		size_t size = static_cast<size_t>(diff);
		ParallelFor(ZERO_SIZE, size, [begin, value](size_t i)
		{
			begin[i] = value;
		}, policy);
	}

	// Adopted from http://ideone.com/Z7zldb
	template <typename IndexType, typename Function>
	void ParallelFor(
		IndexType beginIndex, IndexType endIndex,
		const Function& function, ExecutionPolicy policy)
	{
		if (beginIndex > endIndex)
		{
			return;
		}

#if defined(CUBBYFLOW_TASKING_TBB)
		(void)policy;
		tbb::parallel_for(start, end, function);
#elif defined(CUBBYFLOW_TASKING_CPP11THREAD)
		// Estimate number of threads in the pool
		const unsigned int numThreadsHint = GetMaxNumberOfThreads();
		const unsigned int numThreads = (policy == ExecutionPolicy::Parallel) ?
			(numThreadsHint == 0u ? 8u : numThreadsHint) : 1;

		// Size of a slice for the range functions
		IndexType n = endIndex - beginIndex + 1;
		IndexType slice = static_cast<IndexType>(std::round(n / static_cast<double>(numThreads)));
		slice = std::max(slice, IndexType(1));

		// [Helper] Inner loop
		auto launchRange = [&function](IndexType k1, IndexType k2)
		{
			for (IndexType k = k1; k < k2; ++k)
			{
				function(k);
			}
		};

		// Create pool and launch jobs
		std::vector<std::thread> pool;
		pool.reserve(numThreads);
		IndexType i1 = beginIndex;
		IndexType i2 = std::min(beginIndex + slice, endIndex);

		for (unsigned int i = 0; i + 1 < numThreads && i1 < endIndex; ++i)
		{
			pool.emplace_back(launchRange, i1, i2);
			i1 = i2;
			i2 = std::min(i2 + slice, endIndex);
		}

		if (i1 < endIndex)
		{
			pool.emplace_back(launchRange, i1, endIndex);
		}

		// Wait for jobs to finish
		for (std::thread& t : pool)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
#else
		(void)policy;

#if defined(CUBBYFLOW_TASKING_OPENMP)
#pragma omp parallel for
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
		for (ssize_t i = beginIndex; i < static_cast<ssize_t>(endIndex); ++i)
		{
#else // !MSVC || Intel
		for (auto i = beginIndex; i < endIndex; ++i)
		{
#endif // MSVC && !Intel
			function(i);
		}
#else // CUBBYFLOW_TASKING_SERIAL
		for (auto i = beginIndex; i < endIndex; ++i)
		{
			function(i);
		}
#endif // CUBBYFLOW_TASKING_OPENMP
#endif
	}

	template <typename IndexType, typename Function>
	void ParallelRangeFor(
		IndexType beginIndex, IndexType endIndex,
		const Function& function, ExecutionPolicy policy)
	{
		if (beginIndex > endIndex)
		{
			return;
		}

#if defined(CUBBYFLOW_TASKING_TBB)
		if (policy == ExecutionPolicy::Parallel)
		{
			tbb::parallel_for(tbb::blocked_range<IndexType>(beginIndex, endIndex),
				[&function](const tbb::blocked_range<IndexType>& range)
			{
				function(range.begin(), range.end());
			});
		}
		else
		{
			function(beginIndex, endIndex);
		}
#else
		// Estimate number of threads in the pool
		const unsigned int numThreadsHint = GetMaxNumberOfThreads();
		const unsigned int numThreads = (policy == ExecutionPolicy::Parallel) ?
			(numThreadsHint == 0u ? 8u : numThreadsHint) : 1;

		// Size of a slice for the range functions
		IndexType n = endIndex - beginIndex + 1;
		IndexType slice = static_cast<IndexType>(std::round(n / static_cast<double>(numThreads)));
		slice = std::max(slice, IndexType(1));

		// Create pool and launch jobs
		std::vector<std::future<void>> pool;
		pool.reserve(numThreads);
		IndexType i1 = beginIndex;
		IndexType i2 = std::min(beginIndex + slice, endIndex);

		for (unsigned int i = 0; i + 1 < numThreads && i1 < endIndex; ++i)
		{
			pool.emplace_back(Internal::Async([=]() { function(i1, i2); }));
			i1 = i2;
			i2 = std::min(i2 + slice, endIndex);
		}

		if (i1 < endIndex)
		{
			pool.emplace_back(Internal::Async([=]() { function(i1, endIndex); }));
		}

		// Wait for jobs to finish
		for (auto& f : pool)
		{
			if (f.valid())
			{
				f.wait();
			}
		}
#endif
	}

	template <typename IndexType, typename Function>
	void ParallelFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		const Function& function, ExecutionPolicy policy)
	{
		ParallelFor(beginIndexY, endIndexY, [&](IndexType j)
		{
			for (IndexType i = beginIndexX; i < endIndexX; ++i)
			{
				function(i, j);
			}
		}, policy);
	}

	template <typename IndexType, typename Function>
	void ParallelRangeFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		const Function& function, ExecutionPolicy policy)
	{
		ParallelRangeFor(beginIndexY, endIndexY,
			[&](IndexType jBegin, IndexType jEnd)
		{
			function(beginIndexX, endIndexX, jBegin, jEnd);
		}, policy);
	}

	template <typename IndexType, typename Function>
	void ParallelFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		IndexType beginIndexZ, IndexType endIndexZ,
		const Function& function, ExecutionPolicy policy)
	{
		ParallelFor(beginIndexZ, endIndexZ, [&](IndexType k)
		{
			for (IndexType j = beginIndexY; j < endIndexY; ++j)
			{
				for (IndexType i = beginIndexX; i < endIndexX; ++i)
				{
					function(i, j, k);
				}
			}
		}, policy);
	}

	template <typename IndexType, typename Function>
	void ParallelRangeFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		IndexType beginIndexZ, IndexType endIndexZ,
		const Function& function, ExecutionPolicy policy)
	{
		ParallelRangeFor(beginIndexZ, endIndexZ,
			[&](IndexType kBegin, IndexType kEnd)
		{
			function(beginIndexX, endIndexX, beginIndexY, endIndexY, kBegin, kEnd);
		}, policy);
	}

	template <typename IndexType, typename Value, typename Function, typename Reduce>
	Value ParallelReduce(
		IndexType start, IndexType end,
		const Value& identity, const Function& func,
		const Reduce& reduce, ExecutionPolicy policy)
	{
		if (start > end)
		{
			return identity;	
		}
		
		// Estimate number of threads in the pool
		const unsigned int numThreadsHint = GetMaxNumberOfThreads();
		const unsigned int numThreads = (policy == ExecutionPolicy::Parallel) ?
			(numThreadsHint == 0u ? 8u : numThreadsHint) : 1;
		
		// Size of a slice for the range functions
		IndexType n = end - start + 1;
		IndexType slice = static_cast<IndexType>(std::round(n / static_cast<double>(numThreads)));
		slice = std::max(slice, IndexType(1));
		
		// Results
		std::vector<Value> results(numThreads, identity);
		
		// [Helper] Inner loop
		auto launchRange = [&](IndexType k1, IndexType k2, unsigned int tid)
		{
			results[tid] = func(k1, k2, identity);
		};
		
		// Create pool and launch jobs
		std::vector<std::future<void>> pool;
		pool.reserve(numThreads);

		IndexType i1 = start;
		IndexType i2 = std::min(start + slice, end);
		unsigned int threadID = 0;
		
		for (; threadID + 1 < numThreads && i1 < end; ++threadID)
		{
			pool.emplace_back(Internal::Async([=]() { launchRange(i1, i2, threadID); }));
			i1 = i2;
			i2 = std::min(i2 + slice, end);
		}
		
		if (i1 < end)
		{
			pool.emplace_back(Internal::Async([=]() { launchRange(i1, end, threadID); }));
		}
		
		// Wait for jobs to finish
		for (auto& f : pool)
		{
			if (f.valid())
			{
				f.wait();
			}
		}
		
		// Gather
		Value finalResult = identity;
		for (const Value& val : results)
		{
			finalResult = reduce(val, finalResult);	
		}
		
		return finalResult;
	}

	template<typename RandomIterator>
	void ParallelSort(
		RandomIterator begin, RandomIterator end,
		ExecutionPolicy policy)
	{
		ParallelSort(begin, end, std::less<typename std::iterator_traits<RandomIterator>::value_type>(), policy);
	}

	template<typename RandomIterator, typename CompareFunction>
	void ParallelSort(
		RandomIterator begin, RandomIterator end,
		CompareFunction compareFunction, ExecutionPolicy policy)
	{
		if (begin > end)
		{
			return;
		}

		size_t size = static_cast<size_t>(end - begin);

		using value_type = typename std::iterator_traits<RandomIterator>::value_type;
		std::vector<value_type> temp(size);

		// Estimate number of threads in the pool
		const unsigned int numThreadsHint = GetMaxNumberOfThreads();
		const unsigned int numThreads = (policy == ExecutionPolicy::Parallel) ?
			(numThreadsHint == 0u ? 8u : numThreadsHint) : 1;

		Internal::ParallelMergeSort(begin, size, temp.begin(), numThreads, compareFunction);
	}
}

#endif