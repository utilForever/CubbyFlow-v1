#include "pch.h"
#include "FDMLinearSystemSolverTestHelper2.h"

#include <Solver/FDM/FDMGaussSeidelSolver2.h>

using namespace CubbyFlow;

TEST(FDMGaussSeidelSolver2, SolveLowRes)
{
	FDMLinearSystem2 system;
	FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 3, 3 });

	FDMGaussSeidelSolver2 solver(100, 10, 1e-9);
	solver.Solve(&system);

	EXPECT_GT(solver.GetTolerance(), solver.GetLastResidual());
}

TEST(FDMGaussSeidelSolver2, Solve)
{
	FDMLinearSystem2 system;
	FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 128, 128 });

	auto buffer = system.x;
	FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
	double norm0 = FDMBlas2::L2Norm(buffer);

	FDMGaussSeidelSolver2 solver(100, 10, 1e-9);
	solver.Solve(&system);

	FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
	double norm1 = FDMBlas2::L2Norm(buffer);

	EXPECT_LT(norm1, norm0);
}

TEST(FDMGaussSeidelSolver2, Relax)
{
	FDMLinearSystem2 system;
	FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 128, 128 });

	auto buffer = system.x;
	FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
	double norm0 = FDMBlas2::L2Norm(buffer);

	for (int i = 0; i < 200; ++i)
	{
		FDMGaussSeidelSolver2::Relax(system.A, system.b, 1.0, &system.x);

		FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
		double norm = FDMBlas2::L2Norm(buffer);
		EXPECT_LT(norm, norm0);

		norm0 = norm;
	}
}

TEST(FDMGaussSeidelSolver2, RelaxRedBlack)
{
	FDMLinearSystem2 system;
	FDMLinearSystemSolverTestHelper2::BuildTestLinearSystem(&system, { 128, 128 });

	auto buffer = system.x;
	FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
	double norm0 = FDMBlas2::L2Norm(buffer);

	for (int i = 0; i < 200; ++i)
	{
		FDMGaussSeidelSolver2::RelaxRedBlack(system.A, system.b, 1.0, &system.x);

		FDMBlas2::Residual(system.A, system.x, system.b, &buffer);
		double norm = FDMBlas2::L2Norm(buffer);
		EXPECT_LT(norm, norm0);

		norm0 = norm;
	}
}