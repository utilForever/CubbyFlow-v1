/*************************************************************************
> File Name: Point.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D point class.
> Created Time: 2017/02/03
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Point/Point.h>

namespace CubbyFlow
{
	template <typename T, size_t N>
	Point<T, N>::Point()
	{
		for (auto& elem : m_elements)
		{
			elem = static_cast<T>(0);
		}
	}

	template <typename T, size_t N>
	template <typename... Params>
	Point<T, N>::Point(Params... params)
	{
		static_assert(sizeof...(params) == N, "Invalid number of parameters.");
		
		SetAt(0, params...);
	}

	template <typename T, size_t N>
	template <typename U>
	Point<T, N>::Point(const std::initializer_list<U>& list)
	{
		assert(list.size() >= N);

		size_t i = 0;
		for (const auto& inputElem : list)
		{
			m_elements[i] = static_cast<T>(inputElem);
			++i;
		}
	}

	template <typename T, size_t N>
	Point<T, N>::Point(const Point& other) :
		m_elements(other.m_elements)
	{
		// Do nothing
	}
}