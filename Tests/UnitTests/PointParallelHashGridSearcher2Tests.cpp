#include "pch.h"

#include <Searcher\PointParallelHashGridSearcher2.h>

using namespace CubbyFlow;

TEST(PointParallelHashGridSearcher2, ForEachNearByPoint)
{
	Array1<Vector2D> points = {
		Vector2D(1, 1),
		Vector2D(3, 4),
		Vector2D(-1, 2)
	};

	PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(10));
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(
		Vector2D(0, 0),
		std::sqrt(15.0),
		[&points](size_t i, const Vector2D& pt)
	{
		EXPECT_TRUE(i == 0 || i == 2);
		if (i == 0)
		{
			EXPECT_EQ(points[0], pt);
		}
		else if (i == 2)
		{
			EXPECT_EQ(points[2], pt);
		}
	});
}

TEST(PointParallelHashGridSearcher2, HasEachNearByPoint)
{
	Array1<Vector2D> points = {
		Vector2D(1, 1),
		Vector2D(3, 4),
		Vector2D(4, 5)
	};

	PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(10));
	searcher.Build(points.Accessor());

	bool result;
	result = searcher.HasNearbyPoint(Vector2D(), std::sqrt(15.0));

	EXPECT_TRUE(result);
}

TEST(PointParallelHashGridSearcher2, Build)
{
	Array1<Vector2D> points = {
		Vector2D(3, 4),
		Vector2D(1, 5),
		Vector2D(-3, 0)
	};

	PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(10));
	searcher.Build(points.Accessor());

	EXPECT_EQ(4, searcher.GetHashKeyFromBucketIndex(Point2I(0, 1)));
	EXPECT_EQ(8, searcher.GetHashKeyFromBucketIndex(Point2I(0, 2)));
	EXPECT_EQ(3, searcher.GetHashKeyFromBucketIndex(Point2I(-1, 0)));
	
	std::vector<size_t> resultKey;
	
	resultKey.push_back(3);
	resultKey.push_back(4);
	resultKey.push_back(8);

	for (size_t i = 0; i < 3; i++)
	{
		EXPECT_EQ(resultKey[i], searcher.Keys[i]);
	}

	EXPECT_EQ(0, searcher.StartIndexTable[3]);
	EXPECT_EQ(1, searcher.StartIndexTable[4]);
	EXPECT_EQ(2, searcher.StartIndexTable[8]);

	EXPECT_EQ(1, searcher.EndIndexTable[3]);
	EXPECT_EQ(2, searcher.EndIndexTable[4]);
	EXPECT_EQ(3, searcher.EndIndexTable[8]);
}