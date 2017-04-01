/*************************************************************************
> File Name: Ray3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Class for 3-D ray.
> Created Time: 2017/03/25
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Ray/Ray3.h>

namespace CubbyFlow
{
	template <typename T>
	Ray<T, 3>::Ray() :
		Ray(Vector3<T>(), Vector3<T>(1, 0, 0))
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 3>::Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection) :
		origin(newOrigin), direction(newDirection.Normalized())
	{
		// Do nothing
	}

	template <typename T>
	Ray<T, 3>::Ray(const Ray& other) :
		origin(other.origin), direction(other.direction)
	{
		// Do nothing
	}

	template <typename T>
	Vector3<T> Ray<T, 3>::PointAt(T t) const
	{
		return origin + t * direction;
	}
}