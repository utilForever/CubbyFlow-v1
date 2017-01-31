/*************************************************************************
> File Name: Point.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D point class.
> Created Time: 2017/02/01
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT2_H
#define CUBBYFLOW_POINT2_H

#include <Point/Point.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D point class.
	//!
	//! This class defines simple 2-D point data.
	//!
	//! \tparam T - Type of the element
	//!
	template <typename T>
	class Point<T, 2> final
	{
	public:
		static_assert(std::is_arithmetic<T>::value,	"Point only can be instantiated with arithmetic types");

		//! X (or the first) component of the point.
		T x;

		//! Y (or the second) component of the point.
		T y;

		// MARK: Constructors
		//! Constructs default point (0, 0).
		Point();

		//! Constructs point with given parameters \p x and \p y.
		Point(T x, T y);

		//! Constructs point with initializer list.
		template <typename U>
		Point(const std::initializer_list<U>& list);

		//! Copy constructor.
		Point(const Point& v);

		// MARK: Basic setters
		//! Set both x and y components to \p s.
		void Set(T s);

		//! Set x and y components with given parameters.
		void Set(T x, T y);

		//! Set x and y components with given initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Set x and y with other point \p pt.
		void Set(const Point& pt);

		//! Set both x and y to zero.
		void SetZero();

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + (v, v).
		Point Add(T v) const;

		//! Computes this + (v.x, v.y).
		Point Add(const Point& v) const;

		//! Computes this - (v, v).
		Point Sub(T v) const;

		//! Computes this - (v.x, v.y).
		Point Sub(const Point& v) const;

		//! Computes this * (v, v).
		Point Mul(T v) const;

		//! Computes this * (v.x, v.y).
		Point Mul(const Point& v) const;

		//! Computes this / (v, v).
		Point Div(T v) const;

		//! Computes this / (v.x, v.y).
		Point Div(const Point& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (v, v) - this.
		Point RSub(T v) const;

		//! Computes (v.x, v.y) - this.
		Point RSub(const Point& v) const;

		//! Computes (v, v) / this.
		Point RDiv(T v) const;

		//! Computes (v.x, v.y) / this.
		Point RDiv(const Point& v) const;

		// MARK: Augmented operations: this (+)= v
		//! Computes this += (v, v).
		void IAdd(T v);

		//! Computes this += (v.x, v.y).
		void IAdd(const Point& v);

		//! Computes this -= (v, v).
		void ISub(T v);

		//! Computes this -= (v.x, v.y).
		void ISub(const Point& v);

		//! Computes this *= (v, v).
		void IMul(T v);

		//! Computes this *= (v.x, v.y).
		void IMul(const Point& v);

		//! Computes this /= (v, v).
		void IDiv(T v);

		//! Computes this /= (v.x, v.y).
		void IDiv(const Point& v);

		// MARK: Basic getters
		//! Returns const reference to the \p i -th element of the point.
		const T& At(size_t i) const;

		//! Returns reference to the \p i -th element of the point.
		T& At(size_t i);

		//! Returns the sum of all the components (i.e. x + y).
		T Sum() const;

		//! Returns the minimum value among x and y.
		T Min() const;

		//! Returns the maximum value among x and y.
		T Max() const;

		//! Returns the absolute minimum value among x and y.
		T AbsMin() const;

		//! Returns the absolute maximum value among x and y.
		T AbsMax() const;

		//! Returns the index of the dominant axis.
		size_t DominantAxis() const;

		//! Returns the index of the subdominant axis.
		size_t SubdominantAxis() const;

		//! Returns a point with different value type.
		template <typename U>
		Point<U, 2> CastTo() const;

		//! Returns true if \p other is the same as this point.
		bool IsEqual(const Point& other) const;

		// MARK: Operators
		//! Returns reference to the \p i -th element of the point.
		T& operator[](size_t i);

		//! Returns const reference to the \p i -th element of the point.
		const T& operator[](size_t i) const;

		//! Set x and y components with given initializer list.
		Point& operator=(const std::initializer_list<T>& list);

		//! Set x and y with other point \p pt.
		Point& operator=(const Point& v);

		//! Computes this += (v, v)
		Point& operator+=(T v);

		//! Computes this += (v.x, v.y)
		Point& operator+=(const Point& v);

		//! Computes this -= (v, v)
		Point& operator-=(T v);

		//! Computes this -= (v.x, v.y)
		Point& operator-=(const Point& v);

		//! Computes this *= (v, v)
		Point& operator*=(T v);

		//! Computes this *= (v.x, v.y)
		Point& operator*=(const Point& v);

		//! Computes this /= (v, v)
		Point& operator/=(T v);

		//! Computes this /= (v.x, v.y)
		Point& operator/=(const Point& v);

		//! Returns true if \p other is the same as this point.
		bool operator==(const Point& v) const;

		//! Returns true if \p other is the not same as this point.
		bool operator!=(const Point& v) const;
	};
}

#endif