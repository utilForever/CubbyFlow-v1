#include "pch.h"

#include <Searcher\PointParallelHashGridSearcher3.h>

using namespace CubbyFlow;

TEST(PointParallelHashGridSearcher3, ForEachNearByPoint)
{
	Array1<Vector3D> points = {
		Vector3D(1, 1, 1),
		Vector3D(3, 411, 5),
		Vector3D(-1, 2, -3)
	};

	PointParallelHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(10));
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(
		Vector3D(0, 0, 0),
		std::sqrt(15.0),
		[&points](size_t i, const Vector3D& pt)
	{
		EXPECT_TRUE(i == 0 || i == 2);
		if (i == 0)
		{
			EXPECT_EQ(points[0].x, pt.x);
			EXPECT_EQ(points[0].y, pt.y);
			EXPECT_EQ(points[0].z, pt.z);
		}
		else if (i == 2)
		{
			EXPECT_EQ(points[2].x, pt.x);
			EXPECT_EQ(points[2].y, pt.y);
			EXPECT_EQ(points[2].z, pt.z);
		}
	});
}

TEST(PointParallelHashGridSearcher3, HasEachNearByPoint)
{
	Array1<Vector3D> points = {
		Vector3D(1, 142, 1),
		Vector3D(3, 4123, 13),
		Vector3D(4, 1, 25)
	};

	PointParallelHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(10));
	searcher.Build(points.Accessor());

	bool result;
	result = searcher.HasNearbyPoint(Vector3D(), std::sqrt(15.0));

	EXPECT_FALSE(result);
}

TEST(PointParallelHashGridSearcher3, Build)
{
	Array1<Vector3D> points = {
		Vector3D(3, 41, 234),
		Vector3D(111, 1, 5),
		Vector3D(-3, 123, 1123)
	};

	PointParallelHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(9));
	searcher.Build(points.Accessor());

	EXPECT_EQ(1 + 4 * 1 + 2 * 4 * 4, searcher.GetHashKeyFromBucketIndex(Point3I(1, 13, 78)));
	EXPECT_EQ(1 + 4 * 4, searcher.GetHashKeyFromBucketIndex(Point3I(37, 0, 1)));
	EXPECT_EQ(3 + 4 + 2 * 4 * 4, searcher.GetHashKeyFromBucketIndex(Point3I(-1, 41, 374)));

	std::vector<size_t> resultKey;

	resultKey.push_back(17);
	resultKey.push_back(37);
	resultKey.push_back(39);

	for (size_t i = 0; i < 3; i++)
	{
		EXPECT_EQ(resultKey[i], searcher.Keys()[i]);
	}

	EXPECT_EQ(0, searcher.StartIndexTable()[17]);
	EXPECT_EQ(1, searcher.StartIndexTable()[37]);
	EXPECT_EQ(2, searcher.StartIndexTable()[39]);

	EXPECT_EQ(1, searcher.EndIndexTable()[17]);
	EXPECT_EQ(2, searcher.EndIndexTable()[37]);
	EXPECT_EQ(3, searcher.EndIndexTable()[39]);
}

TEST(PointParallelHashGridSearcher3, Serialization)
{
	Array1<Vector3D> points = {
		Vector3D(1, 2, 3),
		Vector3D(4, 5, 6),
		Vector3D(7, 8, 9)
	};

	PointParallelHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(9)), searcher2(Size3(1, 1, 1), std::sqrt(1));

	searcher.Build(points.Accessor());

	std::vector<Vector3D> src, dst;

	searcher.ForEachNearbyPoint(
		Vector3D(),
		std::numeric_limits<double>::max(),
		[&](size_t i, const Vector3D& pt)
	{
		src.push_back(pt);
	});

	for(size_t i=0;i<3;i++)
	{
		EXPECT_EQ(points[i].x, src[i].x);
		EXPECT_EQ(points[i].y, src[i].y);
		EXPECT_EQ(points[i].z, src[i].z);
	}

	std::vector<uint8_t> buf;

	searcher.Serialize(&buf);

	searcher2.Deserialize(buf);

	for (size_t i = 0; i < searcher.SortedIndices().size(); i++)
	{
		EXPECT_EQ(searcher.SortedIndices()[i], searcher2.SortedIndices()[i]);
	}

	for (size_t i = 0; i < searcher.StartIndexTable().size(); i++)
	{
		EXPECT_EQ(searcher.EndIndexTable()[i], searcher2.EndIndexTable()[i]);
		EXPECT_EQ(searcher.StartIndexTable()[i], searcher2.StartIndexTable()[i]);
	}

	searcher2.ForEachNearbyPoint(
		Vector3D(),
		std::numeric_limits<double>::max(),
		[&](size_t i, const Vector3D& pt)
	{
		dst.push_back(pt);
	});

	for (size_t i = 0; i < 3; i++)
	{
		EXPECT_EQ(src[i].x, dst[i].x);
		EXPECT_EQ(src[i].y, dst[i].y);
		EXPECT_EQ(src[i].z, dst[i].z);
	}

	buf.clear();
}