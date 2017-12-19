/*************************************************************************
> File Name: FDMICCGSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using incomplete
>          Cholesky conjugate gradient (ICCG).
> Created Time: 2017/08/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Math/CG.h>
#include <Solver/FDM/FDMICCGSolver3.h>
#include <Utils/Logger.h>

namespace CubbyFlow
{
	void FDMICCGSolver3::Preconditioner::Build(const FDMMatrix3& matrix)
	{
		Size3 size = matrix.size();
		A = matrix.ConstAccessor();

		d.Resize(size, 0.0);
		y.Resize(size, 0.0);

		matrix.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			double denom =
				matrix(i, j, k).center -
				((i > 0) ? Square(matrix(i - 1, j, k).right) * d(i - 1, j, k) : 0.0) -
				((j > 0) ? Square(matrix(i, j - 1, k).up)    * d(i, j - 1, k) : 0.0) -
				((k > 0) ? Square(matrix(i, j, k - 1).front) * d(i, j, k - 1) : 0.0);

			if (std::fabs(denom) > 0.0)
			{
				d(i, j, k) = 1.0 / denom;
			}
			else
			{
				d(i, j, k) = 0.0;
			}
		});
	}

	void FDMICCGSolver3::Preconditioner::Solve(const FDMVector3& b, FDMVector3* x)
	{
		Size3 size = b.size();
		ssize_t sx = static_cast<ssize_t>(size.x);
		ssize_t sy = static_cast<ssize_t>(size.y);
		ssize_t sz = static_cast<ssize_t>(size.z);

		b.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			y(i, j, k) =
				(b(i, j, k) -
				((i > 0) ? A(i - 1, j, k).right * y(i - 1, j, k) : 0.0) -
				((j > 0) ? A(i, j - 1, k).up    * y(i, j - 1, k) : 0.0) -
				((k > 0) ? A(i, j, k - 1).front * y(i, j, k - 1) : 0.0)) *
				d(i, j, k);
		});

		for (ssize_t k = sz - 1; k >= 0; --k)
		{
			for (ssize_t j = sy - 1; j >= 0; --j)
			{
				for (ssize_t i = sx - 1; i >= 0; --i)
				{
					(*x)(i, j, k) =
						(y(i, j, k) -
						((i + 1 < sx) ? A(i, j, k).right * (*x)(i + 1, j, k) : 0.0) -
						((j + 1 < sy) ? A(i, j, k).up    * (*x)(i, j + 1, k) : 0.0) -
						((k + 1 < sz) ? A(i, j, k).front * (*x)(i, j, k + 1) : 0.0)) *
						d(i, j, k);
				}
			}
		}
	}

	FDMICCGSolver3::FDMICCGSolver3(unsigned int maxNumberOfIterations, double tolerance) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_tolerance(tolerance),
		m_lastResidualNorm(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMICCGSolver3::Solve(FDMLinearSystem3* system)
	{
		FDMMatrix3& matrix = system->A;
		FDMVector3& solution = system->x;
		FDMVector3& rhs = system->b;

		assert(matrix.size() == rhs.size());
		assert(matrix.size() == solution.size());

		Size3 size = matrix.size();
		m_r.Resize(size);
		m_d.Resize(size);
		m_q.Resize(size);
		m_s.Resize(size);

		system->x.Set(0.0);
		m_r.Set(0.0);
		m_d.Set(0.0);
		m_q.Set(0.0);
		m_s.Set(0.0);

		m_precond.Build(matrix);

		PCG<FDMBLAS3, Preconditioner>(
			matrix,
			rhs,
			m_maxNumberOfIterations,
			m_tolerance,
			&m_precond,
			&solution,
			&m_r,
			&m_d,
			&m_q,
			&m_s,
			&m_lastNumberOfIterations,
			&m_lastResidualNorm);

		CUBBYFLOW_INFO << "Residual norm after solving ICCG: " << m_lastResidualNorm
			<< " Number of ICCG iterations: " << m_lastNumberOfIterations;

		return (m_lastResidualNorm <= m_tolerance) || (m_lastNumberOfIterations < m_maxNumberOfIterations);
	}

	unsigned int FDMICCGSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMICCGSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMICCGSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMICCGSolver3::GetLastResidual() const
	{
		return m_lastResidualNorm;
	}
}