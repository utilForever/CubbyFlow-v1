/*************************************************************************
> File Name: LevelSetUtils-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Level set util functions for CubbyFlow.
> Created Time: 2017/07/13
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

// Function fractionInside is from
// http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/

#ifndef CUBBYFLOW_LEVEL_SET_UTILS_IMPL_H
#define CUBBYFLOW_LEVEL_SET_UTILS_IMPL_H

#include <Utils/Constants.h>

#include <algorithm>

namespace CubbyFlow
{
	template <typename T>
	bool IsInsideSDF(T phi)
	{
		return phi < 0;
	}

	template <typename T>
	inline T SmearedHeavisideSDF(T phi)
	{
		if (phi > 1.5)
		{
			return 1;
		}
		
		if (phi < -1.5)
		{
			return 0;
		}

		return 0.5f + phi / 3.0 + 0.5f * (1 / PI<T>()) * std::sin(PI<T>() * phi / 1.5);
	}

	template <typename T>
	inline T SmearedDeltaSDF(T phi)
	{
		if (std::fabs(phi) > 1.5)
		{
			return 0;
		}

		return 1.0 / 3.0 + 1.0 / 3.0 * std::cos(PI<T>() * phi / 1.5);
	}

	template <typename T>
	T FractionInsideSDF(T phi0, T phi1)
	{
		if (IsInsideSDF(phi0) && IsInsideSDF(phi1))
		{
			return 1;
		}

		if (IsInsideSDF(phi0) && !IsInsideSDF(phi1))
		{
			return phi0 / (phi0 - phi1);
		}

		if (!IsInsideSDF(phi0) && IsInsideSDF(phi1))
		{
			return phi1 / (phi1 - phi0);
		}
		
		return 0;
	}

	template <typename T>
	T DistanceToZeroLevelSet(T phi0, T phi1)
	{
		if (std::fabs(phi0) + std::fabs(phi1) > std::numeric_limits<double>::epsilon())
		{
			return std::fabs(phi0) / (std::fabs(phi0) + std::fabs(phi1));
		}
		
		return static_cast<T>(0.5);
	}
}

#endif