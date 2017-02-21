/*************************************************************************
> File Name: Vector.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/19
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_H
#define CUBBYFLOW_VECTOR_H

#include <array>
#include <type_traits>
#include <math.h>

namespace CubbyFlow
{
	//!
	//! \brief Generic N-D vector class.
	//!
	//! \tparam T - Number type.
	//! \tparam N - Dimension.
	//!

	template <typename T, size_t N>
	class Vector final
	{
	public:
		static_assert(N > 0, "Size of static-sized vector should be greater than zero.");
		static_assert(std::is_floating_point<T>::value, "Vector only can be instantiated with floating point types");

		//! Constructs a vector with zeros.
		Vector();

		//! Constructs vector instance with parameters.
		template <typename... Params>
		explicit Vector(Params... params);

		//! Constructs vector instance with initializer list.
		template <typename U>
		Vector(const std::initializer_list<U>& list);

		//! Copy constructor.
		Vector(const Vector& other);

		//! Set vector instance with initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Set vector instance with other vector.
		void Set(const Vector& other);

		//! Set vector instance with initializer list.
		template <typename U>
		Vector& operator=(const std::initializer_list<U>& list);

		//! Set vector instance with other vector.
		Vector& operator=(const Vector& other);

		//! Returns the const reference to the \p i -th element.
		const T& operator[](size_t i) const;

		//! Returns the reference to the \p i -th element.
		T& operator[](size_t i);

	private:
		std::array<T, N> m_elements;

		template <typename... Params>
		void SetAt(size_t i, T v, Params... params);
		void SetAt(size_t i, T v);
	};

	//! Returns the type of the value.
	template <typename T, size_t N>
	struct ScalarType<Vector<T, N>>
	{
		typedef T value;
	};
}

#endif