/*************************************************************************
> File Name: Parallel.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Parallel functions for CubbyFlow.
> Created Time: 2017/02/05
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Utils/Parallel.h>
#include <Utils/Constants.h>

#include <algorithm>
#include <thread>
#include <vector>

namespace CubbyFlow
{
	namespace Internal
	{
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
				std::vector<std::thread> pool;
				pool.reserve(2);

				auto launchRange = [compareFunction](RandomIterator begin, size_t k2, RandomIterator2 temp, unsigned int numThreads)
				{
					ParallelMergeSort(begin, k2, temp, numThreads, compareFunction);
				};

				pool.emplace_back(launchRange, a, size / 2, temp, numThreads / 2);
				pool.emplace_back(launchRange, a + size / 2, size - size / 2, temp + size / 2, numThreads - numThreads / 2);

				// Wait for jobs to finish
				for (std::thread &t : pool)
				{
					if (t.joinable())
					{
						t.join();
					}
				}

				Merge(a, size, temp, compareFunction);
			}
		}
	}
}