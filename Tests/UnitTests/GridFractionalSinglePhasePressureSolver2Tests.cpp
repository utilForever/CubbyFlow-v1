#include "pch.h"

#include <Grid/CellCenteredScalarGrid2.h>
#include <Solver/Grid/GridFractionalSinglePhasePressureSolver2.h>

using namespace CubbyFlow;

TEST(GridFractionalSinglePhasePressureSolver2, SolveFreeSurface)
{
	FaceCenteredGrid2 vel(3, 3);
	CellCenteredScalarGrid2 fluidSDF(3, 3);

	for (size_t j = 0; j < 3; ++j)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			vel.GetU(i, j) = 0.0;
		}
	}

	for (size_t j = 0; j < 4; ++j)
	{
		for (size_t i = 0; i < 3; ++i)
		{
			if (j == 0 || j == 3)
			{
				vel.GetV(i, j) = 0.0;
			}
			else
			{
				vel.GetV(i, j) = 1.0;
			}
		}
	}

	fluidSDF.Fill([&](const Vector2D& x)
	{
		return x.y - 2.0;
	});

	GridFractionalSinglePhasePressureSolver2 solver;
	solver.Solve(
		vel,
		1.0,
		&vel,
		ConstantScalarField2(std::numeric_limits<double>::max()),
		ConstantVectorField2({ 0, 0 }),
		fluidSDF);

	for (size_t j = 0; j < 3; ++j)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			EXPECT_NEAR(0.0, vel.GetU(i, j), 1e-6);
		}
	}

	for (size_t j = 0; j < 4; ++j)
	{
		for (size_t i = 0; i < 3; ++i)
		{
			EXPECT_NEAR(0.0, vel.GetV(i, j), 1e-6);
		}
	}

	const auto& pressure = solver.GetPressure();
	for (size_t i = 0; i < 3; ++i)
	{
		EXPECT_NEAR(1.5, pressure(i, 0), 1e-6);
		EXPECT_NEAR(0.5, pressure(i, 1), 1e-6);
		EXPECT_NEAR(0.0, pressure(i, 2), 1e-6);
	}
}