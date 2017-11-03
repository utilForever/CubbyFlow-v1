/*************************************************************************
> File Name: FDMMGLinearSystem3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Multigrid-syle 3-D linear system.
> Created Time: 2017/11/03
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <FDM/FDMMGLinearSystem3.h>

namespace CubbyFlow
{
	void FDMMGLinearSystem3::Clear()
	{
		A.levels.clear();
		x.levels.clear();
		b.levels.clear();
	}

	size_t FDMMGLinearSystem3::GetNumberOfLevels() const
	{
		return A.levels.size();
	}

	void FDMMGLinearSystem3::ResizeWithCoarsest(const Size3 &coarsestResolution, size_t numberOfLevels)
	{
		FDMMGUtils3::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &A.levels);
		FDMMGUtils3::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &x.levels);
		FDMMGUtils3::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &b.levels);
	}

	void FDMMGLinearSystem3::ResizeWithFinest(const Size3 &finestResolution, size_t maxNumberOfLevels)
	{
		FDMMGUtils3::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &A.levels);
		FDMMGUtils3::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &x.levels);
		FDMMGUtils3::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &b.levels);
	}

	void FDMMGUtils3::Restrict(const FDMVector3 &finer, FDMVector3 *coarser)
	{
		assert(finer.size().x == 2 * coarser->size().x);
		assert(finer.size().y == 2 * coarser->size().y);
		assert(finer.size().z == 2 * coarser->size().z);

		// --*--|--*--|--*--|--*--
		//  1/8   3/8   3/8   1/8
		//           to
		// -----|-----*-----|-----
		static const std::array<double, 4> kernel = { { 0.125, 0.375, 0.375, 0.125 } };

		const Size3 n = coarser->size();
		ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y, ZERO_SIZE, n.z,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd, size_t kBegin, size_t kEnd)
		{
			std::array<size_t, 4> kIndices;

			for (size_t k = kBegin; k < kEnd; ++k)
			{
				kIndices[0] = (k > 0) ? 2 * k - 1 : 2 * k;
				kIndices[1] = 2 * k;
				kIndices[2] = 2 * k + 1;
				kIndices[3] = (k + 1 < n.z) ? 2 * k + 2 : 2 * k + 1;

				std::array<size_t, 4> jIndices;

				for (size_t j = jBegin; j < jEnd; ++j)
				{
					jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
					jIndices[1] = 2 * j;
					jIndices[2] = 2 * j + 1;
					jIndices[3] = (j + 1 < n.y) ? 2 * j + 2 : 2 * j + 1;

					std::array<size_t, 4> iIndices;
					for (size_t i = iBegin; i < iEnd; ++i)
					{
						iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
						iIndices[1] = 2 * i;
						iIndices[2] = 2 * i + 1;
						iIndices[3] = (i + 1 < n.x) ? 2 * i + 2 : 2 * i + 1;

						double sum = 0.0;
						for (size_t z = 0; z < 4; ++z)
						{
							for (size_t y = 0; y < 4; ++y)
							{
								for (size_t x = 0; x < 4; ++x)
								{
									double w = kernel[x] * kernel[y] * kernel[z];
									sum += w * finer(iIndices[x], jIndices[y], kIndices[z]);
								}
							}
						}

						(*coarser)(i, j, k) = sum;
					}
				}
			}
		});
	}

	void FDMMGUtils3::Correct(const FDMVector3 &coarser, FDMVector3 *finer)
	{
		assert(finer->size().x == 2 * coarser.size().x);
		assert(finer->size().y == 2 * coarser.size().y);
		assert(finer->size().z == 2 * coarser.size().z);

		// -----|-----*-----|-----
		//           to
		//  1/4   3/4   3/4   1/4
		// --*--|--*--|--*--|--*--
		static const std::array<double, 4> kernel = { { 0.25, 0.75, 0.75, 0.25 } };

		const Size3 n = coarser.size();
		ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y, ZERO_SIZE, n.z,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd, size_t kBegin, size_t kEnd)
		{
			std::array<size_t, 4> kIndices;

			for (size_t k = kBegin; k < kEnd; ++k)
			{
				kIndices[0] = (k > 0) ? 2 * k - 1 : 2 * k;
				kIndices[1] = 2 * k;
				kIndices[2] = 2 * k + 1;
				kIndices[3] = (k + 1 < n.z) ? 2 * k + 2 : 2 * k + 1;

				std::array<size_t, 4> jIndices;

				for (size_t j = jBegin; j < jEnd; ++j)
				{
					jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
					jIndices[1] = 2 * j;
					jIndices[2] = 2 * j + 1;
					jIndices[3] = (j + 1 < n.y) ? 2 * j + 2 : 2 * j + 1;

					std::array<size_t, 4> iIndices;
					for (size_t i = iBegin; i < iEnd; ++i)
					{
						iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
						iIndices[1] = 2 * i;
						iIndices[2] = 2 * i + 1;
						iIndices[3] = (i + 1 < n.x) ? 2 * i + 2 : 2 * i + 1;

						double cijk = coarser(i, j, k);
						for (size_t z = 0; z < 4; ++z)
						{
							for (size_t y = 0; y < 4; ++y)
							{
								for (size_t x = 0; x < 4; ++x)
								{
									double w = kernel[x] * kernel[y] * kernel[z];
									(*finer)(iIndices[x], jIndices[y], kIndices[z]) += w * cijk;
								}
							}
						}
					}
				}
			}
		});
	}
}