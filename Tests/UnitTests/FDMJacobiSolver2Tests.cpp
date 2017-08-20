#include "pch.h"

#include <Solver/FDM/FDMJacobiSolver2.h>

using namespace CubbyFlow;

TEST(FDMJacobiSolver2, Constructors)
{
	FDMLinearSystem2 system;
	system.A.Resize(3, 3);
	system.x.Resize(3, 3);
	system.b.Resize(3, 3);

	system.A.ForEachIndex([&](size_t i, size_t j)
	{
		if (i > 0)
		{
			system.A(i, j).center += 1.0;
		}
		if (i < system.A.Width() - 1)
		{
			system.A(i, j).center += 1.0;
			system.A(i, j).right -= 1.0;
		}

		if (j > 0)
		{
			system.A(i, j).center += 1.0;
		}
		else
		{
			system.b(i, j) += 1.0;
		}

		if (j < system.A.Height() - 1)
		{
			system.A(i, j).center += 1.0;
			system.A(i, j).up -= 1.0;
		}
		else
		{
			system.b(i, j) -= 1.0;
		}
	});

	FDMJacobiSolver2 solver(100, 10, 1e-9);
	solver.Solve(&system);

	EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}