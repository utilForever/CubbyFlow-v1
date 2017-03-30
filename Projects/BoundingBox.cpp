/*************************************************************************
> File Name: BoundingBox.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Generic N-D axis-aligned bounding box class.
> Created Time: 2017/03/30
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <BoundingBox\BoundingBox.h>

namespace CubbyFlow
{
	template <typename T, size_t N>
	BoundingBox::BoundingBox()
	{

	}

	template <typename T, size_t N>
	BoundingBox::BoundingBox(const VectorType& point1, const VectorType& point2)
	{
		lowerCorner = point1;
		upperCorner = point2;
	}

	template <typename T, size_t N>
	BoundingBox::BoundingBox(const BoundingBox& other)
	{
		lowerCorner(other.lowerCorner);
		upperCorner(other.upperCorner);
	}

	template <typename T, size_t N>
	bool BoundingBox::Overlaps(const BoundingBox& other) const
	{
		return !(other.point1.x > point2.x || other.point2.x < point1.x || other.point2.y > point1.y || other.point1.y < point1.y);
	}

	template <typename T, size_t N>
	bool BoundingBox::Contains(const VectorType& point) const
	{
		return (point1.x < point.x && point.x < point2.x && point1.y < point.y && point.y < point2.y);
	}

	template <typename T, size_t N>
	VectorType BoundingBox::MidPoint() const
	{
		return VectorType(point1.x + point2.x, point1.y + point2.y) / 2;
	}

	template <typename T, size_t N>
	T BoundingBox::DiagonalLength() const
	{
		return point1.DistanceTo(point2);
	}

	template <typename T, size_t N>
	T BoundingBox::DiagonalLengthSquared() const
	{
		return point1.DistanceSquaredTo(point2);
	}

	template <typename T, size_t N>
	void BoundingBox::Reset()
	{
		point1.x(-_LIMITS_(T));
		point1.y(-_LIMITS_(T));
		point2.x(_LIMITS_(T));
		point2.y(_LIMITS_(T));
	}

	template <typename T, size_t N>
	void BoundingBox::Merge(const VectorType& point)
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
}
