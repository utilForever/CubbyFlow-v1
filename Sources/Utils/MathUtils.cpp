/*************************************************************************
> File Name: MathUtils.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Mathematics util functions for CubbyFlow.
> Created Time: 2017/02/27
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Utils/MathUtils.h>

namespace CubbyFlow
{
	template <typename T>
	inline T AbsMin(T x, T y)
	{
		return (x * x < y * y) ? x : y;
	}

	template <typename T>
	inline T AbsMax(T x, T y)
	{
		return (x * x > y * y) ? x : y;
	}
}