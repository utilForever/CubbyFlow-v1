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
	BoundingBox<T, N>::BoundingBox()
	{
		// Do nothing
	}

	template <typename T, size_t N>
	BoundingBox<T, N>::BoundingBox(const VectorType& point1, const VectorType& point2)
	{
		lowerCorner(point1);
		upperCorner(point2);
	}

	template <typename T, size_t N>
	BoundingBox<T, N>::BoundingBox(const BoundingBox& other)
	{
		lowerCorner(other.lowerCorner);
		upperCorner(other.upperCorner);
	}

	template <typename T, size_t N>
	bool BoundingBox<T, N>::Overlaps(const BoundingBox& other) const
	{
		return !(other.point1 > point2.x || other.point2 < point1);
	}

	template <typename T, size_t N>
	bool BoundingBox<T, N>::Contains(const VectorType& point) const
	{
		return (point1 < point && point < point2);
	}

	template <typename T, size_t N>
	VectorType BoundingBox<T, N>::MidPoint() const
	{
		return VectorType(point1 + point2) / 2;
	}

	template <typename T, size_t N>
	T BoundingBox<T, N>::DiagonalLength() const
	{
		return point1.DistanceTo(point2);
	}

	template <typename T, size_t N>
	T BoundingBox<T, N>::DiagonalLengthSquared() const
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
		point1(std::min(point1, point));
		point2(std::max(point2, point));
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
		point1 += 2 * delta;
		point2 += 2 * delta;
	}
}
