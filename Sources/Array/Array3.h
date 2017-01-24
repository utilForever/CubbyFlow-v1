/*************************************************************************
> File Name: Array3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D array class.
> Created Time: 2017/01/24
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY3_H
#define CUBBYFLOW_ARRAY3_H

#include <Array/Array.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 3-D array class.
	//!
	//! This class represents 3-D array data structure. Internally, the 3-D data is
	//! mapped to a linear array such that (i, j, k) element is actually stroed at
	//! (i + width * (j + height * k))th element of the linear array. This mapping
	//! means iterating i first, j and k next will give better performance such as:
	//!
	//! \code{.cpp}
	//! Array<int, 3> array;
	//! for (size_t k = 0; k < array.depth(); ++k) {
	//!     for (size_t j = 0; j < array.height(); ++j) {
	//!         for (size_t i = 0; i < array.width(); ++i) {
	//!             // Read or write array(i, j, k)
	//!         }
	//!     }
	//! }
	//! \endcode
	//!
	//! \tparam T - Type to store in the array.
	//!
	template <typename T>
	class Array<T, 3> final
	{
		using ContainerType = std::vector<T>;
	};
}

#endif