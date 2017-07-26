#include "pch.h"

#include <Searcher\PointSimpleListSearcher2.h>

using namespace CubbyFlow;

TEST(PointSimpleListSearcher2, ForEachNearByPoint)
{
	Array1<Vector2D> points = {
		Vector2D(1, 1),
		Vector2D(3, 4),
		Vector2D(-1, 2)
	};

	PointSimpleListSearcher2 searcher;
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(
		Vector2D(0, 0),
		std::sqrt(15.0),
		[&](size_t i, const Vector2D& pt)
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

TEST(PointSimpleListSearcher2, HasNearByPoint)
{
	Array1<Vector2D> points = {
		Vector2D(1, 3),
		Vector2D(3, 2),
		Vector2D(-1, 2)
	};

	PointSimpleListSearcher2 searcher;
	searcher.Build(points.Accessor());

	bool result;
	result = searcher.HasNearbyPoint(Vector2D(0, 0), std::sqrt(10.0));

	EXPECT_TRUE(result);
}

TEST(PointSimpleListSearcher2, Serialize)
{
	Array1<Vector2D> points = {
		Vector2D(1, 3),
		Vector2D(2, 4),
		Vector2D(3, 1)
	};

	std::vector<const Vector2D&> result;

	PointSimpleListSearcher2 searcher;
	searcher.Build(points.Accessor());

	std::vector<uint8_t>* buf;

	searcher.Serialize(buf);

	PointSimpleListSearcher2 searcher2;
	searcher2.Deserialize(*buf);

	searcher.ForEachNearbyPoint(
		Vector2D(0, 0),
		std::sqrt(std::numeric_limits<double>::max()),
		[&](size_t i, const Vector2D& pt)
	{
		result.push_back(pt);
	});

	searcher2.ForEachNearbyPoint(
		Vector2D(0, 0),
		std::sqrt(std::numeric_limits<double>::max()),
		[&](size_t i, const Vector2D& pt)
	{
		EXPECT_EQ(result[i].x, pt.x);
		EXPECT_EQ(result[i].y, pt.y);
	});
}
