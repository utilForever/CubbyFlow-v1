/*************************************************************************
> File Name: FDMJacobiSolver3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using Jacobi method.
> Created Time: 2017/08/17
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Solver/FDM/FDMJacobiSolver3.h>

namespace CubbyFlow
{
	FDMJacobiSolver3::FDMJacobiSolver3(
		unsigned int maxNumberOfIterations,
		unsigned int residualCheckInterval,
		double tolerance) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_residualCheckInterval(residualCheckInterval),
		m_tolerance(tolerance),
		m_lastResidual(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMJacobiSolver3::Solve(FDMLinearSystem3* system)
	{
		m_xTemp.Resize(system->x.size());
		m_residual.Resize(system->x.size());

		m_lastNumberOfIterations = m_maxNumberOfIterations;

		for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
		{
			Relax(system->A, system->b, &system->x, &m_xTemp);
			m_xTemp.Swap(system->x);

			if (iter != 0 && iter % m_residualCheckInterval == 0)
			{
				FDMBlas3::Residual(system->A, system->x, system->b, &m_residual);

				if (FDMBlas3::L2Norm(m_residual) < m_tolerance)
				{
					m_lastNumberOfIterations = iter + 1;
					break;
				}
			}
		}

		FDMBlas3::Residual(system->A, system->x, system->b, &m_residual);
		m_lastResidual = FDMBlas3::L2Norm(m_residual);

		return m_lastResidual < m_tolerance;
	}

	unsigned int FDMJacobiSolver3::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMJacobiSolver3::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMJacobiSolver3::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMJacobiSolver3::GetLastResidual() const
	{
		return m_lastResidual;
	}

	void FDMJacobiSolver3::Relax(const FDMMatrix3& A, const FDMVector3& b,
		FDMVector3* x, FDMVector3* xTemp)
	{
		Size3 size = A.size();
		FDMVector3& refX = *x;
		FDMVector3& refXTemp = *xTemp;

		A.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			double r =
				((i > 0) ? A(i - 1, j, k).right * refX(i - 1, j, k) : 0.0) +
				((i + 1 < size.x) ? A(i, j, k).right * refX(i + 1, j, k) : 0.0) +
				((j > 0) ? A(i, j - 1, k).up * refX(i, j - 1, k) : 0.0) +
				((j + 1 < size.y) ? A(i, j, k).up * refX(i, j + 1, k) : 0.0) +
				((k > 0) ? A(i, j, k - 1).front * refX(i, j, k - 1) : 0.0) +
				((k + 1 < size.z) ? A(i, j, k).front * refX(i, j, k + 1) : 0.0);

			refXTemp(i, j, k) = (b(i, j, k) - r) / A(i, j, k).center;
		});
	}
}