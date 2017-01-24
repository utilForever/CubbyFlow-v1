/*************************************************************************
> File Name: Array2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D array class.
> Created Time: 2017/01/25
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Array/Array2.h>

#include <cassert>

namespace CubbyFlow
{
	template <typename T>
	Array<T, 2>::Array()
	{
		// Do nothing
	}

	template <typename T>
	Array<T, 2>::Array(const Size2& size, const T& initVal)
	{
		Resize(size, initVal);
	}

	template <typename T>
	Array<T, 2>::Array(size_t width, size_t height, const T& initVal)
	{
		Resize(width, height, initVal);
	}

	template <typename T>
	Array<T, 2>::Array(const std::initializer_list<std::initializer_list<T>>& list)
	{
		Set(list);
	}

	template <typename T>
	Array<T, 2>::Array(const Array& other)
	{
		Set(other);
	}
}