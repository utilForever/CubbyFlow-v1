/*************************************************************************
> File Name: Matrix.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Static-sized M x N matrix class.
> Created Time: 2017/03/06
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Matrix\Matrix.h>

namespace CubbyFlow
{
	template<typename T, size_t M, size_t N>
	Matrix<T, M, N>::Matrix() : elements(0)
	{
		// Do nothing
	}
}	