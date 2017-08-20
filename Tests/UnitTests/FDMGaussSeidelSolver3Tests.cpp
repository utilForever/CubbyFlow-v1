#include "pch.h"

#include <Solver/FDM/FDMGaussSeidelSolver3.h>

using namespace CubbyFlow;

TEST(FDMGaussSeidelSolver3, Constructors)
{
	FDMLinearSystem3 system;
	system.A.Resize(3, 3, 3);
	system.x.Resize(3, 3, 3);
	system.b.Resize(3, 3, 3);

	system.A.ForEachIndex([&](size_t i, size_t j, size_t k)
	{
		if (i > 0)
		{
			system.A(i, j, k).center += 1.0;
		}
		if (i < system.A.Width() - 1)
		{
			system.A(i, j, k).center += 1.0;
			system.A(i, j, k).right -= 1.0;
		}

		if (j > 0)
		{
			system.A(i, j, k).center += 1.0;
		}
		else
		{
			system.b(i, j, k) += 1.0;
		}

		if (j < system.A.Height() - 1)
		{
			system.A(i, j, k).center += 1.0;
			system.A(i, j, k).up -= 1.0;
		}
		else
		{
			system.b(i, j, k) -= 1.0;
		}

		if (k > 0)
		{
			system.A(i, j, k).center += 1.0;
		}
		if (k < system.A.Depth() - 1)
		{
			system.A(i, j, k).center += 1.0;
			system.A(i, j, k).front -= 1.0;
		}
	});

	FDMGaussSeidelSolver3 solver(100, 10, 1e-9);
	solver.Solve(&system);

	EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}