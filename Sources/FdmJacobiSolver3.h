/*************************************************************************
> File Name: FdmJacobiSolver3.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D JacobiSolver
> Created Time: 2017/06/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_FDMJACOBI_SOLVER3_H
#define CUBBYFLOW_FDMJACOBI_SOLVER3_H

#include "FdmLinearSystemSolver3.h"
#include "Fdm.h"

namespace CubbyFlow
{
	//!
	//! \brief 3-D FdmJacobi solver.
	//!
	//! This class implelmeents 3-D finite difference method Jacobian solver.
	//!
	class FdmJacobiSolver3 final : public FdmLinearSystemSolver3{
	public:
		bool solve(FdmLinearSystem3* system) override;

	private:
		unsigned int _maxNumberOfIterations;
		unsigned int _lastNumberOfIterations;
		unsigned int _residualCheckInterval;
		double _tolerance;
		double _lastResidual;

		template<typename T>
		FdmVector3 _xTemp;

		template<typename T>
		FdmVector3 _residual;
		
		template<typename T>
		void relax(FdmLinearSystem3* system, FdmVector3* xTemp);
	};
}

#endif