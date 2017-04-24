/*************************************************************************
> File Name: Matrix.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Static-sized M x N matrix class.
> Created Time: 2017/03/06
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MATRIX_H
#define CUBBYFLOW_MATRIX_H

#include <array>
#include <type_traits>

namespace CubbyFlow
{
	//!
	//! \brief Static-sized M x N matrix class.
	//!
	//! \tparam T - Real number type.
	//! \tparam M - Number of rows.
	//! \tparam N - Number of columns.
	//!
	template <typename T, size_t M, size_t N>
	class Matrix
	{
	public:
		static_assert(M > 0, "Number of rows for static-sized matrix should be greater than zero.");
		static_assert(N > 0, "Number of columns for static-sized matrix should be greater than zero.");
		static_assert(std::is_floating_point<T>::value, "Matrix only can be instantiated with floating point types");

		std::array<T, M * N> elements;

		//! Default constructor.
		//! \warning This constructor will create zero matrix.
		Matrix();
	};
}

#endif