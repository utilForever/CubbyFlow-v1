/*************************************************************************
> File Name: Constants.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Constant lists for CubbyFlow.
> Created Time: 2017/01/31
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CONSTANTS_H
#define CUBBYFLOW_CONSTANTS_H

#include <Utils/Macros.h>

namespace CubbyFlow
{
	// MARK: Zero
	//! Zero size_t.
	constexpr size_t ZERO_SIZE = 0;
	//! Zero ssize_t.
	constexpr ssize_t ZERO_SSIZE = 0;

	//! Zero for type T.
	template <typename T>
	inline T Zero()
	{
		return 0;
	}

	//! Zero for float.
	template <>
	inline float Zero<float>()
	{
		return 0.f;
	}

	//! Zero for double.
	template <>
	inline double Zero<double>()
	{
		return 0.0;
	}

	// MARK: PI
	//! Float-type PI.
	constexpr float PI_FLOAT = 3.14159265358979323846264338327950288f;
	//! Double-type PI.
	constexpr double PI_DOUBLE = 3.14159265358979323846264338327950288;

	//! PI for type T.
	template <typename T>
	constexpr T PI()
	{
		return static_cast<T>(PI_DOUBLE);
	}

	//! Pi for float.
	template <>
	constexpr float PI<float>()
	{
		return PI_FLOAT;
	}

	//! Pi for double.
	template <>
	constexpr double PI<double>()
	{
		return PI_DOUBLE;
	}
}

#endif