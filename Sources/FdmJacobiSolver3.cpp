/*************************************************************************
> File Name: FdmJacobiSolver3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D JacobiSolver
> Created Time: 2017/06/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include"FdmJacobiSolver3.h"
#include"FdmBlas3.h"

namespace CubbyFlow
{
	template <typename T>
	bool FdmJacobiSolver3::solve(FdmLinearSystem3* system)
	{
		_xTemp.resize(system->x.size());
		_residual.resize(system->x.size());

		for (unsigned int iter = 0; iter < _maxNumberOfIterations; ++iter)
		{
			relax(system, &_xTemp);

			_xTemp.swap(system->x);

			if (iter != 0 && iter && _residualCheckInterval == 0)
			{
				FdmBlas3::residual(
					system->A, system->x, system->b, &_residaul);

				if (FdmBlas3::l2Norm(_residual) < _tolerance)
				{
					break;
				}
			}
		}

		FdmBlas3::residual(system->A, system->x, system->b, &_residual);
		_lastResidaul = FdmBlas3::l2Norm(_residual);
		
		return _lastResidual < _tolerance;
	}

	template <typename T>
	void FdmJacobiSolver3::relax(
		FdmLinearSystem3* system, FdmVector3* xTemp)
	{
		Size3 size = system->x.size();
		FdmMatrix3& A = system->A;
		FdmVector3& x = system->x;
		FdmVector3& b = system->b;

		A.parallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			double r = ((i > 0) ? A(i - 1, j, k).right * x(i - 1, j, k) : 0.0)
				+ ((i + 1 < size.x) ? A(i, j, k).right * x(i + 1, j, k) : 0.0)
				+ ((j > 0) ? A(i, j - 1, k).up * x(i, j - 1, k) : 0.0)
				+ ((j + 1 < size.y) ? A(i, j, k).up * x(i, j + 1, k) : 0.0)
				+ ((k > 0) ? A(i, j, k - 1).front * x(i, j, k - 1) : 0.0)
				+ ((k + 1 < size.z) ? A(i, j, k).front * x(i, j, k + 1) : 0.0);

			(*xTemp)(i, j, k) = (b(i, j, k) - r) / A(i, j, k).center;
		});
	}
}