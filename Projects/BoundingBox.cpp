/*************************************************************************
> File Name: BoundingBox.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Generic N-D axis-aligned bounding box class.
> Created Time: 2017/03/30
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <BoundingBox/BoundingBox.h>

#include <Utils/MathUtils.h>

namespace CubbyFlow
{
	template <typename T, size_t N>
	BoundingBox<T, N>::BoundingBox()
	{
		Reset();
	}

	template <typename T, size_t N>
	BoundingBox<T, N>::BoundingBox(const VectorType& point1, const VectorType& point2)
	{
		for (size_t i = 0; i < N; ++i)
		{
			lowerCorner[i] = std::min(point1[i], point2[i]);
			upperCorner[i] = std::max(point1[i], point2[i]);
		}
	}

	template <typename T, size_t N>
	BoundingBox<T, N>::BoundingBox(const BoundingBox& other) :
		lowerCorner(other.lowerCorner), upperCorner(other.upperCorner)
	{
		// Do nothing
	}

	template <typename T, size_t N>
	bool BoundingBox<T, N>::Overlaps(const BoundingBox& other) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (upperCorner[i] < other.lowerCorner[i] || lowerCorner[i] > other.upperCorner[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename T, size_t N>
	bool BoundingBox<T, N>::Contains(const VectorType& point) const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (upperCorner[i] < point[i] || lowerCorner[i] > point[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename T, size_t N>
	Vector<T, N> BoundingBox<T, N>::MidPoint() const
	{
		Vector<T, N> result;

		for (size_t i = 0; i < N; ++i)
		{
			result[i] = (upperCorner[i] + lowerCorner[i]) / 2;
		}

		return result;
	}

	template <typename T, size_t N>
	T BoundingBox<T, N>::DiagonalLength() const
	{
		T result = 0;

		for (size_t i = 0; i < N; ++i)
		{
			result += Square(upperCorner[i] - lowerCorner[i]);
		}

		return std::sqrt(result);
	}

	template <typename T, size_t N>
	T BoundingBox<T, N>::DiagonalLengthSquared() const
	{
		T result = 0;

		for (size_t i = 0; i < N; ++i)
		{
			result += square(upperCorner[i] - lowerCorner[i]);
		}

		return result;
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