/*************************************************************************
> File Name: GridSinglePhasePressureSolver2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <LevelSet/LevelSetUtils.h>
#include <Solver/FDM/FDMICCGSolver2.h>
#include <Solver/Grid/GridBlockedBoundaryConditionSolver2.h>
#include <Solver/Grid/GridSinglePhasePressureSolver2.h>

namespace CubbyFlow
{
	const char FLUID = 0;
	const char AIR = 1;
	const char BOUNDARY = 2;

	const double DEFAULT_TOLERANCE = 1e-6;

	GridSinglePhasePressureSolver2::GridSinglePhasePressureSolver2()
	{
		m_systemSolver = std::make_shared<FDMICCGSolver2>(100, DEFAULT_TOLERANCE);
	}

	GridSinglePhasePressureSolver2::~GridSinglePhasePressureSolver2()
	{
		// Do nothing
	}

	void GridSinglePhasePressureSolver2::Solve(
		const FaceCenteredGrid2& input,
		double timeIntervalInSeconds,
		FaceCenteredGrid2* output,
		const ScalarField2& boundarySDF,
		const VectorField2& boundaryVelocity,
		const ScalarField2& fluidSDF)
	{
		auto pos = input.CellCenterPosition();
		
		BuildMarkers(input.Resolution(), pos, boundarySDF, fluidSDF);
		BuildSystem(input);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Apply pressure gradient
			ApplyPressureGradient(input, output);
		}
	}

	GridBoundaryConditionSolver2Ptr GridSinglePhasePressureSolver2::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridBlockedBoundaryConditionSolver2>();
	}

	void GridSinglePhasePressureSolver2::SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver)
	{
		m_systemSolver = solver;
	}

	const FDMVector2& GridSinglePhasePressureSolver2::GetPressure() const
	{
		return m_system.x;
	}

	void GridSinglePhasePressureSolver2::BuildMarkers(
		const Size2& size,
		const std::function<Vector2D(size_t, size_t)>& pos,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		m_markers.Resize(size);
		m_markers.ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = pos(i, j);	
			if (IsInsideSDF(boundarySDF.Sample(pt)))
			{
				m_markers(i, j) = BOUNDARY;
			}
			else if (IsInsideSDF(fluidSDF.Sample(pt)))
			{
				m_markers(i, j) = FLUID;
			}
			else
			{
				m_markers(i, j) = AIR;
			}
		});
	}

	void GridSinglePhasePressureSolver2::BuildSystem(const FaceCenteredGrid2& input)
	{
		Size2 size = input.Resolution();
		m_system.A.Resize(size);
		m_system.x.Resize(size);
		m_system.b.Resize(size);

		Vector2D invH = 1.0 / input.GridSpacing();
		Vector2D invHSqr = invH * invH;

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j)
		{
			auto& row = m_system.A(i, j);

			// initialize
			row.center = row.right = row.up = 0.0;
			m_system.b(i, j) = 0.0;

			if (m_markers(i, j) == FLUID)
			{
				m_system.b(i, j) = input.DivergenceAtCellCenter(i, j);

				if (i + 1 < size.x && m_markers(i + 1, j) != BOUNDARY)
				{
					row.center += invHSqr.x;
					if (m_markers(i + 1, j) == FLUID)
					{
						row.right -= invHSqr.x;
					}
				}

				if (i > 0 && m_markers(i - 1, j) != BOUNDARY)
				{
					row.center += invHSqr.x;
				}

				if (j + 1 < size.y && m_markers(i, j + 1) != BOUNDARY)
				{
					row.center += invHSqr.y;
					if (m_markers(i, j + 1) == FLUID)
					{
						row.up -= invHSqr.y;
					}
				}

				if (j > 0 && m_markers(i, j - 1) != BOUNDARY)
				{
					row.center += invHSqr.y;
				}
			}
			else
			{
				row.center = 1.0;
			}
		});
	}

	void GridSinglePhasePressureSolver2::ApplyPressureGradient(const FaceCenteredGrid2& input, FaceCenteredGrid2* output)
	{
		Size2 size = input.Resolution();
		auto u = input.GetUConstAccessor();
		auto v = input.GetVConstAccessor();
		auto u0 = output->GetUAccessor();
		auto v0 = output->GetVAccessor();

		Vector2D invH = 1.0 / input.GridSpacing();

		m_system.x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (m_markers(i, j) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j) != BOUNDARY)
				{
					u0(i + 1, j) = u(i + 1, j) + invH.x * (m_system.x(i + 1, j) - m_system.x(i, j));
				}
				if (j + 1 < size.y && m_markers(i, j + 1) != BOUNDARY)
				{
					v0(i, j + 1) = v(i, j + 1) + invH.y * (m_system.x(i, j + 1) - m_system.x(i, j));
				}
			}
		});
	}
}