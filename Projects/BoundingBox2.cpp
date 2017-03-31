/*************************************************************************
> File Name: BoundingBox2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 2-D axis-aligned bounding box class.
> Created Time: 2017/03/31
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <BoundingBox\BoundingBox2.h>

namespace CubbyFlow
{
	template <typename T>
	BoundingBox<T, 2>::BoundingBox()
	{
		// Do nothing
	}

	template <typename T>
	BoundingBox<T, 2>::BoundingBox(const Vector2<T>& point1, const Vector2<T>& point2)
	{
		lowerCorner(point1);
		upperCorner(point2);
	}

	template <typename T>
	BoundingBox<T, 2>::BoundingBox(const BoundingBox& other)
	{
		lowerCorner(other.lowerCorner);
		upperCorner(other.upperCorner);
	}

	template <typename T>
	T BoundingBox<T, 2>::Width() const
	{
		return upperCorner.x - lowerCorner.x;
	}

	template <typename T>
	T BoundingBox<T, 2>::Height() const
	{
		return upperCorner.y - lowerCorner.y;
	}

	template <typename T>
	T BoundingBox<T, 2>::Length(size_t axis)
	{
		if (axis == 0) return Width();
		return Height();
	}

	template <typename T>
	bool BoundingBox<T, 2>::Overlaps(const BoundingBox& other) const
	{
		return !(other.lowerCorner.x > upperCorner.x || other.upperCorner.x < lowerCorner.x || other.upperCorner.y > lowerCorner.y || other.lowerCorner.y < lowerCorner.y);
	}

	template <typename T>
	bool BoundingBox<T, 2>::Contains(const Vector2<T>& point) const
	{
		return (lowerCorner.x < point.x && point.x < upperCorner.x && lowerCorner.y < point.y && point.y < upperCorner.y);
	}

	template <typename T>
	bool BoundingBox<T, 2>::Intersects(const Ray2<T>& ray) const
	{
	}

	template <typename T>
	Vector2<T> BoundingBox<T, 2>::MidPoint() const
	{
		return (lowerCorner + upperCorner) / 2;
	}

	template <typename T>
	T BoundingBox<T, 2>::DiagonalLength() const
	{
		return lowerCorner.DistanceTo(upperCorner);
	}

	template <typename T>
	T BoundingBox<T, 2>::DiagonalLengthSquared() const
	{
		return lowerCorner.DistanceSquaredTo(upperCorner);
	}

	template <typename T>
	void BoundingBox<T, 2>::Reset()
	{
		lowerCorner(-_LIMITS_(T));
		upperCorner(_LIMITS_(T));
	}

	template <typename T>
	void BoundingBox<T, 2>::Merge(const Vector2<T>& point)
	{
		point1.x(std::min(lowerCorner.x, point.x));
		point1.y(std::min(lowerCorner.y, point.y));
		point2.x(std::max(upperCorner.x, point.x));
		point2.y(std::max(upperCorner.y, point.y));
	}

	template <typename T>
	void BoundingBox<T, 2>::Merge(const BoundingBox& other)
	{
		Merge(other.lowerCorner);
		Merge(other.upperCorner);
	}

	template <typename T>
	void BoundingBox<T, 2>::Expand(T delta)
	{
		lowerCorner += delta * 2;
		upperCorner += delta * 2;
	}

	template <typename T>
	Vector2<T> BoundingBox<T, 2>::Corner(size_t idx) const
	{
		switch (idx)
		{
		case 0:
			return lowerCorner;
		case 1:
			return Vector2<T>(lowerCorner.x, upperCorner.y);
		case 2:
			return Vector2<T>(uppercorner.x, lowerCorner.y);
		case 3:
			return upperCorner;
		}
	}
}
