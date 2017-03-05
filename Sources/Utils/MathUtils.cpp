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
	inline T Sign(T x)
	{
		if (x >= 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	template <typename T>
	inline T AbsMin(T x, T y)
	{
		return (x * x < y * y) ? x : y;
	}

	template <typename T>
	inline T AbsMin(T x, T y, T z)
	{
		if (AbsMin(x, y) == x)
		{
			if (AbsMin(x, z) == x)
			{
				return x;
			}
			else
			{
				return z;
			}
		}
		else
		{
			if (AbsMin(y, z) == y)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	template <typename T>
	inline T AbsMin(T x, T y, T z, T w)
	{
		return AbsMin(AbsMin(x, y), AbsMin(z, w));
	}

	template <typename T>
	inline T AbsMax(T x, T y)
	{
		return (x * x > y * y) ? x : y;
	}

	template <typename T>
	inline T AbsMax(T x, T y, T z)
	{
		if (AbsMax(x, y) == x)
		{
			if (AbsMax(x, z) == x)
			{
				return x;
			}
			else
			{
				return z;
			}
		}
		else
		{
			if (AbsMax(y, z) == y)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	template <typename T>
	inline T AbsMax(T x, T y, T z, T w)
	{
		return AbsMax(AbsMax(x, y), AbsMax(z, w));
	}

	template <typename T>
	inline T Square(T x)
	{
		return x * x;
	}

	template <typename T>
	inline T Cubic(T x)
	{
		return x * x * x;
	}

	template <typename T>
	inline T Clamp(T val, T low, T high)
	{
		if (val < low)
		{
			return low;
		}

		if (val > high)
		{
			return high;
		}
		
		return val;
	}
}