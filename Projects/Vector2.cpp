/*************************************************************************
> File Name: Vector2.cpp
> Project Name: CubbyFlow
> Author: Dongmin KIm
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/21
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Vector\Vector2.h>

namespace CubbyFlow
{
	template <typename T>
	Vector<T, 2>::Vector()
	{
		// Do nothing
	}

	template <typename T>
	Vector<T, 2>::Vector(T x, T y)
	{
		Set(x, y);
	}

	template <typename T>
	template <typename U>
	Vector<T, 2>::Vector(const std::initializer_list<U>& list)
	{
		Set(list);
	}
	
	template <typename T>
	Vector<T, 2>::Vector(const Vector& v)
	{
		Set(v);
	}

	template <typename T>
	void Vector<T, 2>::Set(T s)
	{
		Set(s, s);
	}

	template <typename T>
	void Vector<T, 2>::Set(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	template <typename T>
	template <typename U>
	void Vector<T, 2>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= 2);

		auto inputElem = list.begin();
		x = static_cast<T>(*inputElem);
		y = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Vector<T, 2>::Set(const Vector& pt)
	{
		x = pt.x;
		y = pt.y;
	}

	template <typename T>
	void Vector<T, 2>::SetZero()
	{
		x = 0;
		y = 0;
	}

	template <typename T>
	void Vector<T, 2>::Normalize()
	{
		x /= Length();
		y /= Length();
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Add(T v) const
	{
		return Vector(x + v, y + v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Add(const Vector<T, 2>& v) const
	{
		return Vector(x + v.x, y + v.y);
	}


	template <typename T>
	Vector<T, 2> Vector<T, 2>::Sub(T v) const
	{
		return Vector(x - v, y - v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Sub(const Vector<T, 2>& v) const
	{
		return Vector(x - v.x, y - v.y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Mul(T v) const
	{
		return Vector(x * v, y * v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Mul(const Vector<T, 2>& v) const
	{
		return Vector(x * v.x, y * v.y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Div(T v) const
	{
		return Vector(x / v, y / v);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Div(const Vector<T, 2>& v) const
	{
		return Vector(x / v.x, y / v.y);
	}

	template <typename T>
	T Vector<T, 2>::Dot(const Vector<T, 2>& v) const
	{
		return x * v.x + y * v.y;
	}

	template <typename T>
	T Vector<T, 2>::Cross(const Vector<T, 2>& v) const
	{
		return x * v.y - y * v.x;
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RSub(T v) const
	{
		return Vector(v - x, v - y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RSub(const Vector& v) const
	{
		return Vector(v.x - x, v.y - y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RDiv(T v) const
	{
		return Vector(v / x, v / y);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::RDiv(const Vector& v) const
	{
		return Vector(v.x / x, v.y / y);
	}

	template <typename T>
	T Vector<T, 2>::RCross(const Vector& v) const
	{
		return v.x * y - v.y * x;
	}

	template <typename T>
	void Vector<T, 2>::IAdd(T v)
	{
		x += v;
		y += v;
	}

	template <typename T>
	void Vector<T, 2>::IAdd(const Vector<T, 2>& v)
	{
		x += v.x;
		y += v.y;
	}

	template <typename T>
	void Vector<T, 2>::ISub(T v)
	{
		x -= v;
		y -= v;
	}

	template <typename T>
	void Vector<T, 2>::ISub(const Vector<T, 2>& v)
	{
		x -= v.x;
		y -= v.y;
	}

	template <typename T>
	void Vector<T, 2>::IMul(T v)
	{
		x *= v;
		y *= v;
	}

	template <typename T>
	void Vector<T, 2>::IMul(const Vector<T, 2>& v)
	{
		x *= v.x;
		y *= v.y;
	}

	template <typename T>
	void Vector<T, 2>::IDiv(T v)
	{
		x /= v;
		y /= v;
	}

	template <typename T>
	void Vector<T, 2>::IDiv(const Vector<T, 2>& v)
	{
		x /= v.x;
		y /= v.y;
	}

	template <typename T>
	T& Vector<T, 2>::At(size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}

	template <typename T>
	T Vector<T, 2>::Sum() const
	{
		return x + y;
	}

	template <typename T>
	T Vector<T, 2>::Avg() const
	{
		return (x + y)/2.0;
	}

	template <typename T>
	T Vector<T, 2>::Min() const
	{
		return min(x, y);
	}

	template <typename T>
	T Vector<T, 2>::Max() const
	{
		return max(x, y);
	}

	template <typename T>
	T Vector<T, 2>::AbsMin() const
	{
		return min(abs(x), abs(y));
	}

	template <typename T>
	T Vector<T, 2>::AbsMax() const
	{
		return max(abs(x), abs(y));
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Normalized() const
	{
		return Vector(x / Length(), y / Length());
	}

	template <typename T>
	T Vector<T, 2>::Length() const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	template <typename T>
	T Vector<T, 2>::LengthSquared() const
	{
		return pow(Length(), 2);
	}

	template <typename T>
	T DistanceTo(const Vector<T, 2>& other) const
	{
		return (*this - other).Length();
	}

	template <typename T>
	T DistanceSquaredTo(const Vector& other) const
	{
		return pow(DistanceTo(other), 2);
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Reflected(const Vector& normal) const
	{
		return (*this) - Cross(Dot(normal)) * 2;
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Projected(const Vector& normal) const
	{
		return normal / (Dot(normal) / normal.LengthSquared());
	}

	template <typename T>
	Vector<T, 2> Vector<T, 2>::Tangential() const
	{
		return Vector<T, 2>(-y, x);
	}

	template <typename T>
	template <typename U>
	Vector<U, 2> Vector<T, 2>::CastTo() const
	{
		return Vector<U, 2>(static_cast<U>(x), static_cast<U>(y));
	}

	template <typename T>
	bool Vector<T, 2>::IsEqual(const Vector<T, 2>& other) const
	{
		return (x == other.x && y == other.y);
	}

	template <typename T>
	bool Vector<T, 2>::IsSimilar(const Vector<T, 2>& other, T epsilon = std::numeric_limits<T>::epsilon()) const
	{
		return (abs(x - other.x) < epsilon() && abs(y - other.y) < epsilon());
	}

	template <typename T>
	T& Vector<T, 2>::operator[](size_t i)
	{
		return At(i);
	}

	template <typename T>
	const T& Vector<T, 2>::operator[](size_t i) const 
	{
		return At(i);
	}

	template <typename T>
	template <typename U>
	Vector<T, 2>& Vector<T, 2>::operator=(const std::initializer_list<U>& list)
	{
		return Set(list);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator+=(T v)
	{
		return Add(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator+=(const Vector& v)
	{
		return Add(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator-=(T v)
	{
		return Sub(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator-=(const Vector& v)
	{
		return Sub(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator*=(T v)
	{
		return Mul(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator*=(const Vector& v)
	{
		return Mul(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator/=(T v)
	{
		return Div(v);
	}

	template <typename T>
	Vector<T, 2>& Vector<T, 2>::operator/=(const Vector& v)
	{
		return Div(v);
	}

	template <typename T>
	bool Vector<T, 2>::operator==(const Vector<T, 2>& v) const
	{
		return IsEqual(v);
	}

	template <typename T>
	bool Vector<T, 2>::operator!=(const Vector<T, 2>& v) const
	{
		return !IsEqual(v);
	}

	template <typename T>
	Vector2<T> operator+(const Vector2<T>& a)
	{
		return a;
	}

	template <typename T>
	Vector2<T> operator-(const Vector2<T>& a)
	{
		return Vector<T, 2>(-a.x, -a.y);
	}

	template <typename T>
	Vector2<T> operator+(T a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a + b.x, a + b.y);
	}

	template <typename T>
	Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a.x + b.x, a.y + b.y);
	}

	template <typename T>
	Vector2<T> operator-(const Vector2<T>& a, T b)
	{
		return Vector<T, 2>(a.x - b, a.y - b);
	}

	template <typename T>
	Vector2<T> operator-(T a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a - b.x, a - b.y);
	}

	template <typename T>
	Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a.x - b.x, a.y - b.y);
	}

	template <typename T>
	Vector2<T> operator*(const Vector2<T>& a, T b)
	{
		return Vector<T, 2>(a.x * b, a.y * b);
	}

	template <typename T>
	Vector2<T> operator*(T a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a * b.x, a * b.y);
	}

	template <typename T>
	Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a.x * b.x, a.y * b.y);
	}

	template <typename T>
	Vector2<T> operator/(const Vector2<T>& a, T b)
	{
		return Vector<T, 2>(a.x / b, a.y / b);
	}

	template <typename T>
	Vector2<T> operator/(T a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a / b.x, a / b.y);
	}

	template <typename T>
	Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(a.x / b.x, a.y / b.y);
	}

	template <typename T>
	Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(min(a.x, b.x), (a.y, b.y));
	}

	template <typename T>
	Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector<T, 2>(max(a.x, b.x), max(a.y, b.y));
	}

	template <typename T>
	Vector2<T> Clamp(const Vector2<T>& v, const Vector2<T>& low, const Vector2<T>& high)
	{
		return Min(Max(v, low), high);
	}

	template <typename T>
	Vector2<T> Ceil(const Vector2<T>& a)
	{
		return Vector<T, 2>(ceil(a.x), ceil(a.y));
	}

	template <typename T>
	Vector2<T> Floor(const Vector2<T>& a)
	{
		return Vector<T, 2>(floor(a.x), floor(a.y));
	}

	template <typename T>
	Vector2<T> MonotonicCatmullRom(
		const Vector2<T>& v0,
		const Vector2<T>& v1,
		const Vector2<T>& v2,
		const Vector2<T>& v3,
		T f)
	{

	}
}