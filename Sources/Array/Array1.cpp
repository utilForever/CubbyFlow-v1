/*************************************************************************
> File Name: Array1.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 1-D array class.
> Created Time: 2017/01/24
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Array/Array1.h>

#include <cassert>

namespace CubbyFlow
{
	template <typename T>
	Array<T, 1>::Array()
	{
		// Do nothing
	}

	template <typename T>
	Array<T, 1>::Array(size_t size, const T& initVal)
	{
		Resize(size, initVal);
	}

	template <typename T>
	Array<T, 1>::Array(const std::initializer_list<T>& list)
	{
		Set(list);
	}

	template <typename T>
	Array<T, 1>::Array(const Array& other)
	{
		Set(other);
	}

	template <typename T>
	void Array<T, 1>::Set(const T& value)
	{
		for (auto& v : m_data) {
			v = value;
		}
	}

	template <typename T>
	void Array<T, 1>::Set(const Array& other)
	{
		m_data.resize(other.m_data.size());
		std::copy(other.m_data.begin(), other.m_data.end(), m_data.begin());
	}

	template <typename T>
	void Array<T ,1>::Set(const std::initializer_list<T>& list)
	{
		size_t size = list.size();
		Resize(size);
		
		auto colIter = list.begin();
		for (size_t i = 0; i < size; ++i) {
			(*this)[i] = *colIter;
			++colIter;
		}
	}

	template <typename T>
	void Array<T, 1>::Clear()
	{
		m_data.clear();
	}

	template <typename T>
	void Array<T, 1>::Resize(size_t size, const T& initVal)
	{
		m_data.resize(size, initVal);
	}

	template <typename T>
	T& Array<T, 1>::At(size_t i)
	{
		assert(i < Size());
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 1>::At(size_t i) const
	{
		assert(i < Size());
		return m_data[i];
	}

	template <typename T>
	size_t Array<T, 1>::Size() const
	{
		return m_data.size();
	}

	template <typename T>
	T* Array<T, 1>::Data()
	{
		return m_data.data();
	}

	template <typename T>
	const T* Array<T, 1>::Data() const
	{
		return m_data.data();
	}
}