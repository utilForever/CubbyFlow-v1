/*************************************************************************
> File Name: MathUtils.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Mathematics util functions for CubbyFlow.
> Created Time: 2017/02/27
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Utils/MathUtils.h>

#include <algorithm>

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

	template<typename T>
	inline void GetBarycentric(T x, ssize_t iLow, ssize_t iHigh, ssize_t* i, T* f)
	{
		T s = std::floor(x);
		*i = static_cast<ssize_t>(s);

		ssize_t offset = -iLow;
		iLow += offset;
		iHigh += offset;

		if (iLow == iHigh)
		{
			*i = iLow;
			*f = 0;
		}
		else if (*i < iLow)
		{
			*i = iLow;
			*f = 0;
		}
		else if (*i > iHigh - 1)
		{
			*i = iHigh - 1;
			*f = 1;
		}
		else
		{
			*f = static_cast<T>(x - s);
		}

		*i -= offset;
	}

	template<typename S, typename T>
	inline S Lerp(const S& value0, const S& value1, T f)
	{
		return (1 - f) * value0 + f * value1;
	}

	template<typename S, typename T>
	inline S BiLerp(
		const S& f00, const S& f10,
		const S& f01, const S& f11,
		T tx, T ty)
	{
		return Lerp(Lerp(f00, f10, tx), Lerp(f01, f11, tx), ty);
	}

	template<typename S, typename T>
	inline S TriLerp(
		const S& f000, const S& f100,
		const S& f010, const S& f110,
		const S& f001, const S& f101,
		const S& f011, const S& f111,
		T tx, T ty, T fz)
	{
		return Lerp(
			BiLerp(f000, f100, f010, f110, tx, ty),
			BiLerp(f001, f101, f011, f111, tx, ty),
			fz);
	}
}