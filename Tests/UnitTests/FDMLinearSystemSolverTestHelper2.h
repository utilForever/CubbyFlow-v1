#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER2_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER2_H

#include <Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	class FdmLinearSystemSolverTestHelper2
	{
	public:
		static void BuildTestLinearSystem(FDMLinearSystem2* system, const Size2& size)
		{
			system->A.Resize(size);
			system->x.Resize(size);
			system->b.Resize(size);

			system->A.ForEachIndex([&](size_t i, size_t j)
			{
				if (i > 0)
				{
					system->A(i, j).center += 1.0;
				}
				if (i < system->A.Width() - 1)
				{
					system->A(i, j).center += 1.0;
					system->A(i, j).right -= 1.0;
				}

				if (j > 0)
				{
					system->A(i, j).center += 1.0;
				}
				else
				{
					system->b(i, j) += 1.0;
				}

				if (j < system->A.Height() - 1)
				{
					system->A(i, j).center += 1.0;
					system->A(i, j).up -= 1.0;
				}
				else
				{
					system->b(i, j) -= 1.0;
				}
			});
		}
	};
}

#endif