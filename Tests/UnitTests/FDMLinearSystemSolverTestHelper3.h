#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER3_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER3_H

#include <Solver/FDM/FDMLinearSystemSolver3.h>

namespace CubbyFlow
{
	class FDMLinearSystemSolverTestHelper3
	{
	public:
		static void BuildTestLinearSystem(FDMLinearSystem3* system, const Size3& size)
		{
			system->A.Resize(size);
			system->x.Resize(size);
			system->b.Resize(size);

			system->A.ForEachIndex([&](size_t i, size_t j, size_t k)
			{
				if (i > 0)
				{
					system->A(i, j, k).center += 1.0;
				}
				if (i < system->A.Width() - 1)
				{
					system->A(i, j, k).center += 1.0;
					system->A(i, j, k).right -= 1.0;
				}

				if (j > 0)
				{
					system->A(i, j, k).center += 1.0;
				}
				else
				{
					system->b(i, j, k) += 1.0;
				}

				if (j < system->A.Height() - 1)
				{
					system->A(i, j, k).center += 1.0;
					system->A(i, j, k).up -= 1.0;
				}
				else
				{
					system->b(i, j, k) -= 1.0;
				}

				if (k > 0)
				{
					system->A(i, j, k).center += 1.0;
				}
				if (k < system->A.Depth() - 1)
				{
					system->A(i, j, k).center += 1.0;
					system->A(i, j, k).front -= 1.0;
				}
			});
		}
	};
}

#endif