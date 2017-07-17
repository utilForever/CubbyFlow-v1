#include "pch.h"

#include <Geometry/TriangleMesh3.h>

using namespace CubbyFlow;

TEST(TriangleMesh3, Constructors)
{
	TriangleMesh3 mesh1;
	EXPECT_EQ(0u, mesh1.NumberOfPoints());
	EXPECT_EQ(0u, mesh1.NumberOfNormals());
	EXPECT_EQ(0u, mesh1.NumberOfUVs());
	EXPECT_EQ(0u, mesh1.NumberOfTriangles());
}

TEST(TriangleMesh3, Builder)
{
	TriangleMesh3::PointArray points =
	{
		Vector3D(1, 2, 3),
		Vector3D(4, 5, 6),
		Vector3D(7, 8, 9),
		Vector3D(10, 11, 12)
	};

	TriangleMesh3::NormalArray normals =
	{
		Vector3D(10, 11, 12),
		Vector3D(7, 8, 9),
		Vector3D(4, 5, 6),
		Vector3D(1, 2, 3)
	};

	TriangleMesh3::UVArray uvs =
	{
		Vector2D(13, 14),
		Vector2D(15, 16)
	};

	TriangleMesh3::IndexArray pointIndices =
	{
		Point3UI(0, 1, 2),
		Point3UI(0, 1, 3)
	};

	TriangleMesh3::IndexArray normalIndices =
	{
		Point3UI(1, 2, 3),
		Point3UI(2, 1, 0)
	};

	TriangleMesh3::IndexArray uvIndices =
	{
		Point3UI(1, 0, 2),
		Point3UI(3, 1, 0)
	};

	TriangleMesh3 mesh = TriangleMesh3::GetBuilder()
		.WithPoints(points)
		.WithNormals(normals)
		.WithUVs(uvs)
		.WithPointIndices(pointIndices)
		.WithNormalIndices(normalIndices)
		.WithUVIndices(uvIndices)
		.Build();

	EXPECT_EQ(4u, mesh.NumberOfPoints());
	EXPECT_EQ(4u, mesh.NumberOfNormals());
	EXPECT_EQ(2u, mesh.NumberOfUVs());
	EXPECT_EQ(2u, mesh.NumberOfTriangles());

	for (size_t i = 0; i < mesh.NumberOfPoints(); ++i)
	{
		EXPECT_EQ(points[i], mesh.Point(i));
	}

	for (size_t i = 0; i < mesh.NumberOfNormals(); ++i)
	{
		EXPECT_EQ(normals[i], mesh.Normal(i));
	}

	for (size_t i = 0; i < mesh.NumberOfUVs(); ++i)
	{
		EXPECT_EQ(uvs[i], mesh.UV(i));
	}

	for (size_t i = 0; i < mesh.NumberOfTriangles(); ++i)
	{
		EXPECT_EQ(pointIndices[i], mesh.PointIndex(i));
		EXPECT_EQ(normalIndices[i], mesh.NormalIndex(i));
		EXPECT_EQ(uvIndices[i], mesh.UVIndex(i));
	}
}