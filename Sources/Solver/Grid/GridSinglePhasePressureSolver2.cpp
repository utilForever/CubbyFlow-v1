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

	namespace
	{
		void BuildSingleSystem(FDMMatrix2* A, FDMVector2* b,
			const Array2<char>& markers,
			const FaceCenteredGrid2& input)
		{
			Size2 size = input.Resolution();
			const Vector2D invH = 1.0 / input.GridSpacing();
			Vector2D invHSqr = invH * invH;

			A->ParallelForEachIndex([&](size_t i, size_t j)
			{
				auto& row = (*A)(i, j);

				// initialize
				row.center = row.right = row.up = 0.0;
				(*b)(i, j) = 0.0;

				if (markers(i, j) == FLUID)
				{
					(*b)(i, j) = input.DivergenceAtCellCenter(i, j);

					if (i + 1 < size.x && markers(i + 1, j) != BOUNDARY)
					{
						row.center += invHSqr.x;
						if (markers(i + 1, j) == FLUID)
						{
							row.right -= invHSqr.x;
						}
					}

					if (i > 0 && markers(i - 1, j) != BOUNDARY)
					{
						row.center += invHSqr.x;
					}

					if (j + 1 < size.y && markers(i, j + 1) != BOUNDARY)
					{
						row.center += invHSqr.y;
						if (markers(i, j + 1) == FLUID)
						{
							row.up -= invHSqr.y;
						}
					}

					if (j > 0 && markers(i, j - 1) != BOUNDARY)
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
	}

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
		const auto pos = input.CellCenterPosition();
		
		BuildMarkers(input.Resolution(), pos, boundarySDF, fluidSDF);
		BuildSystem(input);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			if (m_mgSystemSolver == nullptr)
			{
				m_systemSolver->Solve(&m_system);
			}
			else
			{
				m_mgSystemSolver->Solve(&m_mgSystem);
			}
			
			// Apply pressure gradient
			ApplyPressureGradient(input, output);
		}
	}

	GridBoundaryConditionSolver2Ptr GridSinglePhasePressureSolver2::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridBlockedBoundaryConditionSolver2>();
	}

	const FDMLinearSystemSolver2Ptr& GridSinglePhasePressureSolver2::GetLinearSystemSolver() const
	{
		return m_systemSolver;
	}

	void GridSinglePhasePressureSolver2::SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver)
	{
		m_systemSolver = solver;
		m_mgSystemSolver = std::dynamic_pointer_cast<FDMMGSolver2>(m_systemSolver);

		if (m_mgSystemSolver == nullptr)
		{
			// In case of non-mg system, use flat structure.
			m_mgSystem.Clear();
		}
		else {
			// In case of mg system, use multi-level structure.
			m_system.Clear();
		}
	}

	const FDMVector2& GridSinglePhasePressureSolver2::GetPressure() const
	{
		if (m_mgSystemSolver == nullptr)
		{
			return m_system.x;
		}
		
		return m_mgSystem.x.levels.front();
	}

	void GridSinglePhasePressureSolver2::BuildMarkers(
		const Size2& size,
		const std::function<Vector2D(size_t, size_t)>& pos,
		const ScalarField2& boundarySDF,
		const ScalarField2& fluidSDF)
	{
		// Build levels
		size_t maxLevels = 1;
		if (m_mgSystemSolver != nullptr)
		{
			maxLevels = m_mgSystemSolver->GetParams().maxNumberOfLevels;
		}
		FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_markers);

		// Build top-level markers
		m_markers[0].ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = pos(i, j);
			if (IsInsideSDF(boundarySDF.Sample(pt)))
			{
				m_markers[0](i, j) = BOUNDARY;
			}
			else if (IsInsideSDF(fluidSDF.Sample(pt)))
			{
				m_markers[0](i, j) = FLUID;
			}
			else
			{
				m_markers[0](i, j) = AIR;
			}
		});

		// Build sub-level markers
		for (size_t l = 1; l < m_markers.size(); ++l)
		{
			const auto& finer = m_markers[l - 1];
			auto& coarser = m_markers[l];
			const Size2 n = coarser.size();

			ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y,
				[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd)
			{
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

						int cnt[3] = { 0, 0, 0 };
						for (size_t y = 0; y < 4; ++y)
						{
							for (size_t x = 0; x < 4; ++x)
							{
								char f = finer(iIndices[x], jIndices[y]);
								if (f == BOUNDARY)
								{
									++cnt[static_cast<int>(BOUNDARY)];
								}
								else if (f == FLUID)
								{
									++cnt[static_cast<int>(FLUID)];
								}
								else
								{
									++cnt[static_cast<int>(AIR)];
								}
							}
						}

						coarser(i, j) = static_cast<char>(ArgMax3(cnt[0], cnt[1], cnt[2]));
					}
				}
			});
		}
	}

	void GridSinglePhasePressureSolver2::BuildSystem(const FaceCenteredGrid2& input)
	{
		const Size2 size = input.Resolution();
		size_t numLevels = 1;
		FDMMatrix2* A;
		FDMVector2* b;

		if (m_mgSystemSolver == nullptr)
		{
			m_system.A.Resize(size);
			m_system.x.Resize(size);
			m_system.b.Resize(size);
			A = &m_system.A;
			b = &m_system.b;
		}
		else
		{
			// Build levels
			const size_t maxLevels = m_mgSystemSolver->GetParams().maxNumberOfLevels;
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.A.levels);
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.x.levels);
			FDMMGUtils2::ResizeArrayWithFinest(size, maxLevels, &m_mgSystem.b.levels);
			
			A = &m_mgSystem.A.levels.front();
			b = &m_mgSystem.b.levels.front();
			numLevels = m_mgSystem.A.levels.size();
		}

		// Build top level
		const FaceCenteredGrid2* finer = &input;
		BuildSingleSystem(A, b, m_markers[0], *finer);

		// Build sub-levels
		FaceCenteredGrid2 coarser;
		for (size_t l = 1; l < numLevels; ++l)
		{
			auto res = finer->Resolution();
			auto h = finer->GridSpacing();
			const auto o = finer->Origin();
			res.x = res.x >> 1;
			res.y = res.y >> 1;
			h *= 2.0;

			// Down sample
			coarser.Resize(res, h, o);
			coarser.Fill(finer->Sampler());

			A = &m_mgSystem.A.levels[l];
			b = &m_mgSystem.b.levels[l];
			BuildSingleSystem(A, b, m_markers[l], coarser);

			finer = &coarser;
		}
	}

	void GridSinglePhasePressureSolver2::ApplyPressureGradient(const FaceCenteredGrid2& input, FaceCenteredGrid2* output)
	{
		Size2 size = input.Resolution();
		auto u = input.GetUConstAccessor();
		auto v = input.GetVConstAccessor();
		auto u0 = output->GetUAccessor();
		auto v0 = output->GetVAccessor();

		const auto& x = GetPressure();

		Vector2D invH = 1.0 / input.GridSpacing();

		x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (m_markers[0](i, j) == FLUID)
			{
				if (i + 1 < size.x && m_markers[0](i + 1, j) != BOUNDARY)
				{
					u0(i + 1, j) = u(i + 1, j) + invH.x * (x(i + 1, j) - x(i, j));
				}
				if (j + 1 < size.y && m_markers[0](i, j + 1) != BOUNDARY)
				{
					v0(i, j + 1) = v(i, j + 1) + invH.y * (x(i, j + 1) - x(i, j));
				}
			}
		});
	}
}