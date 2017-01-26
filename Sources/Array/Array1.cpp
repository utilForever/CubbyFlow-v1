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
		for (auto& v : m_data)
		{
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
		for (size_t i = 0; i < size; ++i)
		{
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

	template <typename T>
	typename Array<T, 1>::ContainerType::iterator Array<T, 1>::Begin()
	{
		return m_data.begin();
	}

	template <typename T>
	typename Array<T, 1>::ContainerType::const_iterator Array<T, 1>::Begin() const
	{
		return m_data.cbegin();
	}

	template <typename T>
	typename Array<T, 1>::ContainerType::iterator Array<T, 1>::End()
	{
		return m_data.end();
	}

	template <typename T>
	typename Array<T, 1>::ContainerType::const_iterator Array<T, 1>::End() const
	{
		return m_data.cend();
	}

	template <typename T>
	ArrayAccessor1<T> Array<T, 1>::Accessor()
	{
		return ArrayAccessor1<T>(Size(), Data());
	}

	template <typename T>
	ConstArrayAccessor1<T> Array<T, 1>::ConstAccessor() const
	{
		return ConstArrayAccessor1<T>(Size(), Data());
	}

	template <typename T>
	void Array<T, 1>::Swap(Array& other)
	{
		std::swap(other.m_data, m_data);
	}

	template <typename T>
	void Array<T, 1>::Append(const T& newVal)
	{
		m_data.push_back(newVal);
	}

	template <typename T>
	void Array<T, 1>::Append(const Array& other)
	{
		m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 1>::ForEach(Callback func) const
	{
		ConstAccessor().ForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 1>::ForEachIndex(Callback func) const
	{
		ConstAccessor().ForEachIndex(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 1>::ParallelForEach(Callback func)
	{
		Accessor().ParallelForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 1>::ParallelForEachIndex(Callback func) const
	{
		ConstAccessor().ParallelForEachIndex(func);
	}

	template <typename T>
	T& Array<T, 1>::operator[](size_t i)
	{
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 1>::operator[](size_t i) const
	{
		return m_data[i];
	}

	template <typename T>
	Array<T, 1>& Array<T, 1>::operator=(const T& value)
	{
		Set(value);
		return *this;
	}

	template <typename T>
	Array<T, 1>& Array<T, 1>::operator=(const Array& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	Array<T, 1>& Array<T, 1>::operator=(const std::initializer_list<T>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T>
	Array<T, 1>::operator ArrayAccessor1<T>()
	{
		return Accessor();
	}

	template <typename T>
	Array<T, 1>::operator ConstArrayAccessor1<T>() const
	{
		return ConstAccessor();
	}
}