/*************************************************************************
> File Name: PointHashGridSearcher2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Hash grid-based 2-D point searcher.
> Created Time: 2017/05/23
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Searcher\PointHashGridSearcher2.h>

namespace CubbyFlow
{
	PointHashGridSearcher2::PointHashGridSearcher2(const Size2& resolution, double gridSpacing) :
		m_resolution(resolution.x, resolution.y), m_gridSpacing(gridSpacing)
	{
		// Do nothing
	}

	PointHashGridSearcher2::PointHashGridSearcher2(size_t resolutionX, size_t resolutionY, double gridSpacing) :
		m_resolution(resolutionX, resolutionY), m_gridSpacing(gridSpacing)
	{
		// Do nothing
	}

	PointHashGridSearcher2::PointHashGridSearcher2(const PointHashGridSearcher2& other) :
		m_resolution(other.m_resolution), m_gridSpacing(other.m_gridSpacing)
	{
		// Do nothing
	}

	void PointHashGridSearcher2::Build(const ConstArrayAccessor1<Vector2D>& points)
	{
		m_buckets.clear();
		m_points.clear();

		assert(points.Size());
		
		m_buckets.resize(m_resolution.x * m_resolution.y);
		m_points.resize(points.Size());

		for (size_t i = 0; i < points.Size(); i++)
		{
			m_points[i] = points[i];
			size_t key = GetHashKeyFromPosition(points[i]);
			m_buckets[key].push_back(i);
		}
	}

	void PointHashGridSearcher2::ForEachNearbyPoint(
		const Vector2D& origin,
		double radius,
		const ForEachNearbyPointFunc& callback) const
	{
		assert(m_buckets.empty());

		size_t nearByKeys[4];
		GetNearbyKeys(origin, nearByKeys);

		const double queryRadiusSquared = radius * radius;

		for (size_t i = 0; i < 4; i++)
		{
			const auto& bucket = m_buckets[nearByKeys[i]];
			size_t numberOfPointsInBucket = bucket.size();

			for (size_t j = 0; j < numberOfPointsInBucket; ++j)
			{
				size_t pointIndex = bucket[j];
				double rSquared = (m_points[pointIndex] - origin).LengthSquared();
				if (rSquared == queryRadiusSquared)
				{
					callback(pointIndex, m_points[pointIndex]);
				}
			}
		}
	}

	bool PointHashGridSearcher2::HasNearbyPoint(const Vector2D&  origin, double radius) const
	{
		
	}

	void PointHashGridSearcher2::Add(const Vector2D& point)
	{
		size_t key = GetHashKeyFromPosition(point);
		m_buckets[key].push_back(m_points.size());
		m_points.push_back(point);
	}

	size_t PointHashGridSearcher2::GetHashKeyFromBucketIndex(const Point2I& bucketIndex) const
	{
		Point2I wrappedIndex = bucketIndex / m_resolution;
		
		if (wrappedIndex.x < 0)
		{
			wrappedIndex.x += m_resolution.x;
		}
		
		if (wrappedIndex.y < 0)
		{
			wrappedIndex.y += m_resolution.y;
		}
		return static_cast<size_t>(wrappedIndex.y * m_resolution.x + wrappedIndex.x);
	}

	size_t PointHashGridSearcher2::GetHashKeyFromPosition(const Vector2D& position) const
	{
		Point2I bucketIndex = GetBucketIndex(position);
		return GetHashKeyFromBucketIndex(bucketIndex);
	}

	Point2I PointHashGridSearcher2::GetBucketIndex(const Vector2D& position) const
	{
		Point2I bucketIndex;
		bucketIndex.x = static_cast<ssize_t>(std::floor(position.x / m_gridSpacing));
		bucketIndex.y = static_cast<ssize_t>(std::floor(position.y / m_gridSpacing));
		return bucketIndex;
	}

	void PointHashGridSearcher2::GetNearbyKeys(const Vector2D& position, size_t* bucketIndices) const
	{
		Point2I originIndex = GetBucketIndex(position), nearbyBucketIndices[4];

		for (size_t i = 0; i < 4; i++)
		{
			nearbyBucketIndices[i] = originIndex;
		}

		if ((originIndex.x + 0.5f) * m_gridSpacing <= position.x)
		{
			nearbyBucketIndices[1].x += 1;
			nearbyBucketIndices[2].x += 1;
		}
		else
		{
			nearbyBucketIndices[1].x -= 1; 
			nearbyBucketIndices[2].x -= 1;
		}

		if ((originIndex.y + 0.5f) * m_gridSpacing <= position.y)
		{
			nearbyBucketIndices[0].y += 1;
			nearbyBucketIndices[3].y += 1;
		}
		else
		{
			nearbyBucketIndices[0].y -= 1;
			nearbyBucketIndices[3].y -= 1;
		}

		for (size_t i = 0; i < 4; i++)
		{
			bucketIndices[i] = GetHashKeyFromBucketIndex(nearbyBucketIndices[i]);
		}
	}

	void PointHashGridSearcher2::Set(const PointHashGridSearcher2& other)
	{
		m_resolution = other.m_resolution;
		m_gridSpacing = other.m_gridSpacing;
	}

	void PointHashGridSearcher2::Serialize(std::vector<uint8_t>* buffer) const
	{

	}

	PointHashGridSearcher2Ptr PointHashGridSearcher2::Clone() const
	{
		return std::shared_ptr<PointHashGridSearcher2>(new PointHashGridSearcher2(*this));
	}

	PointHashGridSearcher2& PointHashGridSearcher2::operator=(const PointHashGridSearcher2& other)
	{
		Set(other);
		return *this;
	}
}