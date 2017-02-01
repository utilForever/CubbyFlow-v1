/*************************************************************************
> File Name: Point3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D point class.
> Created Time: 2017/02/01
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT3_H
#define CUBBYFLOW_POINT3_H

#include <Point/Point.h>

#include <Utils/Constants.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D point class.
	//!
	//! This class defines simple 3-D point data.
	//!
	//! \tparam T - Type of the element
	//!
	template <typename T>
	class Point<T, 3> final
	{
	public:
		static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmatic types");

		//! X (or the first) component of the point.
		T x;

		//! Y (or the second) component of the point.
		T y;

		//! Z (or the third) component of the point.
		T z;

		// MARK: Constructors
		//! Constructs default point (0, 0, 0).
		Point();

		//! Constructs point with given parameters \p x, \p y, and \p z.
		Point(T x, T y, T z);

		//! Constructs point with a 2-D point and a scalar.
		Point(const Point2<T>& pt, T z);

		//! Constructs point with initializer list.
		template <typename U>
		Point(const std::initializer_list<U>& list);

		//! Copy constructor.
		Point(const Point& v);

		// MARK: Basic setters
		//! Set all x, y, and z components to \p s.
		void Set(T s);
		
		//! Set x, y, and z components with given parameters.
		void Set(T x, T y, T z);

		//! Set x, y, and z components with given \p pt.x, \p pt.y, and \p z.
		void Set(const Point2<T>& pt, T z);

		//! Set x, y, and z components with given initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Set x, y, and z with other point \p pt.
		void Set(const Point& v);

		//! Set both x, y, and z to zero.
		void SetZero();

		// MARK: Binary operations: new instance = this (+) v
		//! Computes this + (v, v, v).
		Point Add(T v) const;

		//! Computes this + (v.x, v.y, v.z).
		Point Add(const Point& v) const;

		//! Computes this - (v, v, v).
		Point Sub(T v) const;

		//! Computes this - (v.x, v.y, v.z).
		Point Sub(const Point& v) const;

		//! Computes this * (v, v, v).
		Point Mul(T v) const;

		//! Computes this * (v.x, v.y, v.z).
		Point Mul(const Point& v) const;

		//! Computes this / (v, v, v).
		Point Div(T v) const;

		//! Computes this / (v.x, v.y, v.z).
		Point Div(const Point& v) const;

		// MARK: Binary operations: new instance = v (+) this
		//! Computes (v, v, v) - this.
		Point RSub(T v) const;

		//! Computes (v.x, v.y, v.z) - this.
		Point RSub(const Point& v) const;

		//! Computes (v, v, v) / this.
		Point RDiv(T v) const;

		//! Computes (v.x, v.y, v.z) / this.
		Point RDiv(const Point& v) const;

		// MARK: Augmented operations: this (+)= v
		//! Computes this += (v, v, v).
		void IAdd(T v);

		//! Computes this += (v.x, v.y, v.z).
		void IAdd(const Point& v);

		//! Computes this -= (v, v, v).
		void ISub(T v);

		//! Computes this -= (v.x, v.y, v.z).
		void ISub(const Point& v);

		//! Computes this *= (v, v, v).
		void IMul(T v);

		//! Computes this *= (v.x, v.y, v.z).
		void IMul(const Point& v);

		//! Computes this /= (v, v, v).
		void IDiv(T v);

		//! Computes this /= (v.x, v.y, v.z).
		void IDiv(const Point& v);

		// MARK: Basic getters
		//! Returns const reference to the \p i -th element of the point.
		const T& At(size_t i) const;

		//! Returns reference to the \p i -th element of the point.
		T& At(size_t i);

		//! Returns the sum of all the components (i.e. x + y).
		T Sum() const;

		//! Returns the minimum value among x, y, and z.
		T Min() const;

		//! Returns the maximum value among x, y, and z.
		T Max() const;

		//! Returns the absolute minimum value among x, y, and z.
		T AbsMin() const;

		//! Returns the absolute maximum value among x, y, and z.
		T AbsMax() const;

		//! Returns the index of the dominant axis.
		size_t DominantAxis() const;

		//! Returns the index of the subdominant axis.
		size_t SubdominantAxis() const;

		//! Returns a point with different value type.
		template <typename U>
		Point<U, 3> CastTo() const;

		//! Returns true if \p other is the same as this point.
		bool IsEqual(const Point& other) const;

		// MARK: Operators
		//! Returns reference to the \p i -th element of the point.
		T& operator[](size_t i);

		//! Returns const reference to the \p i -th element of the point.
		const T& operator[](size_t i) const;

		//! Set x, y, and z components with given initializer list.
		Point& operator=(const std::initializer_list<T>& list);

		//! Set x, y, and z with other point \p pt.
		Point& operator=(const Point& v);

		//! Computes this += (v, v, v)
		Point& operator+=(T v);

		//! Computes this += (v.x, v.y, v.z)
		Point& operator+=(const Point& v);

		//! Computes this -= (v, v, v)
		Point& operator-=(T v);

		//! Computes this -= (v.x, v.y, v.z)
		Point& operator-=(const Point& v);

		//! Computes this *= (v, v, v)
		Point& operator*=(T v);

		//! Computes this *= (v.x, v.y, v.z)
		Point& operator*=(const Point& v);

		//! Computes this /= (v, v, v)
		Point& operator/=(T v);

		//! Computes this /= (v.x, v.y, v.z)
		Point& operator/=(const Point& v);

		//! Returns true if \p other is the same as this point.
		bool operator==(const Point& v) const;

		//! Returns true if \p other is the not same as this point.
		bool operator!=(const Point& v) const;
	};

	//! Type alias for three dimensional point.
	template <typename T> using Point3 = Point<T, 3>;

	//! Positive sign operator.
	template <typename T>
	Point<T, 3> operator+(const Point<T, 3>& a);

	//! Negative sign operator.
	template <typename T>
	Point3<T> operator-(const Point3<T>& a);

	//! Computes (a, a, a) + (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator+(T a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator+(const Point3<T>& a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) - (b, b, b).
	template <typename T>
	Point3<T> operator-(const Point3<T>& a, T b);

	//! Computes (a, a, a) - (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator-(T a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator-(const Point3<T>& a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) * (b, b, b).
	template <typename T>
	Point3<T> operator*(const Point3<T>& a, T b);

	//! Computes (a, a, a) * (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator*(T a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator*(const Point3<T>& a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) / (b, b, b).
	template <typename T>
	Point3<T> operator/(const Point3<T>& a, T b);

	//! Computes (a, a, a) / (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator/(T a, const Point3<T>& b);

	//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
	template <typename T>
	Point3<T> operator/(const Point3<T>& a, const Point3<T>& b);

	//! Returns element-wise min vector.
	template <typename T>
	Point3<T> Min(const Point3<T>& a, const Point3<T>& b);

	//! Returns element-wise max vector.
	template <typename T>
	Point3<T> Max(const Point3<T>& a, const Point3<T>& b);

	//! Returns element-wise clamped vector.
	template <typename T>
	Point3<T> Clamp(const Point3<T>& v, const Point3<T>& low, const Point3<T>& high);

	//! Returns element-wise ceiled vector.
	template <typename T>
	Point3<T> Ceil(const Point3<T>& a);

	//! Returns element-wise floored vector.
	template <typename T>
	Point3<T> Floor(const Point3<T>& a);

	//! Float-type 3D point.
	using Point3F = Point3<float>;

	//! Double-type 3D point.
	using Point3D = Point3<double>;

	//! Integer-type 3D point.
	using Point3I = Point3<ssize_t>;

	//! Unsigned integer-type 3D point.
	using Point3UI = Point3<size_t>;
}

#endif