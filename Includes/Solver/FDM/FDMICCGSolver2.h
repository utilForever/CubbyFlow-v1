/*************************************************************************
> File Name: FDMICCGSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D finite difference-type linear system solver using incomplete
>          Cholesky conjugate gradient (ICCG).
> Created Time: 2017/08/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_ICCG_SOLVER2_H
#define CUBBYFLOW_FDM_ICCG_SOLVER2_H

#include <Solver/FDM/FDMLinearSystemSolver2.h>

namespace CubbyFlow
{
	class FDMICCGSolver2 final : public FDMLinearSystemSolver2
	{
	public:
		//! Constructs the solver with given parameters.
		FDMICCGSolver2(unsigned int maxNumberOfIterations, double tolerance);

		//! Solves the given linear system.
		bool Solve(FDMLinearSystem2* system) override;

		//! Returns the max number of Jacobi iterations.
		unsigned int GetMaxNumberOfIterations() const;

		//! Returns the last number of Jacobi iterations the solver made.
		unsigned int GetLastNumberOfIterations() const;

		//! Returns the max residual tolerance for the Jacobi method.
		double GetTolerance() const;

		//! Returns the last residual after the Jacobi iterations.
		double GetLastResidual() const;

	private:
		struct Preconditioner final
		{
			ConstArrayAccessor2<FDMMatrixRow2> A;
			FDMVector2 d;
			FDMVector2 y;

			void Build(const FDMMatrix2& matrix);

			void Solve(const FDMVector2& b, FDMVector2* x);
		};

		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidualNorm;

		FDMVector2 m_r;
		FDMVector2 m_d;
		FDMVector2 m_q;
		FDMVector2 m_s;
		Preconditioner m_precond;
	};

	//! Shared pointer type for the FDMICCGSolver2.
	using FDMICCGSolver2Ptr = std::shared_ptr<FDMICCGSolver2>;
}

#endif