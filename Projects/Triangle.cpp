/*************************************************************************
> File Name: Triangle3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D triangle geometry.
> Created Time: 2017/04/14
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Geometry\Triangle3.h>
#include<Math\Quaternion.h>

namespace CubbyFlow
{
	Triangle3::Triangle3(const Transform3& transform, bool isNormalFlipped)
	{
		// Do nothing
	}

	Triangle3::Triangle3(
		const std::array<Vector3D, 3>& points_,
		const std::array<Vector3D, 3>& normals_,
		const std::array<Vector2D, 3>& uvs_,
		const Transform3& transform_,
		bool isNormalFlipped_) :
		points(points), normals(normals_), uvs(uvs_)
	{
		// Do nothing
	}

	Triangle3::Triangle3(const Triangle3& other) :
		points(other.points), normals(other.normals), uvs(other.uvs)
	{
		// Do nothing
	}

	double Triangle3::Area() const
	{
		return (points[1] - points[0]).Cross(points[2] - points[1]).Length() / 2;
	}

	void Triangle3::GetBarycentricCoords(const Vector3D& pt, double* b0, double* b1, double* b2) const
	{
		*b0 = (points[1] - pt).Cross(points[2] - points[1]).Length() / 2 / Area();
		*b1 = (pt - points[0]).Cross(points[2] - pt).Length() / 2 / Area();
		*b2 = 1 - *b0 - *b1;
	}

	Vector3D Triangle3::FaceNormal() const
	{
		return (points[1] - points[0]).Cross(points[2] - points[1]);
	}

	void Triangle3::SetNormalsToFaceNormal()
	{	
		Quaternion<double> direction = Quaternion<double>(normals, normals);
	}
}