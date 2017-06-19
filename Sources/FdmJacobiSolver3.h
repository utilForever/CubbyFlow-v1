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

namespace CUBBYFLOW
{
	//!
	//! \brief 3-D FdmJacobi solver.
	//!
	//! This class implelmeents 3-D finite difference method Jacobian solver.
	//!
	class FdmJacobiSolver3 {
	public:
		void solve();
	};
}

#endif