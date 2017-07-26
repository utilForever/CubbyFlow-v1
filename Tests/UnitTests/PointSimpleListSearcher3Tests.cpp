#include "pch.h"

#include <Searcher\PointSimpleListSearcher3.h>

using namespace CubbyFlow;

TEST(PointSimpleListSearcher3, ForEachNearByPoint)
{
	Array1<Vector3D> points = {
		Vector3D(1, 1, 3),
		Vector3D(3, 4, 1),
		Vector3D(-1, 2, 1)
	};

	PointSimpleListSearcher3 searcher;
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(
		Vector3D(0, 0, 1),
		std::sqrt(15.0),
		[&](size_t i, const Vector3D& pt)
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

TEST(PointSimpleListSearcher3, HasNearByPoint)
{
	Array1<Vector3D> points = {
		Vector3D(1, 3, 2),
		Vector3D(3, 2, 4),
		Vector3D(-1, 2, 10)
	};

	PointSimpleListSearcher3 searcher;
	searcher.Build(points.Accessor());

	bool result;
	result = searcher.HasNearbyPoint(Vector3D(0, 0, 0), std::sqrt(10.0));

	EXPECT_FALSE(result);
}

TEST(PointSimpleListSearcher3, Serialize)
{
	Array1<Vector3D> points = {
		Vector3D(1, 3, 3),
		Vector3D(2, 4, 1),
		Vector3D(3, 1, 5)
	};

	std::vector<const Vector3D&> result;

	PointSimpleListSearcher3 searcher;
	searcher.Build(points.Accessor());

	std::vector<uint8_t>* buf;

	searcher.Serialize(buf);

	PointSimpleListSearcher3 searcher2;
	searcher2.Deserialize(*buf);

	searcher.ForEachNearbyPoint(
		Vector3D(),
		std::sqrt(std::numeric_limits<double>::max()),
		[&](size_t i, const Vector3D& pt)
	{
		result.push_back(pt);
	});

	searcher2.ForEachNearbyPoint(
		Vector3D(),
		std::sqrt(std::numeric_limits<double>::max()),
		[&](size_t i, const Vector3D& pt)
	{
		EXPECT_EQ(result[i].x, pt.x);
		EXPECT_EQ(result[i].y, pt.y);
		EXPECT_EQ(result[i].z, pt.z);
	});
}