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
	EXPECT_EQ(4, searcher.GetHashKeyFromBucketIndex(Point2I(0, 1)));
	EXPECT_EQ(3, searcher.GetHashKeyFromBucketIndex(Point2I(-1, 0)));

	std::vector<size_t> resultKey;

	resultKey.push_back(3);
	resultKey.push_back(4);
	resultKey.push_back(4);

	for (size_t i = 0; i < 3; i++)
	{
		EXPECT_EQ(resultKey[i], searcher.Keys()[i]);
	}

	EXPECT_EQ(0, searcher.StartIndexTable()[3]);
	EXPECT_EQ(1, searcher.StartIndexTable()[4]);

	EXPECT_EQ(1, searcher.EndIndexTable()[3]);
	EXPECT_EQ(3, searcher.EndIndexTable()[4]);
}

TEST(PointParallelHashGridSearcher2, Serialization)
{
	Array1<Vector2D> points = {
		Vector2D(1, 34),
		Vector2D(13, 12441),
		Vector2D(132, 141)
	};

	PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(9)), searcher2(1, 1, std::sqrt(1));

	searcher.Build(points.Accessor());

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

	std::vector<Vector2D> tmp;
	std::vector<Vector2D> tmp2;

	searcher.ForEachNearbyPoint(
		Vector2D(),
		std::numeric_limits<double>::max(),
		[&](size_t i, const Vector2D& pt)
	{
		tmp.push_back(pt);
	});

	searcher2.ForEachNearbyPoint(
		Vector2D(), 
		std::numeric_limits<double>::max(),
		[&](size_t i, const Vector2D& pt)
	{
		tmp2.push_back(pt);
	});

	for (size_t i = 0; i < 3; i++)
	{
		EXPECT_EQ(tmp[i], tmp2[i]);
	}

	buf.clear();
}