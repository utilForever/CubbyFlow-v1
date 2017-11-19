/*************************************************************************
> File Name: AnisotropicPointsToImplicit2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D points-to-implicit converter using Anisotropic kernels.
> Created Time: 2017/11/19
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Math/SVD.h>
#include <Matrix/Matrix2x2.h>
#include <Point/Implicit/AnisotropicPointsToImplicit2.h>
#include <Solver/LevelSet/FMMLevelSetSolver2.h>
#include <SPH/SPHSystemData2.h>
#include <Utils/Logger.h>

namespace CubbyFlow
{
	inline double P(double distance)
	{
		const double distanceSquared = distance * distance;

		if (distanceSquared >= 1.0)
		{
			return 0.0;
		}

		const double x = 1.0 - distanceSquared;
		return x * x * x;
	}

	inline double Wij(double distance, double r)
	{
		if (distance < r)
		{
			return 1.0 - Cubic(distance / r);
		}
		
		return 0.0;
	}

	inline Matrix2x2D Vvt(const Vector2D& v)
	{
		return Matrix2x2D(v.x * v.x, v.x * v.y, v.y * v.x, v.y * v.y);
	}

	inline double W(const Vector2D& r, const Matrix2x2D& g, double gDet)
	{
		static const double sigma = 4.0 / PI_DOUBLE;
		return sigma * gDet * P((g * r).Length());
	}

	AnisotropicPointsToImplicit2::AnisotropicPointsToImplicit2(
		double kernelRadius,
		double cutOffDensity,
		double positionSmoothingFactor,
		size_t minNumNeighbors) :
		m_kernelRadius(kernelRadius),
		m_cutOffDensity(cutOffDensity),
		m_positionSmoothingFactor(positionSmoothingFactor),
		m_minNumNeighbors(minNumNeighbors)
	{
		// Do nothing
	}

	void AnisotropicPointsToImplicit2::Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const
	{
		if (output == nullptr)
		{
			CUBBYFLOW_WARN << "Null scalar grid output pointer provided.";
			return;
		}

		const auto res = output->Resolution();
		if (res.x * res.y == 0)
		{
			CUBBYFLOW_WARN << "Empty grid is provided.";
			return;
		}

		const auto bbox = output->BoundingBox();
		if (bbox.IsEmpty())
		{
			CUBBYFLOW_WARN << "Empty domain is provided.";
			return;
		}

		const double h = m_kernelRadius;
		const double invH = 1 / h;
		const double r = 2.0 * h;

		// Mean estimator for cov. mat.
		ParticleSystemData2 meanParticles;
		meanParticles.AddParticles(points);
		meanParticles.BuildNeighborSearcher(r);
		const auto meanNeighborSearcher = meanParticles.GetNeighborSearcher();

		// Compute G and xMean
		std::vector<Matrix2x2D> gs(points.size());
		std::vector<Vector2D> xMeans(points.size());

		ParallelFor(ZERO_SIZE, points.size(), [&](size_t i)
		{
			const auto& x = points[i];

			// Compute xMean
			Vector2D xMean;
			double wSum = 0.0;
			size_t numNeighbors = 0;
			const auto getXMean = [&](size_t, const Vector2D& xj)
			{
				const double wj = Wij((x - xj).Length(), r);
				wSum += wj;
				xMean += wj * xj;
				++numNeighbors;
			};
			meanNeighborSearcher->ForEachNearbyPoint(x, r, getXMean);

			assert(wSum > 0.0);
			xMean /= wSum;

			xMeans[i] = Lerp(x, xMean, m_positionSmoothingFactor);

			if (numNeighbors < m_minNumNeighbors)
			{
				const auto g = Matrix2x2D::MakeScaleMatrix(invH, invH);
				gs[i] = g;
			}
			else
			{
				// Compute covariance matrix
				// We start with small scale matrix (h*h) in order to
				// prevent zero covariance matrix when points are all
				// perfectly lined up.
				auto cov = Matrix2x2D::MakeScaleMatrix(h * h, h * h);
				wSum = 0.0;
				const auto getCov = [&](size_t, const Vector2D& xj)
				{
					const double wj = Wij((xMean - xj).Length(), r);
					wSum += wj;
					cov += wj * Vvt(xj - xMean);
				};
				meanNeighborSearcher->ForEachNearbyPoint(x, r, getCov);

				cov /= wSum;

				// SVD
				Matrix2x2D u;
				Vector2D v;
				Matrix2x2D w;
				SVD(cov, u, v, w);

				// Constrain Sigma
				const double maxSingularVal = v.AbsMax();
				const double kr = 4.0;
				v[0] = std::max(v[0], maxSingularVal / kr);
				v[1] = std::max(v[1], maxSingularVal / kr);
				const auto invSigma = Matrix2x2D::MakeScaleMatrix(1.0 / v);

				// Compute G
				const double relA = v[0] * v[1];
				const Matrix2x2D g = invH * std::sqrt(relA) * (w * invSigma * u.Transposed());
				gs[i] = g;
			}
		});

		// SPH estimator
		SPHSystemData2 sphParticles;
		sphParticles.AddParticles(ConstArrayAccessor1<Vector2D>(xMeans.size(), xMeans.data()));
		sphParticles.SetKernelRadius(h);
		sphParticles.BuildNeighborSearcher();
		sphParticles.UpdateDensities();
		const auto d = sphParticles.GetDensities();
		const double m = sphParticles.GetMass();

		meanParticles.Resize(0);
		meanParticles.AddParticles(ConstArrayAccessor1<Vector2D>(xMeans.size(), xMeans.data()));
		meanParticles.BuildNeighborSearcher(r);
		const auto meanNeighborSearcher2 = meanParticles.GetNeighborSearcher();

		// Compute SDF
		auto temp = output->Clone();
		temp->Fill([&](const Vector2D& x)
		{
			double sum = 0.0;
			meanNeighborSearcher2->ForEachNearbyPoint(x, r,
				[&](size_t i, const Vector2D& neighborPosition)
			{
				sum += m / d[i] * W(neighborPosition - x, gs[i], gs[i].Determinant());
			});

			return m_cutOffDensity - sum;
		});

		FMMLevelSetSolver2 solver;
		solver.Reinitialize(*temp, std::numeric_limits<double>::max(), output);
	}
}
