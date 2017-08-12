/*************************************************************************
> File Name: FDMICCGSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D finite difference-type linear system solver using incomplete
>          Cholesky conjugate gradient (ICCG).
> Created Time: 2017/08/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_ICCG_SOLVER3_H
#define CUBBYFLOW_FDM_ICCG_SOLVER3_H

#include <Solver/FDM/FDMLinearSystemSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D finite difference-type linear system solver using incomplete
	//!        Cholesky conjugate gradient (ICCG).
	//!
	class FDMICCGSolver3 final : public FDMLinearSystemSolver3
	{
	public:
		//! Constructs the solver with given parameters.
		FDMICCGSolver3(unsigned int maxNumberOfIterations, double tolerance);

		//! Solves the given linear system.
		bool Solve(FDMLinearSystem3* system) override;

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
			ConstArrayAccessor3<FDMMatrixRow3> A;
			FDMVector3 d;
			FDMVector3 y;

			void Build(const FDMMatrix3& matrix);

			void Solve(const FDMVector3& b, FDMVector3* x);
		};

		unsigned int m_maxNumberOfIterations;
		unsigned int m_lastNumberOfIterations;
		double m_tolerance;
		double m_lastResidualNorm;

		FDMVector3 m_r;
		FDMVector3 m_d;
		FDMVector3 m_q;
		FDMVector3 m_s;
		Preconditioner m_precond;
	};

	//! Shared pointer type for the FDMICCGSolver3.
	using FDMICCGSolver3Ptr = std::shared_ptr<FDMICCGSolver3>;
}

#endif