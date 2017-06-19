/*************************************************************************
> File Name: Fdm.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Basic Finite difference method.
> Created Time: 2017/06/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_FDM_H
#define CUBBYFLOW_FDM_H

#include "../Includes/Array/Array3.h"

namespace CUBBYFLOW
{
	template<typename T>
	typedef Array3<double> FdmVector3;

	struct FdmMatrixRow3
	{
		double center = 0.0;
		double right = 0.0;
		double up = 0.0;
		double front = 0.0;
	};

	template<typename FdmMatrixRow3> typedef Array3<FdmMatrixRow3> FdmMatrix3;

	struct FdmLinearSystem3
	{
		template<typename FdmMatrixRow3>
		FdmMatrix3 A;

		template<typename T>
		FdmVector3 x;

		template<typename T>
		FdmVector3 b;
	};
}

#endif