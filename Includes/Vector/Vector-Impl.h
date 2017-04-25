/*************************************************************************
> File Name: Vector-Impl.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_IMPL_H
#define CUBBYFLOW_VECTOR_IMPL_H

namespace CubbyFlow
{
	template <typename T, size_t N>
	Vector<T, N>::Vector()
	{
		for (auto& elem : m_elements)
		{
			elem = static_cast<T>(0);
		}
	}

	template <typename T, size_t N>
	template <typename... Params>
	Vector<T, N>::Vector(Params... params)
	{
		static_assert(sizeof...(params) == N, "Invalid number of parameters");

		SetAt(0, params...);
	}

	template <typename T, size_t N>
	template <typename U>
	Vector<T, N>::Vector(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T, size_t N>
	Vector<T, N>::Vector(const Vector& other) :
		m_elements(other.m_elements)
	{

	}

	template <typename T, size_t N>
	template <typename U>
	void Vector<T, N>::Set(const std::initializer_list<U>& list)
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
	void Vector<T, N>::Set(const Vector& other)
	{
		m_elements = other.m_elements;
	}

	template <typename T, size_t N>
	template <typename U>
	Vector<T, N>& Vector<T, N>::operator=(const std::initializer_list<U>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T, size_t N>
	Vector<T, N>& Vector<T, N>::operator=(const Vector& other)
	{
		Set(other);
		return *this;
	}

	template <typename T, size_t N>
	const T& Vector<T, N>::operator[](size_t i) const
	{
		return m_elements[i];
	}

	template <typename T, size_t N>
	T& Vector<T, N>::operator[](size_t i)
	{
		return m_elements[i];
	}
	
	template <typename T, size_t N>
	template <typename... Params>
	void Vector<T, N>::SetAt(size_t i, T v, Params... params)
	{
		m_elements[i] = v;

		SetAt(i + 1, params...);
	}

	template <typename T, size_t N>
	void Vector<T, N>::SetAt(size_t i, T v)
	{
		m_elements[i] = v;
	}
}

#endif