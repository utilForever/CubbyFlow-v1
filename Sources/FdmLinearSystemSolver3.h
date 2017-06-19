/*************************************************************************
> File Name: FdmLinearSystemSolver3.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Implements finite difference method linear system solver.
> Created Time: 2017/06/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEARSYSTEM_SOLVER_H
#define CUBBYFLOW_FDM_LINEARSYSTEM_SOLVER_H

#include "Fdm.h"
#include "FdmBlas3.h"

namespace CubbyFlow
{
	class FdmLinearSystemSolver3
	{
	public:
		virtual bool solve(FdmLinearSystem3* system) = 0;
	};
}
#endif