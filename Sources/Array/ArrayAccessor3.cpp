/*************************************************************************
> File Name: ArrayAccessor3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk, Kim Dongmin
> Purpose: 3-D array accessor class.
> Created Time: 2017/01/28
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include "ArrayAccessor3.h"

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
	template <typename T>
	ArrayAccessor<T, 3>::ArrayAccessor() :
		m_size(0), m_data(nullptr)
	{
		// Do nothing
	}

	template <typename T>
	ArrayAccessor<T, 3>::ArrayAccessor(const Size3& size, T* const data)
	{
		Reset(size, data);
	}

	template <typename T>
	ArrayAccessor<T, 3>::ArrayAccessor(size_t width, size_t height, size_t depth, T* const data)
	{
		Reset(width, height, depth, data);
	}

	template <typename T>
	ArrayAccessor<T, 3>::ArrayAccessor(const ArrayAccessor& other)
	{
		Set(other);
	}

	template <typename T>
	ArrayAccessor<T, 3>::Set(const ArrayAccessor& other)
	{
		Reset(other.m_size, other.m_data);
	}

	template <typename T>
	void ArrayAccessor<T, 3>::Reset(Size3& size, T* const data)
	{
		m_size = size;
		m_data = data;
	}

	template <typename T>
	void ArrayAccessor<T, 3>::Reset(size_t width, size_t height, size_t depth, T* const data)
	{
		m_size = Size3(width, hegiht, depth);
		m_data = data;
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::At(size_t i)
	{
		assert(i < Width() * Height() * Depth());
		return m_data[i];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::At(size_t i) const
	{
		assert(i < Width() * Height() * Depth());
		return m_data[i];
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::At(const Point3UI& pt)
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::At(const Point3UI& pt) const
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k)
	{
		assert(i < Width());
		assert(j < Height());
		assert(k < Depth());
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k) const
	{
		assert(i < Width());
		assert(j < Height());
		assert(k < Depth());
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	const T* const ArrayAccessor<T, 3>::Begin() const
	{
		return m_data;
	}

	template <typename T>
	const T* const ArrayAccessor<T, 3>::End() const
	{
		return m_data + Width() * Height();
	}

	template <typename T>
	T* const ArrayAccessor<T, 3>::Begin()
	{
		return m_data;
	}

	template <typename T>
	T* const ArrayAccessor<T, 3>::End()
	{
		return m_data + Width() * Height() * Depth();
	}

	template <typename T>
	Size3 ArrayAccessor<T, 3>::Size() const
	{
		return m_size;
	}

	template <typename T>
	size_t ArrayAccessor<T, 3>::Width() const
	{
		return m_size.x;
	}

	template <typename T>
	size_t ArrayAccessor<T, 3>::Height() const
	{
		return m_size.y;
	}

	template <typename T>
	size_t ArrayAccessor<T, 3>::Depth() const
	{
		return m_size.z;
	}

	template <typename T>
	T* const ArrayAccessor<T, 3>::Data() const
	{
		return m_data;
	}

	template <typename T>
	void ArrayAccessor<T, 3>::Swap(ArrayAccessor& other)
	{
		std::swap(other.m_data, m_data);
		std::swap(ohter.m_size, other.m_size);
	}

	template <typename T>
	template <typename Callback>
	void ArrayAccessor<T, 3>::ForEach(Callback func) const
	{
		for (size_t k = 0; k < Depth(); ++k)
		{
			for (size_t j = 0; j < Height(); ++j)
			{
				for (size_t i = 0; i < Width(); ++i)
				{
					func(At(i, j, k));
				}
			}
		}
		
	}

	template <typename T>
	template <typename Callback>
	void ArrayAccessor<T, 3>::ForEachIndex(Callback func) const
	{
		for (size_t k = 0; k < Depth(); ++k)
		{
			for (size_t j = 0; j < Height(); ++j)
			{
				for (size_t i = 0; i < Width(); ++i)
				{
					func(i, j, k);
				}
			}
		}
		
	}

	template <typename T>
	template <typename Callback>
	void ArrayAccessor<T, 3>::ParallelForEach(Callback func)
	{
		ParallelFor(ZERO_SIZE, Size(), [&](size_t i, size_t j, size_t k)
		{
			func(At(i, j, k));
		});
	}

	template <typename T>
	template <typename Callback>
	void ArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const
	{
		ParallelFor(ZERO_SIZE, Size(), [&](size_t i, size_t j, size_t k)
		{
			func(i, j, k);
		});
	}

	template <typename T>
	size_t ArrayAccessor<T, 3>::Index(const Point3UI& pt) const
	{
		return pt.x + pt.y * Width() + pt.z * Width() * Height();
	}

	template <typename T>
	size_t ArrayAccessor<T, 3>::Index(size_t i, size_t j, size_t k) const
	{
		return i + j * Width() + k * Width() * Height();
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::operator[](size_t i)
	{
		return m_data[i];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::operator[](size_t i) const
	{
		return m_data[i];
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::operator()(const Point3UI& pt)
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::operator()(const Point3UI& pt) const
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	T& ArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k)
	{
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	const T& ArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k) const
	{
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	ArrayAccessor& ArrayAccessor<T, 3>::operator=(const ArrayAccessor& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	ArrayAccessor<T, 3>::operator ConstArrayAccessor<T, 3>() const
	{
		return ConstArrayAccessor<T, 3>(*this);
	}

	template <typename T>
	ConstArrayAccessor<T, 3>::ConstArrayAccessor() :
		m_size(0), m_data(nullptr)
	{
		// Do nothing
	}

	template <typename T>
	ConstArrayAccessor<T, 3>::ConstArrayAccessor(const Size3& size, T* const data)
	{
		m_size = size;
		m_data = data;
	}

	template <typename T>
	ConstArrayAccessor<T, 3>::ConstArrayAccessor(size_t width, size_t height, size_t depth, T* const data)
	{
		m_size = Size3(width, height, depth);
		m_data = data
	}

	template <typename T>
	ConstArrayAccessor<T, 3>::ConstArrayAccessor(const ArrayAccessor<T, 3>& other)
	{
		m_size = other.Size();
		m_data = other.Data();
	}

	template <typename T>
	void ConstArrayAccessor<T, 3>::ConstArrayAccessor(const ConstArrayAccessor& other)
	{
		m_size = other.m_size;
		m_data = other.m_data;
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::At(size_t i) const
	{
		assert(i < Width() * Height() * Depth());
		return m_data[i];
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::At(const Point3UI& pt) const
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::At(size_t i, size_t j, size_t k) const
	{
		assert(i < Width());
		assert(j < Height());
		assert(k < Depth());
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	const T* const ConstArrayAccessor<T, 3>::Begin() const
	{
		return m_data;
	}

	template <typename T>
	const T* const ConstArrayAccessor<T, 3>::End() const
	{
		return m_data + Width() * Height() * Depth();
	}

	template <typename T>
	Size3 ConstArrayAccessor<T, 3>::Size() const
	{
		return m_size;
	}

	template <typename T>
	size_t ConstArrayAccessor<T, 3>::Width() const
	{
		return m_size.x;
	}

	template <typename T>
	size_t ConstArrayAccessor<T, 3>::Height() const
	{
		return m_size.y;
	}

	template <typename T>
	size_t ConstArrayAccessor<T, 3>::Depth() const
	{
		return m_size.z;
	}

	template <typename T>
	T* const ConstArrayAccessor<T, 3>::Data() const
	{
		return m_data;
	}

	template <typename T>
	void ConstArrayAccessor<T, 3>::Swap(ConstArrayAccessor& other)
	{
		std::swap(other.m_data, m_data);
		std::swap(ohter.m_size, other.m_size);
	}

	template <typename T>
	template <typename Callback>
	void ConstArrayAccessor<T, 3>::ForEach(Callback func) const
	{
		for (size_t k = 0; k < Depth(); ++k)
		{
			for (size_t j = 0; j < Height(); ++j)
			{
				for (size_t i = 0; i < Width(); ++i)
				{
					func(At(i, j, k));
				}
			}
		}
	}

	template <typename T>
	template <typename Callback>
	void ConstArrayAccessor<T, 3>::ForEachIndex(Callback func) const
	{
		for (size_t k = 0; k < Depth(); ++k)
		{
			for (size_t j = 0; j < Height(); ++j)
			{
				for (size_t i = 0; i < Width(); ++i)
				{
					func(i, j, k);
				}
			}
		}
	}

	template <typename T>
	template <typename Callback>
	void ConstArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const
	{
		ParallelFor(ZERO_SIZE, m_size.x, ZERO_SIZE, m_size.y, ZERO_SIZE, m_size.z, func);
	}

	template <typename T>
	size_t ConstArrayAccessor<T, 3>::Index(const Point3UI& pt) const
	{
		return pt.x + pt.y * Width() + pt.z * Width() * Height();
	}

	template <typename T>
	size_t ConstArrayAccessor<T, 3>::Index(size_t i, size_t j, size_t k) const
	{
		return i + j * Width() + k * Width() * Height();
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::operator[](size_t i) const
	{
		return m_data[i];
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::operator()(const Point3UI& pt) const
	{
		return m_data[Index(pt)];
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k) const
	{
		return m_data[Index(i, j, k)];
	}

	template <typename T>
	const T& ConstArrayAccessor<T, 3>::operator()(const Point3UI& pt) const
	{
		return m_data[Index(pt)];
	}
}
