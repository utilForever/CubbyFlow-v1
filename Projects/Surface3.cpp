/*************************************************************************
> File Name: Surface3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for 3-D surface.
> Created Time: 2017/04/09
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Geometry\Surface3.h>

namespace CubbyFlow
{
	Surface3::Surface3(const Transform3& transform_ , bool isNormalFlipped_) : 
		transform(transform_), isNormalFlipped(isNormalFlipped_)
	{
		// Do nothing
	}

	Surface3::Surface3(const Surface3& other) :
		transform(other.transform), isNormalFlipped(other.isNormalFlipped)
	{
		// Do nothing
	}

	Surface3::~Surface3()
	{
		// Do nothing
	}

	Vector3D Surface3::ClosestPoint(const Vector3D& otherPoint) const
	{
		Vector3D normal = transform.Translation();
		double t = normal.Dot(otherPoint) / normal.LengthSquared();
		return otherPoint - normal * t;
	}

	BoundingBox3D Surface3::BoundingBox() const
	{
		return BoundingBox3D(Vector3D(0, 0, 0), Vector3D(transform.Translation().x, transform.Translation().y, transform.Translation().z));
	}

	bool Surface3::Intersects(const Ray3D& ray) const
	{
		Vector3D normal = transform.Translation();
		return normal.Dot(ray.origin) * normal.Dot(ray.direction) < 0;
	}

	double Surface3::ClosestDistance(const Vector3D& otherPoint) const
	{
		ClosestDistanceLocal(transform.ToLocal(otherPoint));
	}

	Vector3D Surface3::ClosestNormal(const Vector3D& otherPoint) const
	{
		transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
	}

	SurfaceRayIntersection3 Surface3::ClosestIntersection(const Ray3D& ray) const
	{
	
	}

	bool Surface3::IntersectsLocal(const Ray3D& ray) const
	{
		auto result = ClosestIntersectionLocal(ray);
		return result.isIntersecting;
	}

	double Surface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return otherPoint.DistanceTo(ClosestPointLocal(otherPoint));
	}
}