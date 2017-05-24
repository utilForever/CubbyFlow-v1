/*************************************************************************
> File Name: PointHashGridSearcher3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Hash grid-based 3-D point searcher.
> Created Time: 2017/05/24
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Searcher\PointHashGridSearcher3.h>
#include <flatbuffers/flatbuffers.h>
#include <Utils\FlatbuffersHelper.h>

#include <Flatbuffers/generated/PointHashGridSearcher3_generated.h>

namespace CubbyFlow
{
	PointHashGridSearcher3::PointHashGridSearcher3(const Size3& resolution, double gridSpacing) :
		m_resolution(resolution.x, resolution.y, resolution.z), m_gridSpacing(gridSpacing)
	{
		// Do nothing
	}

	PointHashGridSearcher3::PointHashGridSearcher3(size_t resolutionX, size_t resolutionY, size_t resolutionZ, double gridSpacing) :
		m_resolution(resolutionX, resolutionY, resolutionZ), m_gridSpacing(gridSpacing)
	{
		// Do nothing
	}

	PointHashGridSearcher3::PointHashGridSearcher3(const PointHashGridSearcher3& other) :
		m_resolution(other.m_resolution), m_gridSpacing(other.m_gridSpacing)
	{
		// Do nothing
	}

	void PointHashGridSearcher3::Build(const ConstArrayAccessor1<Vector3D>& points)
	{
		m_buckets.clear();
		m_points.clear();

		assert(points.Size());

		m_buckets.resize(m_resolution.x * m_resolution.y * m_resolution.z);
		m_points.resize(points.Size());

		for (size_t i = 0; i < points.Size(); i++)
		{
			m_points[i] = points[i];
			size_t key = GetHashKeyFromPosition(points[i]);
			m_buckets[key].push_back(i);
		}
	}

	void PointHashGridSearcher3::ForEachNearbyPoint(
		const Vector3D& origin,
		double radius,
		const ForEachNearbyPointFunc& callback) const
	{
		assert(m_buckets.empty());

		size_t nearByKeys[4];
		GetNearbyKeys(origin, nearByKeys);

		const double queryRadiusSquared = radius * radius;

		for (size_t i = 0; i < 8; i++)
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

	bool PointHashGridSearcher3::HasNearbyPoint(const Vector3D&  origin, double radius) const
	{
		size_t nearbyKeys[4];
		GetNearbyKeys(origin, nearbyKeys);

		for (int i = 0; i < 8; i++)
		{
			const auto& bucket = m_buckets[nearbyKeys[i]];
			size_t numberOfPointsInBucket = bucket.size();

			for (size_t j = 0; j < numberOfPointsInBucket; ++j)
			{
				if (m_points[bucket[j]].DistanceTo(origin) <= radius) return true;
			}
		}
		return false;
	}

	void PointHashGridSearcher3::Add(const Vector3D& point)
	{
		size_t key = GetHashKeyFromPosition(point);
		m_buckets[key].push_back(m_points.size());
		m_points.push_back(point);
	}

	size_t PointHashGridSearcher3::GetHashKeyFromBucketIndex(const Point3I& bucketIndex) const
	{
		Point3I wrappedIndex = bucketIndex / m_resolution;

		if (wrappedIndex.x < 0)
		{
			wrappedIndex.x += m_resolution.x;
		}

		if (wrappedIndex.y < 0)
		{
			wrappedIndex.y += m_resolution.y;
		}

		if (wrappedIndex.z < 0)
		{
			wrappedIndex.z += m_resolution.z;
		}

		return static_cast<size_t>((wrappedIndex.z * m_resolution.y + wrappedIndex.y) * m_resolution.x + wrappedIndex.x);
	}

	size_t PointHashGridSearcher3::GetHashKeyFromPosition(const Vector3D& position) const
	{
		Point3I bucketIndex = GetBucketIndex(position);
		return GetHashKeyFromBucketIndex(bucketIndex);
	}

	Point3I PointHashGridSearcher3::GetBucketIndex(const Vector3D& position) const
	{
		Point3I bucketIndex;
		bucketIndex.x = static_cast<ssize_t>(std::floor(position.x / m_gridSpacing));
		bucketIndex.y = static_cast<ssize_t>(std::floor(position.y / m_gridSpacing));
		bucketIndex.z = static_cast<ssize_t>(std::floor(position.z / m_gridSpacing));
		return bucketIndex;
	}

	void PointHashGridSearcher3::GetNearbyKeys(const Vector3D& position, size_t* bucketIndices) const
	{
		Point3I originIndex = GetBucketIndex(position), nearbyBucketIndices[4];

		for (int i = 0; i < 8; ++i)
		{
			nearbyBucketIndices[i] = originIndex;
		}

		if ((originIndex.x + 0.5f) * m_gridSpacing <= position.x)
		{
			nearbyBucketIndices[4].x += 1;
			nearbyBucketIndices[5].x += 1;
			nearbyBucketIndices[6].x += 1;
			nearbyBucketIndices[7].x += 1;
		}
		else
		{
			nearbyBucketIndices[4].x -= 1;
			nearbyBucketIndices[5].x -= 1;
			nearbyBucketIndices[6].x -= 1;
			nearbyBucketIndices[7].x -= 1;
		}

		if ((originIndex.y + 0.5f) * m_gridSpacing <= position.y)
		{
			nearbyBucketIndices[2].y += 1;
			nearbyBucketIndices[3].y += 1;
			nearbyBucketIndices[6].y += 1;
			nearbyBucketIndices[7].y += 1;
		}
		else
		{
			nearbyBucketIndices[2].y -= 1;
			nearbyBucketIndices[3].y -= 1;
			nearbyBucketIndices[6].y -= 1;
			nearbyBucketIndices[7].y -= 1;
		}

		if ((originIndex.z + 0.5f) * m_gridSpacing <= position.z)
		{
			nearbyBucketIndices[1].z += 1;
			nearbyBucketIndices[3].z += 1;
			nearbyBucketIndices[5].z += 1;
			nearbyBucketIndices[7].z += 1;
		}
		else
		{
			nearbyBucketIndices[1].z -= 1;
			nearbyBucketIndices[3].z -= 1;
			nearbyBucketIndices[5].z -= 1;
			nearbyBucketIndices[7].z -= 1;
		}

		for (int i = 0; i < 8; ++i)
		{
			bucketIndices[i] = GetHashKeyFromBucketIndex(nearbyBucketIndices[i]);
		}
	}

	void PointHashGridSearcher3::Set(const PointHashGridSearcher3& other)
	{
		m_resolution = other.m_resolution;
		m_gridSpacing = other.m_gridSpacing;
		m_points = other.m_points;
		m_buckets = other.m_buckets;
	}

	void PointHashGridSearcher3::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		auto fbsResolution = fbs::Size3(m_resolution.x, m_resolution.y, m_resolution.z);

		std::vector<fbs::Vector3D> points;
		for (const auto& pt : m_points)
		{
			points.push_back(CubbyFlowToFlatbuffers(pt));
		}

		auto fbsPoints = builder.CreateVectorOfStructs(points.data(), points.size());
		auto fbsSearcher = fbs::CreatePointHashGridSearcher3(bulider, m_resolution, m_gridSpacing);
		builder.Finish(fbsSearcher);

		uint8_t *buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void PointHashGridSearcher3::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto fbsSearcher = fbs::GetPointHashGridSearcher3(buffer.data());

		auto res = FlatbuffersToCubbyFlow(*fbsSearcher->Resolution());
		m_resolution.Set({ res.x, res.y });
		m_gridSpacing = fbsSearcher->GridSpacing();

		auto fbsPoints = fbsSearcher->Points();
		m_points.resize(fbsPoints->size());

		for (uint32_t i = 0; i < fbsPoints->size(); ++i)
		{
			m_points[i] = FlatbuffersToCubbyFlow(*fbsPoints->Get(i));
		}
	}

	const std::vector<std::vector<size_t>>& PointHashGridSearcher3::GetBuckets() const
	{
		return m_buckets;
	}

	PointNeighborSearcher3Ptr PointHashGridSearcher3::Clone() const
	{
		return std::shared_ptr<PointNeighborSearcher3>(
			new PointHashGridSearcher3(*this), [](PointHashGridSearcher3* obj)
		{
			delete obj;
		});
	}

	PointHashGridSearcher3& PointHashGridSearcher3::operator=(const PointHashGridSearcher3& other)
	{
		Set(other);
		return *this;
	}

	PointHashGridSearcher3::Builder PointHashGridSearcher3::GetBuilder()
	{
		return Builder();
	}

	PointHashGridSearcher3 PointHashGridSearcher3::Builder::Build() const
	{
		return PointHashGridSearcher3(m_resolution, m_gridSpacing);
	}

	PointHashGridSearcher3::Builder& PointHashGridSearcher3::Builder::WithResolution(const Size3& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	PointHashGridSearcher3::Builder& PointHashGridSearcher3::Builder::WithGridSpacing(double gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	PointHashGridSearcher3Ptr PointHashGridSearcher3::Builder::MakeShared() const
	{
		return std::shared_ptr<PointHashGridSearcher3>(new PointHashGridSearcher3(m_resolution, m_gridSpacing),
			[](PointHashGridSearcher3* obj)
		{
			delete obj;
		});
	}

	PointNeighborSearcher3Ptr PointHashGridSearcher3::Builder::BuildPointNeighborSearcher() const
	{
		return std::make_shared<PointNeighborSearcher3>(new PointHashGridSearcher3(m_resolution, m_gridSpacing),
			[](PointNeighborSearcher3Ptr* obj) {
			delete obj;
		});
	}
}