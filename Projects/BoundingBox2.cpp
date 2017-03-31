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
		return !(other.point1.x > point2.x || other.point2.x < point1.x || other.point2.y > point1.y || other.point1.y < point1.y);
	}

	template <typename T>
	bool BoundingBox<T, 2>::Contains(const Vector2<T>& point) const
	{
		return (point1.x < point.x && point.x < point2.x && point1.y < point.y && point.y < point2.y);
	}

	template <typename T>
	bool BoundingBox<T, 2>::Intersects(const Ray2<T>& ray) const
	{
		double tx1 = (lowerCorner.x-ray.origin.x) * 
	}

	template <typename T>
	Vector2<T> BoundingBox::MidPoint() const
	{
		return VectorType(point1.x + point2.x, point1.y + point2.y) / 2;
	}

	template <typename T>
	T BoundingBox::DiagonalLength() const
	{
		return point1.DistanceTo(point2);
	}

	template <typename T>
	T BoundingBox::DiagonalLengthSquared() const
	{
		return point1.DistanceSquaredTo(point2);
	}

	template <typename T>
	void BoundingBox::Reset()
	{
		point1.x(-_LIMITS_(T));
		point1.y(-_LIMITS_(T));
		point2.x(_LIMITS_(T));
		point2.y(_LIMITS_(T));
	}

	template <typename T>
	void BoundingBox<T, 2>::Merge(const Vector2<T>& point)
	{
		point1.x(std::min(point1.x, point.x));
		point1.y(std::min(point1.y, point.y));
		point2.x(std::max(point2.x, point.x));
		point2.y(std::max(point2.y, point.y));
	}

	template <typename T, size_t N>
	void BoundingBox::Merge(const BoundingBox& other)
	{
		Merge(other.point1);
		Merge(other.point2);
	}

	template <typename T, size_t N>
	void BoundingBox::Expand(T delta)
	{
		point1.x += 2 * delta;
		point1.y += 2 * delta;
		point2.x += 2 * delta;
		point2.y += 2 * delta;
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
