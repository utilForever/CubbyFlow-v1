/*************************************************************************
> File Name: FDMCGSolver2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D finite difference-type linear system solver using conjugate gradient.
> Created Time: 2017/08/16
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Math/CG.h>
#include <Solver/FDM/FDMCGSolver2.h>

namespace CubbyFlow
{
	FDMCGSolver2::FDMCGSolver2(unsigned int maxNumberOfIterations, double tolerance) :
		m_maxNumberOfIterations(maxNumberOfIterations),
		m_lastNumberOfIterations(0),
		m_tolerance(tolerance),
		m_lastResidual(std::numeric_limits<double>::max())
	{
		// Do nothing
	}

	bool FDMCGSolver2::Solve(FDMLinearSystem2* system)
	{
		FDMMatrix2& matrix = system->A;
		FDMVector2& solution = system->x;
		FDMVector2& rhs = system->b;

		assert(matrix.Size() == rhs.Size());
		assert(matrix.Size() == solution.Size());

		Size2 size = matrix.Size();
		m_r.Resize(size);
		m_d.Resize(size);
		m_q.Resize(size);
		m_s.Resize(size);

		system->x.Set(0.0);
		m_r.Set(0.0);
		m_d.Set(0.0);
		m_q.Set(0.0);
		m_s.Set(0.0);

		CG<FDMBlas2>(
			matrix,
			rhs,
			m_maxNumberOfIterations,
			m_tolerance,
			&solution,
			&m_r,
			&m_d,
			&m_q,
			&m_s,
			&m_lastNumberOfIterations,
			&m_lastResidual);

		return (m_lastResidual <= m_tolerance) || (m_lastNumberOfIterations < m_maxNumberOfIterations);
	}

	unsigned int FDMCGSolver2::GetMaxNumberOfIterations() const
	{
		return m_maxNumberOfIterations;
	}

	unsigned int FDMCGSolver2::GetLastNumberOfIterations() const
	{
		return m_lastNumberOfIterations;
	}

	double FDMCGSolver2::GetTolerance() const
	{
		return m_tolerance;
	}

	double FDMCGSolver2::GetLastResidual() const
	{
		return m_lastResidual;
	}
}