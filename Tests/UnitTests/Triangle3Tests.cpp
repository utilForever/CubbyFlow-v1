#include "pch.h"

#include <Geometry/Triangle3.h>

using namespace CubbyFlow;

TEST(Triangle3, Constructors)
{
	Triangle3 tri1;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(0.0, tri1.points[i][j]);
			EXPECT_DOUBLE_EQ(0.0, tri1.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(0.0, tri1.uvs[i][j]);
		}
	}

	std::array<Vector3D, 3> points =
	{ 
		Vector3D(1, 2, 3),
		Vector3D(4, 5, 6),
		Vector3D(7, 8, 9)
	};
	std::array<Vector3D, 3> normals = 
	{
		Vector3D(1, 0, 0),
		Vector3D(0, 1, 0),
		Vector3D(0, 0, 1)
	};
	std::array<Vector2D, 3> uvs =
	{
		Vector2D(1, 0),
		Vector2D(0, 1),
		Vector2D(0.5, 0.5)
	};

	Triangle3 tri2(points, normals, uvs);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri2.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri2.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri2.uvs[i][j]);
		}
	}

	Triangle3 tri3(tri2);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri3.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri3.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri3.uvs[i][j]);
		}
	}
}

TEST(Triangle3, Builder)
{
	std::array<Vector3D, 3> points =
	{
		Vector3D(1, 2, 3),
		Vector3D(4, 5, 6),
		Vector3D(7, 8, 9)
	};
	std::array<Vector3D, 3> normals =
	{
		Vector3D(1, 0, 0),
		Vector3D(0, 1, 0),
		Vector3D(0, 0, 1)
	};
	std::array<Vector2D, 3> uvs =
	{
		Vector2D(1, 0),
		Vector2D(0, 1),
		Vector2D(0.5, 0.5)
	};

	Triangle3 tri = Triangle3::GetBuilder()
		.WithPoints(points)
		.WithNormals(normals)
		.WithUVs(uvs)
		.Build();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri.uvs[i][j]);
		}
	}
}