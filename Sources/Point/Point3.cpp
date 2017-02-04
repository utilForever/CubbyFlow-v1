/*************************************************************************
> File Name: Point3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D point class.
> Created Time: 2017/02/03
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Point/Point2.h>
#include <Point/Point3.h>

#include <cassert>

namespace CubbyFlow
{
	// Constructors
	template <typename T>
	Point<T, 3>::Point() :
		x(0), y(0), z(0)
	{
		// Do nothing
	}

	template <typename T>
	Point<T, 3>::Point(T newX, T newY, T newZ) :
		x(newX), y(newY), z(newZ)
	{
		// Do nothing
	}

	template <typename T>
	Point<T, 3>::Point(const Point2<T>& pt, T newZ) :
		x(pt.x), y(pt.y), z(newZ)
	{
		// Do nothing
	}

	template <typename T>
	template <typename U>
	Point<T, 3>::Point(const std::initializer_list<U>& list)
	{
		Set(list);
	}

	template <typename T>
	Point<T, 3>::Point(const Point& v) :
		x(v.x), y(v.y), z(v.z)
	{
		// Do nothing
	}

	// Basic setters
	template <typename T>
	void Point<T, 3>::Set(T s)
	{
		x = s;
		y = s;
		z = s;
	}

	template <typename T>
	void Point<T, 3>::Set(T newX, T newY, T newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	template <typename T>
	void Point<T, 3>::Set(const Point2<T>& pt, T newZ)
	{
		x = pt.x;
		y = pt.y;
		z = newZ;
	}

	template <typename T>
	template <typename U>
	void Point<T, 3>::Set(const std::initializer_list<U>& list)
	{
		assert(list.size() >= 3);

		auto inputElem = list.begin();
		x = static_cast<T>(*inputElem);
		y = static_cast<T>(*(++inputElem));
		z = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Point<T, 3>::Set(const Point& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	template <typename T>
	void Point<T, 3>::SetZero()
	{
		x = y = z = 0;
	}

	// Binary operators: new instance = this (+) v
	template <typename T>
	Point<T, 3> Point<T, 3>::Add(T v) const
	{
		return Point(x + v, y + v, z + v);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Add(const Point& v) const
	{
		return Point(x + v.x, y + v.y, z + v.z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Sub(T v) const
	{
		return Point(x - v, y - v, z - v);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Sub(const Point& v) const
	{
		return Point(x - v.x, y - v.y, z - v.z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Mul(T v) const
	{
		return Point(x * v, y * v, z * v);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Mul(const Point& v) const
	{
		return Point(x * v.x, y * v.y, z * v.z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Div(T v) const
	{
		return Point(x / v, y / v, z / v);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::Div(const Point& v) const
	{
		return Point(x / v.x, y / v.y, z / v.z);
	}

	// Binary operators: new instance = v (+) this
	template <typename T>
	Point<T, 3> Point<T, 3>::RSub(T v) const
	{
		return Point(v - x, v - y, v - z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::RSub(const Point& v) const
	{
		return Point(v.x - x, v.y - y, v.z - z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::RDiv(T v) const
	{
		return Point(v / x, v / y, v / z);
	}

	template <typename T>
	Point<T, 3> Point<T, 3>::RDiv(const Point& v) const
	{
		return Point(v.x / x, v.y / y, v.z / z);
	}

	// Augmented operators: this (+)= v
	template <typename T>
	void Point<T, 3>::IAdd(T v) {
		x += v;
		y += v;
		z += v;
	}

	template <typename T>
	void Point<T, 3>::IAdd(const Point& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	template <typename T>
	void Point<T, 3>::ISub(T v)
	{
		x -= v;
		y -= v;
		z -= v;
	}

	template <typename T>
	void Point<T, 3>::ISub(const Point& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	template <typename T>
	void Point<T, 3>::IMul(T v)
	{
		x *= v;
		y *= v;
		z *= v;
	}

	template <typename T>
	void Point<T, 3>::IMul(const Point& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	template <typename T>
	void Point<T, 3>::IDiv(T v)
	{
		x /= v;
		y /= v;
		z /= v;
	}

	template <typename T>
	void Point<T, 3>::IDiv(const Point& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}
}