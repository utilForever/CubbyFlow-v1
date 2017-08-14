/*************************************************************************
> File Name: GridFractionalSinglePhasePressureSolver2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D fractional single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

//
// Adopted the code from:
// http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/
// and
// https://github.com/christopherbatty/FluidRigidCoupling2D
//

#include <LevelSet/LevelSetUtils.h>
#include <Solver/FDM/FDMICCGSolver2.h>
#include <Solver/Grid/GridFractionalBoundaryConditionSolver2.h>
#include <Solver/Grid/GridFractionalSinglePhasePressureSolver2.h>

namespace CubbyFlow
{
	const double DEFAULT_TOLERANCE = 1e-6;
	const double MIN_WEIGHT = 0.01;

	GridFractionalSinglePhasePressureSolver2::GridFractionalSinglePhasePressureSolver2()
	{
		m_systemSolver = std::make_shared<FDMICCGSolver2>(100, DEFAULT_TOLERANCE);
	}

	GridFractionalSinglePhasePressureSolver2::~GridFractionalSinglePhasePressureSolver2()
	{
		// Do nothing
	}

	void GridFractionalSinglePhasePressureSolver2::Solve(
		const FaceCenteredGrid2& input,
		double timeIntervalInSeconds,
		FaceCenteredGrid2* output,
		const ScalarField2& boundarySDF,
		const VectorField2& boundaryVelocity,
		const ScalarField2& fluidSDF)
	{
		BuildWeights(input, boundarySDF, boundaryVelocity, fluidSDF);
		BuildSystem(input);

		if (m_systemSolver != nullptr)
		{
			// Solve the system
			m_systemSolver->Solve(&m_system);

			// Apply pressure gradient
			ApplyPressureGradient(input, output);
		}
	}

	GridBoundaryConditionSolver2Ptr GridFractionalSinglePhasePressureSolver2::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridFractionalBoundaryConditionSolver2>();
	}

	void GridFractionalSinglePhasePressureSolver2::SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver)
	{
		m_systemSolver = solver;
	}

	const FDMVector2& GridFractionalSinglePhasePressureSolver2::GetPressure() const
	{
		return m_system.x;
	}

	void GridFractionalSinglePhasePressureSolver2::BuildWeights(
		const FaceCenteredGrid2& input,
		const ScalarField2& boundarySDF,
		const VectorField2& boundaryVelocity,
		const ScalarField2& fluidSDF)
	{
		Size2 uSize = input.GetUSize();
		Size2 vSize = input.GetVSize();
		auto uPos = input.GetUPosition();
		auto vPos = input.GetVPosition();

		m_uWeights.Resize(uSize);
		m_vWeights.Resize(vSize);
		m_fluidSDF.Resize(input.Resolution());
		m_boundaryVel = boundaryVelocity.Sampler();

		auto cellPos = input.CellCenterPosition();
		m_fluidSDF.ParallelForEachIndex([&](size_t i, size_t j)
		{
			m_fluidSDF(i, j) = static_cast<float>(fluidSDF.Sample(cellPos(i, j)));
		});

		Vector2D h = input.GridSpacing();

		m_uWeights.ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = uPos(i, j);
			double phi0 = boundarySDF.Sample(pt - Vector2D(0.5 * h.x, 0.0));
			double phi1 = boundarySDF.Sample(pt + Vector2D(0.5 * h.x, 0.0));
			double frac = FractionInsideSDF(phi0, phi1);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to MIN_WEIGHT. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_uWeights(i, j) = static_cast<float>(weight);
		});

		m_vWeights.ParallelForEachIndex([&](size_t i, size_t j)
		{
			Vector2D pt = vPos(i, j);
			double phi0 = boundarySDF.Sample(pt - Vector2D(0.0, 0.5 * h.y));
			double phi1 = boundarySDF.Sample(pt + Vector2D(0.0, 0.5 * h.y));
			double frac = FractionInsideSDF(phi0, phi1);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to MIN_WEIGHT. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_vWeights(i, j) = static_cast<float>(weight);
		});
	}

	void GridFractionalSinglePhasePressureSolver2::BuildSystem(const FaceCenteredGrid2& input)
	{
		const Size2 size = input.Resolution();
		const auto uPos = input.GetUPosition();
		const auto vPos = input.GetVPosition();
		
		m_system.A.Resize(size);
		m_system.x.Resize(size);
		m_system.b.Resize(size);

		const Vector2D invH = 1.0 / input.GridSpacing();
		const Vector2D invHSqr = invH * invH;

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j)
		{
			auto& row = m_system.A(i, j);

			// initialize
			row.center = row.right = row.up = 0.0;
			m_system.b(i, j) = 0.0;

			double centerPhi = m_fluidSDF(i, j);

			if (IsInsideSDF(centerPhi))
			{
				double term;

				if (i + 1 < size.x)
				{
					term = m_uWeights(i + 1, j) * invHSqr.x;
					double rightPhi = m_fluidSDF(i + 1, j);

					if (IsInsideSDF(rightPhi))
					{
						row.center += term;
						row.right -= term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, rightPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}

					m_system.b(i, j) += m_uWeights(i + 1, j) * input.GetU(i + 1, j) * invH.x;
				}
				else
				{
					m_system.b(i, j) += input.GetU(i + 1, j) * invH.x;
				}
				
				if (i > 0)
				{
					term = m_uWeights(i, j) * invHSqr.x;
					double leftPhi = m_fluidSDF(i - 1, j);

					if (IsInsideSDF(leftPhi))
					{
						row.center += term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, leftPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}

					m_system.b(i, j) -= m_uWeights(i, j) * input.GetU(i, j) * invH.x;
				}
				else
				{
					m_system.b(i, j) -= input.GetU(i, j) * invH.x;
				}

				if (j + 1 < size.y)
				{
					term = m_vWeights(i, j + 1) * invHSqr.y;
					double upPhi = m_fluidSDF(i, j + 1);

					if (IsInsideSDF(upPhi))
					{
						row.center += term;
						row.up -= term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, upPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}

					m_system.b(i, j) += m_vWeights(i, j + 1) * input.GetV(i, j + 1) * invH.y;
				}
				else
				{
					m_system.b(i, j) += input.GetV(i, j + 1) * invH.y;
				}

				if (j > 0)
				{
					term = m_vWeights(i, j) * invHSqr.y;
					double downPhi = m_fluidSDF(i, j - 1);

					if (IsInsideSDF(downPhi))
					{
						row.center += term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, downPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}

					m_system.b(i, j) -= m_vWeights(i, j) * input.GetV(i, j) * invH.y;
				}
				else
				{
					m_system.b(i, j) -= input.GetV(i, j) * invH.y;
				}

				// Accumulate contributions from the moving boundary
				double boundaryContribution =
					(1.0 - m_uWeights(i + 1, j)) * m_boundaryVel(uPos(i + 1, j)).x * invH.x -
					(1.0 - m_uWeights(i, j)) * m_boundaryVel(uPos(i, j)).x * invH.x +
					(1.0 - m_vWeights(i, j + 1)) * m_boundaryVel(vPos(i, j + 1)).y * invH.y -
					(1.0 - m_vWeights(i, j)) * m_boundaryVel(vPos(i, j)).y * invH.y;
				m_system.b(i, j) += boundaryContribution;
			}
			else
			{
				row.center = 1.0;
			}
		});
	}

	void GridFractionalSinglePhasePressureSolver2::ApplyPressureGradient(const FaceCenteredGrid2& input, FaceCenteredGrid2* output)
	{
		Size2 size = input.Resolution();
		auto u = input.GetUConstAccessor();
		auto v = input.GetVConstAccessor();
		auto u0 = output->GetUAccessor();
		auto v0 = output->GetVAccessor();

		Vector2D invH = 1.0 / input.GridSpacing();

		m_system.x.ParallelForEachIndex([&](size_t i, size_t j)
		{
			double centerPhi = m_fluidSDF(i, j);

			if (i + 1 < size.x && m_uWeights(i + 1, j) > 0.0 && (IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF(i + 1, j))))
			{
				double rightPhi = m_fluidSDF(i + 1, j);
				double theta = FractionInsideSDF(centerPhi, rightPhi);
				theta = std::max(theta, 0.01);

				u0(i + 1, j) = u(i + 1, j) + invH.x / theta * (m_system.x(i + 1, j) - m_system.x(i, j));
			}

			if (j + 1 < size.y && m_vWeights(i, j + 1) > 0.0 && (IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF(i, j + 1))))
			{
				double upPhi = m_fluidSDF(i, j + 1);
				double theta = FractionInsideSDF(centerPhi, upPhi);
				theta = std::max(theta, 0.01);

				v0(i, j + 1) = v(i, j + 1) + invH.y / theta * (m_system.x(i, j + 1) - m_system.x(i, j));
			}
		});
	}
}