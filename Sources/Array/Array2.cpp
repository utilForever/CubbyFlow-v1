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

	template <typename T>
	void Array<T, 2>::Set(const T& value)
	{
		for (auto& v : m_data)
		{
			v = value;
		}
	}

	template <typename T>
	void Array<T, 2>::Set(const Array& other)
	{
		m_data.resize(other.m_data.size());
		std::copy(other.m_data.begin(), other.m_data.end(), m_data.begin());
		m_size = other.m_size;
	}

	template <typename T>
	void Array<T, 2>::Set(const std::initializer_list<std::initializer_list<T>>& list)
	{
		size_t height = list.size();
		size_t width = (height > 0) ? list.begin()->size() : 0;
		
		Resize(Size2(width, height));
		
		auto rowIter = list.begin();
		for (size_t j = 0; j < height; ++j)
		{
			assert(width == rowIter->size());

			auto colIter = rowIter->begin();
			for (size_t i = 0; i < width; ++i)
			{
				(*this)(i, j) = *colIter;
				++colIter;
			}

			++rowIter;
		}
	}

	template <typename T>
	void Array<T, 2>::Clear()
	{
		m_data.clear();
		m_size = Size2(0, 0);
	}

	template <typename T>
	void Array<T, 2>::Resize(const Size2& size, const T& initVal)
	{
		Array grid;
		grid.m_data.resize(size.x * size.y, initVal);
		grid.m_size = size;
		
		size_t iMin = std::min(size.x, m_size.x);
		size_t jMin = std::min(size.y, m_size.y);
		for (size_t j = 0; j < jMin; ++j)
		{
			for (size_t i = 0; i < iMin; ++i)
			{
				grid(i, j) = at(i, j);
			}
		}

		Swap(grid);
	}

	template <typename T>
	void Array<T, 2>::Resize(size_t width, size_t height, const T& initVal)
	{
		Resize(Size2(width, height), initVal);
	}

	template <typename T>
	T& Array<T, 2>::At(size_t i)
	{
		assert(i < m_size.x * m_size.y);
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 2>::At(size_t i) const
	{
		assert(i < m_size.x * m_size.y);
		return m_data[i];
	}

	template <typename T>
	T& Array<T, 2>::At(const Point2UI& pt)
	{
		return At(pt.x, pt.y);
	}

	template <typename T>
	const T& Array<T, 2>::At(const Point2UI& pt) const
	{
		return At(pt.x, pt.y);
	}

	template <typename T>
	T& Array<T, 2>::At(size_t i, size_t j)
	{
		assert(i < m_size.x && j < m_size.y);
		return m_data[i + m_size.x * j];
	}

	template <typename T>
	const T& Array<T, 2>::At(size_t i, size_t j) const
	{
		assert(i < m_size.x && j < m_size.y);
		return m_data[i + m_size.x * j];
	}
}