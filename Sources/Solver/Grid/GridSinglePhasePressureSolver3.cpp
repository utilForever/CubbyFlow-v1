/*************************************************************************
> File Name: GridSinglePhasePressureSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <LevelSet/LevelSetUtils.h>
#include <Solver/FDM/FDMICCGSolver3.h>
#include <Solver/Grid/GridBlockedBoundaryConditionSolver3.h>
#include <Solver/Grid/GridSinglePhasePressureSolver3.h>

namespace CubbyFlow
{
	const char FLUID = 0;
	const char AIR = 1;
	const char BOUNDARY = 2;

	const double DEFAULT_TOLERANCE = 1e-6;

	GridSinglePhasePressureSolver3::GridSinglePhasePressureSolver3()
	{
		m_systemSolver = std::make_shared<FDMICCGSolver3>(100, DEFAULT_TOLERANCE);
	}

	GridSinglePhasePressureSolver3::~GridSinglePhasePressureSolver3()
	{
		// Do nothing
	}

	void GridSinglePhasePressureSolver3::Solve(
		const FaceCenteredGrid3& input,
		double timeIntervalInSeconds,
		FaceCenteredGrid3* output,
		const ScalarField3& boundarySDF,
		const VectorField3& boundaryVelocity,
		const ScalarField3& fluidSDF)
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

	GridBoundaryConditionSolver3Ptr GridSinglePhasePressureSolver3::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridBlockedBoundaryConditionSolver3>();
	}

	void GridSinglePhasePressureSolver3::SetLinearSystemSolver(const FDMLinearSystemSolver3Ptr& solver)
	{
		m_systemSolver = solver;
	}

	const FDMVector3& GridSinglePhasePressureSolver3::GetPressure() const
	{
		return m_system.x;
	}

	void GridSinglePhasePressureSolver3::BuildMarkers(
		const Size3& size,
		const std::function<Vector3D(size_t, size_t, size_t)>& pos,
		const ScalarField3& boundarySDF,
		const ScalarField3& fluidSDF)
	{
		m_markers.Resize(size);
		m_markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			Vector3D pt = pos(i, j, k);
			if (IsInsideSDF(boundarySDF.Sample(pt)))
			{
				m_markers(i, j, k) = BOUNDARY;
			}
			else if (IsInsideSDF(fluidSDF.Sample(pt)))
			{
				m_markers(i, j, k) = FLUID;
			}
			else
			{
				m_markers(i, j, k) = AIR;
			}
		});
	}

	void GridSinglePhasePressureSolver3::BuildSystem(const FaceCenteredGrid3& input)
	{
		Size3 size = input.Resolution();
		m_system.A.Resize(size);
		m_system.x.Resize(size);
		m_system.b.Resize(size);

		Vector3D invH = 1.0 / input.GridSpacing();
		Vector3D invHSqr = invH * invH;

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			auto& row = m_system.A(i, j, k);

			// initialize
			row.center = row.right = row.up = row.front = 0.0;
			m_system.b(i, j, k) = 0.0;

			if (m_markers(i, j, k) == FLUID)
			{
				m_system.b(i, j, k) = input.DivergenceAtCellCenter(i, j, k);

				if (i + 1 < size.x && m_markers(i + 1, j, k) != BOUNDARY)
				{
					row.center += invHSqr.x;
					if (m_markers(i + 1, j, k) == FLUID)
					{
						row.right -= invHSqr.x;
					}
				}

				if (i > 0 && m_markers(i - 1, j, k) != BOUNDARY)
				{
					row.center += invHSqr.x;
				}

				if (j + 1 < size.y && m_markers(i, j + 1, k) != BOUNDARY)
				{
					row.center += invHSqr.y;
					if (m_markers(i, j + 1, k) == FLUID)
					{
						row.up -= invHSqr.y;
					}
				}

				if (j > 0 && m_markers(i, j - 1, k) != BOUNDARY)
				{
					row.center += invHSqr.y;
				}

				if (k + 1 < size.z && m_markers(i, j, k + 1) != BOUNDARY)
				{
					row.center += invHSqr.z;
					if (m_markers(i, j, k + 1) == FLUID)
					{
						row.front -= invHSqr.z;
					}
				}

				if (k > 0 && m_markers(i, j, k - 1) != BOUNDARY)
				{
					row.center += invHSqr.z;
				}
			}
			else
			{
				row.center = 1.0;
			}
		});
	}

	void GridSinglePhasePressureSolver3::ApplyPressureGradient(const FaceCenteredGrid3& input, FaceCenteredGrid3* output)
	{
		Size3 size = input.Resolution();
		auto u = input.GetUConstAccessor();
		auto v = input.GetVConstAccessor();
		auto w = input.GetWConstAccessor();
		auto u0 = output->GetUAccessor();
		auto v0 = output->GetVAccessor();
		auto w0 = output->GetWAccessor();

		Vector3D invH = 1.0 / input.GridSpacing();

		m_system.x.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (m_markers(i, j, k) == FLUID)
			{
				if (i + 1 < size.x && m_markers(i + 1, j, k) != BOUNDARY)
				{
					u0(i + 1, j, k) = u(i + 1, j, k) + invH.x * (m_system.x(i + 1, j, k) - m_system.x(i, j, k));
				}
				if (j + 1 < size.y && m_markers(i, j + 1, k) != BOUNDARY)
				{
					v0(i, j + 1, k) = v(i, j + 1, k) + invH.y * (m_system.x(i, j + 1, k) - m_system.x(i, j, k));
				}
				if (k + 1 < size.z && m_markers(i, j, k + 1) != BOUNDARY)
				{
					w0(i, j, k + 1) = w(i, j, k + 1) + invH.z * (m_system.x(i, j, k + 1) - m_system.x(i, j, k));
				}
			}
		});
	}
}