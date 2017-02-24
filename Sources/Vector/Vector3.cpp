/*************************************************************************
> File Name: Vector3.cpp
> Project Name: CubbyFlow
> Author: Dongmin KIm
> Purpose: Generic N-D vector class.
> Created Time: 2017/02/22
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Vector\Vector3.h>

namespace CubbyFlow
{
	template <typename T>
	Vector<T, 3>::Vector()
	{
		// Do nothing
	}

	template <typename T>
	Vector<T, 3>::Vector(T x, T y, T z)
	{
		Set(x, y, z);
	}

	template <typename T>
	Vector<T, 3>::Vector(const Vector2<T>& pt, T z)
	{
		Set(pt.x, pt.y, z);
	}

	template <typename T>
	template <typename U>
	Vector<T, 3>::Vector(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T>
	Vector<T, 3>::Vector(const Vector& v)
	{
		Set(v);
	}

	template <typename T>
	void Vector<T, 3>::Set(T s)
	{
		Set(s, s, s);
	}

	template <typename T>
	void Vector<T, 3>::Set(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template <typename T>
	void Vector<T, 3>::Set(const Vector2<T>& pt, T z)
	{
		Set(pt.x, pt.y, z);
	}

	template <typename T>
	template <typename U>
	void Vector<T, 3>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= 3);

		auto inputElem = list.begin();
		x = static_cast<T>(*inputElem);
		y = static_cast<T>(*(++inputElem));
		z = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Vector<T, 3>::Set(const Vector& v)
	{
		Set(v.x, v.y, v.z);
	}

	template <typename T>
	void Vector<T, 3>::SetZero()
	{
		Set(0, 0, 0);
	}

	template <typename T>
	void Vector<T, 3>::Normalize()
	{
		IDiv(Length());
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Add(T v) const
	{
		return Vector<T, 3>(x + v, y + v, z + v);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Add(const Vector& v) const
	{
		return Vector<T, 3>(x + v.x, y + v.y, z + v.z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Sub(T v) const
	{
		return Vector<T, 3>(x - v, y - v, z - v);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Sub(const Vector& v) const
	{
		return Vector<T, 3>(x - v.x, y - v.y, z - v.z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Mul(T v) const
	{
		return Vector<T, 3>(x * v, y * v, z * v);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Mul(const Vector& v) const
	{
		return Vector<T, 3>(x * v.x, y * v.y, z * v.z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Div(T v) const
	{
		return Vector<T, 3>(x / v, y / v, z / v);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Div(const Vector& v) const
	{
		return Vector<T, 3>(x / v.x, y / v.y, z / v.z);
	}

	template <typename T>
	T Vector<T, 3>::Dot(const Vector& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Cross(const Vector& v) const
	{
		return Vector<T, 3>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::RSub(T v) const
	{
		return Vector<T, 3>(v - x, v - y, v - z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::RSub(const Vector& v) const
	{
		return Vector<T, 3>(v.x - x, v.y - y, v.z - z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::RDiv(T v) const
	{
		return Vector<T, 3>(v / x, v / y, v / z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::RDiv(const Vector& v) const
	{
		return Vector<T, 3>(v.x / x, v.y / y, v.z / z);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::RCross(const Vector& v) const
	{
		return (v.y * z - v.z * y, v.z * x - v.x * z, v.x * y - v.y * x);
	}

	template <typename T>
	void Vector<T, 3>::IAdd(T v)
	{
		x += v;
		y += v;
		z += v;
	}

	template <typename T>
	void Vector<T, 3>::IAdd(const Vector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	template <typename T>
	void Vector<T, 3>::ISub(T v)
	{
		x -= v;
		y -= v;
		z -= v;
	}

	template <typename T>
	void Vector<T, 3>::ISub(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	template <typename T>
	void Vector<T, 3>::IMul(T v)
	{
		x *= v;
		y *= v;
		z *= v;
	}

	template <typename T>
	void Vector<T, 3>::IMul(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	template <typename T>
	void Vector<T, 3>::IDiv(T v)
	{
		x /= v;
		y /= v;
		z /= v;
	}

	template <typename T>
	void Vector<T, 3>::IDiv(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	template <typename T>
	const T& Vector<T, 3>::At(size_t i) const
	{
		assert(i < 3);
		return &x[i];
	}

	template <typename T>
	T& Vector<T, 3>::At(size_t i)
	{
		return At(i);
	}

	template <typename T>
	T Vector<T, 3>::Sum() const
	{
		return x + y + z;
	}

	template <typename T>
	T Vector<T, 3>::Avg() const
	{
		return Sum() / 3.0;
	}

	template <typename T>
	T Vector<T, 3>::Min() const
	{
		return min(x, y, z);
	}

	template <typename T>
	T Vector<T, 3>::Max() const
	{
		return max(x, y, z);
	}

	template <typename T>
	T Vector<T, 3>::AbsMin() const
	{
		return min(abs(x), abs(y), abs(z));
	}

	template <typename T>
	T Vector<T, 3>::AbsMax() const
	{
		return max(abs(x), abs(y), abs(z));
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Normalized() const
	{
		return Vector<T, 3>(x / Length(), y / Length(), z / Length());
	}

	template <typename T>
	T Vector<T, 3>::Length() const
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	template <typename T>
	T Vector<T, 3>::LengthSquared() const
	{
		return pow(Length(), 2);
	}


	template <typename T>
	T Vector<T, 3>::DistanceTo(const Vector& other) const
	{
		return (*this - other).Length();
	}

	template <typename T>
	T Vector<T, 3>::DistanceSquaredTo(const Vector& other) const
	{
		return pow(DistanceTo(other), 2);
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Reflected(const Vector& normal) const
	{
		return (*this) - Cross(Dot(normal)) * 2;
	}

	template <typename T>
	Vector<T, 3> Vector<T, 3>::Projected(const Vector& normal) const
	{
		return normal / (Dot(normal) / normal.LengthSquared());
	}

	template <typename T>
	std::tuple<Vector<T, 3>, Vector<T, 3>> Tangential() const
	{

	}

	template <typename T>
	template <typename U>
	Vector<U, 3> Vector<T, 3>::CastTo() const
	{
		return Vector<U, 3>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
	}

	template <typename T>
	bool Vector<T, 3>::IsEqual(const Vector& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	template <typename T>
	bool Vector<T, 3>::IsSimilar(const Vector& other, T epsilon = std::numeric_limits<T>::epsilon()) const
	{
		return (abs(x - other.x) < epsilon() && abs(y - other.y) < epsilon() && abs(z - other.z) < epsilon());
	}

	template <typename T>
	T& Vector<T, 3>::operator[](size_t i)
	{
		return At(i);
	}

	template <typename T>
	const T& Vector<T, 3>::operator[](size_t i) const
	{
		return At(i);
	}

	template <typename T>
	template <typename U>
	Vector<T, 3>& Vector<T, 3>::operator=(const std::initializer_list<U>& list)
	{
		return Set(list);
	}

	template <typename T>
	Vector<T, 3>& operator=(const Vector& v)
	{
		return Set(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator+=(T v)
	{
		return Add(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator+=(const Vector& v)
	{
		return Add(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator-=(T v)
	{
		return Sub(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator-=(const Vector& v)
	{
		return Sub(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator*=(T v)
	{
		return Mul(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator*=(const Vector& v)
	{
		return Mul(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator/=(T v)
	{
		return Div(v);
	}

	template <typename T>
	Vector<T, 3>& Vector<T, 3>::operator/=(const Vector& v)
	{
		return Div(v);
	}

	template <typename T>
	bool Vector<T, 3>::operator==(const Vector& v) const
	{
		return IsEqual(v);
	}

	template <typename T>
	bool Vector<T, 3>::operator!=(const Vector& v) const
	{
		return !IsEqual(v);
	}


	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& a)
	{
		return a;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& a)
	{
		return Vector<T, 3>(-a.x,-a.y,-a.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(T a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a + b.x, a + b.y, a + b.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template <typename T>
	Vector3<T> Vector<T, 3>::operator-(const Vector3<T>& a, T b)
	{
		return Vector<T, 3>(a.x - b, a.y - b, a.z - b);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(T a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a - b.x, a - b.y, a - b.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(const Vector3<T>& a, T b)
	{
		return Vector<T, 3>(a.x * b, a.y * b, a.z * b);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(T a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a * b.x, a * b.y, a * b.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector<T, 3>(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator/(const Vector3<T>& a, T b)
	{
		return Vector<T, 3>(a.x / b, a.y / b, a.z / b);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator/(T a, const Vector3<T>& b)
	{
		return Vector<T, 3>(b.x / a, b.y / a, b.z / a);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator/(const Vector3<T>& a, const Vector3<T>& b)
	{
		return a / b;
	}

	template <typename T>
	Vector3<T> Vector3<T>::Min(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector<T, 3>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Max(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector<T, 3>(max(a.x, b, x), max(a.y, b.y), max(a.z, b.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Clamp(const Vector3<T>& v, const Vector3<T>& low, const Vector3<T>& high)
	{
		return Min(Max(v, low), high);
	}

	template <typename T>
	Vector3<T> Vector3<T>::Ceil(const Vector3<T>& a)
	{
		return Vector<T, 3>(ceil(a.x), ceil(a.y), ceil(a.z));
	}

	template <typename T>
	Vector3<T> Vector3<T>::Floor(const Vector3<T>& a)
	{
		return Vector<T, 3>(floor(a.x), floor(a.y), floor(a.z));
	}
}