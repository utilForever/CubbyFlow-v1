/*************************************************************************
> File Name: GridFractionalSinglePhasePressureSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D fractional single-phase pressure solver.
> Created Time: 2017/08/14
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

//
// Adopted the code from:
// http://www.cs.GetUbc.ca/labs/imager/tr/2007/Batty_VariationalFluids/
// and
// https://github.com/christopherbatty/FluidRigidCoupling2D
//

#include <LevelSet/LevelSetUtils.h>
#include <Solver/FDM/FDMICCGSolver3.h>
#include <Solver/Grid/GridFractionalBoundaryConditionSolver3.h>
#include <Solver/Grid/GridFractionalSinglePhasePressureSolver3.h>

namespace CubbyFlow
{
	const double DEFAULT_TOLERANCE = 1e-6;
	const double MIN_WEIGHT = 0.01;

	GridFractionalSinglePhasePressureSolver3::GridFractionalSinglePhasePressureSolver3()
	{
		m_systemSolver = std::make_shared<FDMICCGSolver3>(100, DEFAULT_TOLERANCE);
	}

	GridFractionalSinglePhasePressureSolver3::~GridFractionalSinglePhasePressureSolver3()
	{
		// Do nothing
	}

	void GridFractionalSinglePhasePressureSolver3::Solve(
		const FaceCenteredGrid3& input,
		double timeIntervalInSeconds,
		FaceCenteredGrid3* output,
		const ScalarField3& boundarySDF,
		const VectorField3& boundaryVelocity,
		const ScalarField3& fluidSDF)
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

	GridBoundaryConditionSolver3Ptr GridFractionalSinglePhasePressureSolver3::SuggestedBoundaryConditionSolver() const
	{
		return std::make_shared<GridFractionalBoundaryConditionSolver3>();
	}

	void GridFractionalSinglePhasePressureSolver3::SetLinearSystemSolver(const FDMLinearSystemSolver3Ptr& solver)
	{
		m_systemSolver = solver;
	}

	const FDMVector3& GridFractionalSinglePhasePressureSolver3::GetPressure() const
	{
		return m_system.x;
	}

	void GridFractionalSinglePhasePressureSolver3::BuildWeights(
		const FaceCenteredGrid3& input,
		const ScalarField3& boundarySDF,
		const VectorField3& boundaryVelocity,
		const ScalarField3& fluidSDF)
	{
		Size3 uSize = input.GetUSize();
		Size3 vSize = input.GetVSize();
		Size3 wSize = input.GetWSize();
		auto uPos = input.GetUPosition();
		auto vPos = input.GetVPosition();
		auto wPos = input.GetWPosition();

		m_uWeights.Resize(uSize);
		m_vWeights.Resize(vSize);
		m_wWeights.Resize(wSize);
		m_fluidSDF.Resize(input.Resolution());

		m_boundaryVel = boundaryVelocity.Sampler();

		auto cellPos = input.CellCenterPosition();
		m_fluidSDF.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			m_fluidSDF(i, j, k) = static_cast<float>(fluidSDF.Sample(cellPos(i, j, k)));
		});

		Vector3D h = input.GridSpacing();

		m_uWeights.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			Vector3D pt = uPos(i, j, k);
			double phi0 = boundarySDF.Sample(pt + Vector3D(0.0, -0.5 * h.y, -0.5 * h.z));
			double phi1 = boundarySDF.Sample(pt + Vector3D(0.0, 0.5 * h.y, -0.5 * h.z));
			double phi2 = boundarySDF.Sample(pt + Vector3D(0.0, -0.5 * h.y, 0.5 * h.z));
			double phi3 = boundarySDF.Sample(pt + Vector3D(0.0, 0.5 * h.y, 0.5 * h.z));
			double frac = FractionInside(phi0, phi1, phi2, phi3);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to MIN_WEIGHT. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_uWeights(i, j, k) = static_cast<float>(weight);
		});

		m_vWeights.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			Vector3D pt = vPos(i, j, k);
			double phi0 = boundarySDF.Sample(pt + Vector3D(-0.5 * h.x, 0.0, -0.5 * h.z));
			double phi1 = boundarySDF.Sample(pt + Vector3D(-0.5 * h.x, 0.0, 0.5 * h.z));
			double phi2 = boundarySDF.Sample(pt + Vector3D(0.5 * h.x, 0.0, -0.5 * h.z));
			double phi3 = boundarySDF.Sample(pt + Vector3D(0.5 * h.x, 0.0, 0.5 * h.z));
			double frac = FractionInside(phi0, phi1, phi2, phi3);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to MIN_WEIGHT. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_vWeights(i, j, k) = static_cast<float>(weight);
		});

		m_wWeights.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			Vector3D pt = wPos(i, j, k);
			double phi0 = boundarySDF.Sample(pt + Vector3D(-0.5 * h.x, -0.5 * h.y, 0.0));
			double phi1 = boundarySDF.Sample(pt + Vector3D(-0.5 * h.x, 0.5 * h.y, 0.0));
			double phi2 = boundarySDF.Sample(pt + Vector3D(0.5 * h.x, -0.5 * h.y, 0.0));
			double phi3 = boundarySDF.Sample(pt + Vector3D(0.5 * h.x, 0.5 * h.y, 0.0));
			double frac = FractionInside(phi0, phi1, phi2, phi3);
			double weight = std::clamp(1.0 - frac, 0.0, 1.0);

			// Clamp non-zero weight to MIN_WEIGHT. Having nearly-zero element
			// in the matrix can be an issue.
			if (weight < MIN_WEIGHT && weight > 0.0)
			{
				weight = MIN_WEIGHT;
			}

			m_wWeights(i, j, k) = static_cast<float>(weight);
		});
	}

	void GridFractionalSinglePhasePressureSolver3::BuildSystem(const FaceCenteredGrid3& input)
	{
		const Size3 size = input.Resolution();
		const auto uPos = input.GetUPosition();
		const auto vPos = input.GetVPosition();
		const auto wPos = input.GetWPosition();

		m_system.A.Resize(size);
		m_system.x.Resize(size);
		m_system.b.Resize(size);

		const Vector3D invH = 1.0 / input.GridSpacing();
		const Vector3D invHSqr = invH * invH;

		// Build linear system
		m_system.A.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			auto& row = m_system.A(i, j, k);

			// initialize
			row.center = row.right = row.up = row.front = 0.0;
			m_system.b(i, j, k) = 0.0;

			double centerPhi = m_fluidSDF(i, j, k);

			if (IsInsideSDF(centerPhi))
			{
				double term;

				if (i + 1 < size.x)
				{
					term = m_uWeights(i + 1, j, k) * invHSqr.x;
					double rightPhi = m_fluidSDF(i + 1, j, k);
					
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

					m_system.b(i, j, k) += m_uWeights(i + 1, j, k) * input.GetU(i + 1, j, k) * invH.x;
				}
				else
				{
					m_system.b(i, j, k) += input.GetU(i + 1, j, k) * invH.x;
				}

				if (i > 0)
				{
					term = m_uWeights(i, j, k) * invHSqr.x;
					double leftPhi = m_fluidSDF(i - 1, j, k);

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

					m_system.b(i, j, k) -= m_uWeights(i, j, k) * input.GetU(i, j, k) * invH.x;
				}
				else
				{
					m_system.b(i, j, k) -= input.GetU(i, j, k) * invH.x;
				}

				if (j + 1 < size.y)
				{
					term = m_vWeights(i, j + 1, k) * invHSqr.y;
					double upPhi = m_fluidSDF(i, j + 1, k);

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

					m_system.b(i, j, k) += m_vWeights(i, j + 1, k) * input.GetV(i, j + 1, k) * invH.y;
				}
				else
				{
					m_system.b(i, j, k) += input.GetV(i, j + 1, k) * invH.y;
				}

				if (j > 0)
				{
					term = m_vWeights(i, j, k) * invHSqr.y;
					double downPhi = m_fluidSDF(i, j - 1, k);

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

					m_system.b(i, j, k) -= m_vWeights(i, j, k) * input.GetV(i, j, k) * invH.y;
				}
				else 
				{
					m_system.b(i, j, k) -= input.GetV(i, j, k) * invH.y;
				}

				if (k + 1 < size.z)
				{
					term = m_wWeights(i, j, k + 1) * invHSqr.z;
					double frontPhi = m_fluidSDF(i, j, k + 1);

					if (IsInsideSDF(frontPhi))
					{
						row.center += term;
						row.front -= term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, frontPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}
					
					m_system.b(i, j, k) += m_wWeights(i, j, k + 1) * input.GetW(i, j, k + 1) * invH.z;
				}
				else
				{
					m_system.b(i, j, k) += input.GetW(i, j, k + 1) * invH.z;
				}

				if (k > 0)
				{
					term = m_wWeights(i, j, k) * invHSqr.z;
					double backPhi = m_fluidSDF(i, j, k - 1);

					if (IsInsideSDF(backPhi))
					{
						row.center += term;
					}
					else
					{
						double theta = FractionInsideSDF(centerPhi, backPhi);
						theta = std::max(theta, 0.01);
						row.center += term / theta;
					}

					m_system.b(i, j, k) -= m_wWeights(i, j, k) * input.GetW(i, j, k) * invH.z;
				}
				else
				{
					m_system.b(i, j, k) -= input.GetW(i, j, k) * invH.z;
				}

				// Accumulate contributions from the moving boundary
				double boundaryContribution =
					(1.0 - m_uWeights(i + 1, j, k)) * m_boundaryVel(uPos(i + 1, j, k)).x * invH.x -
					(1.0 - m_uWeights(i, j, k)) * m_boundaryVel(uPos(i, j, k)).x * invH.x +
					(1.0 - m_vWeights(i, j + 1, k)) * m_boundaryVel(vPos(i, j + 1, k)).y * invH.y -
					(1.0 - m_vWeights(i, j, k)) * m_boundaryVel(vPos(i, j, k)).y * invH.y +
					(1.0 - m_wWeights(i, j, k + 1)) * m_boundaryVel(wPos(i, j, k + 1)).z * invH.z -
					(1.0 - m_wWeights(i, j, k)) * m_boundaryVel(wPos(i, j, k)).z * invH.z;
				m_system.b(i, j, k) += boundaryContribution;
			}
			else
			{
				row.center = 1.0;
			}
		});
	}

	void GridFractionalSinglePhasePressureSolver3::ApplyPressureGradient(const FaceCenteredGrid3& input, FaceCenteredGrid3* output)
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
			double centerPhi = m_fluidSDF(i, j, k);

			if (i + 1 < size.x && m_uWeights(i + 1, j, k) > 0.0 && (IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF(i + 1, j, k))))
			{
				double rightPhi = m_fluidSDF(i + 1, j, k);
				double theta = FractionInsideSDF(centerPhi, rightPhi);
				theta = std::max(theta, 0.01);

				u0(i + 1, j, k) = u(i + 1, j, k) + invH.x / theta * (m_system.x(i + 1, j, k) - m_system.x(i, j, k));
			}

			if (j + 1 < size.y && m_vWeights(i, j + 1, k) > 0.0 && (IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF(i, j + 1, k))))
			{
				double upPhi = m_fluidSDF(i, j + 1, k);
				double theta = FractionInsideSDF(centerPhi, upPhi);
				theta = std::max(theta, 0.01);

				v0(i, j + 1, k) = v(i, j + 1, k) + invH.y / theta * (m_system.x(i, j + 1, k) - m_system.x(i, j, k));
			}

			if (k + 1 < size.z && m_wWeights(i, j, k + 1) > 0.0 && (IsInsideSDF(centerPhi) || IsInsideSDF(m_fluidSDF(i, j, k + 1))))
			{
				double frontPhi = m_fluidSDF(i, j, k + 1);
				double theta = FractionInsideSDF(centerPhi, frontPhi);
				theta = std::max(theta, 0.01);

				w0(i, j, k + 1) = w(i, j, k + 1) + invH.z / theta * (m_system.x(i, j, k + 1) - m_system.x(i, j, k));
			}
		});
	}
}