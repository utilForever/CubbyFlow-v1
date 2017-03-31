/*************************************************************************
> File Name: BoundingBox3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D axis-aligned bounding box class.
> Created Time: 2017/04/01
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <BoundingBox\BoundingBox3.h>

namespace CubbyFlow
{
	template <typename T>
	BoundingBox<T, 3>::BoundingBox()
	{
		// Do nothing
	}

	template <typename T>
	BoundingBox<T, 3>::BoundingBox(const Vector3<T>& point1, const Vector3<T>& point2)
	{
		lowerCorner(point1);
		upperCorner(point2);
	}

	template <typename T>
	BoundingBox<T, 3>::BoundingBox(const BoundingBox& other)
	{
		lowerCorner(other.lowerCorner);
		upperCorner(other.upperCorner);
	}

	template <typename T>
	BoundingBox<T, 3>::Width() const
	{
		return upperCorner.x - lowerCorner.x;
	}

	template <typename T>
	BoundingBox<T, 3>::Height() const
	{
		return upperCorner.y - lowerCorner.y;
	}

	template <typename T>
	BoundingBox<T, 3>::Depth() const
	{
		return upperCorner.z - lowerCorner.z;
	}

	template <typename T>
	BoundingBox<T, 3>::Length(size_t axis)
	{
		switch (axis)
		{
		case 0:
			return Width();
		case 1:
			return Height();
		case 2:
			return Depth();
		}
	}

	template <typename T>
	bool BoundingBox<T, 3>::Overlaps(const BoundingBox& other) const
	{
	
	}

	template <typename T>
	bool BoundingBox<T, 3>::Contains(const Vector3<T>& point) const
	{
		return (lowerCorner.x < point.x && point.x < upperCorner.y &&
			lowerCorner.y < point.y && point.y < upperCorner.y &&
			lowerCorner.z < point.z && point.z < upperCorner.z &&)
	}

	template <typename T>
	bool BoundingBox<T, 3>::Intersects(const Ray3<T>& ray) const
	{

	}

	template <typename T>
	Vector3<T> BoundingBox<T, 3>::MidPoint() const
	{
		return (lowerCorner + upperCorner) / 2;
	}

	template <typename T>
	T BoundingBox<T, 3>::DiagonalLength() const
	{
		return lowerCorner.DistanceTo(upperCorner);
	}

	template <typename T>
	T BoundingBox<T, 3>::DiagonalLengthSquared() const
	{
		return lowerCorner.DistanceSquaredTo(upperCorner);
	}

	template <typename T>
	void BoundingBox<T, 3>::Reset()
	{
		lowerCorner(-_LIMITS_(T));
		upperCorner(_LIMITS_(T));
	}
}