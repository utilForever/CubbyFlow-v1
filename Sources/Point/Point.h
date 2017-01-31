/*************************************************************************
> File Name: Point.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D point class.
> Created Time: 2017/01/31
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT_H
#define CUBBYFLOW_POINT_H

#include <type_traits>

namespace CubbyFlow
{
	//!
	//! \brief Generic N-D point class.
	//!
	//! \tparam T - Number type.
	//! \tparam N - Dimension.
	//!
	template <typename T, size_t N>
	class Point final {
	public:
		static_assert(N > 0, "Size of static-sized point should be greater than zero.");
		static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmetic types");
	};
}

#endif