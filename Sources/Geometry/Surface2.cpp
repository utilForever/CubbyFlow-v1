/*************************************************************************
> File Name: Surface2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for 2-D surface.
> Created Time: 2017/03/23
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Geometry\Surface2.h>

namespace CubbyFlow
{
	Surface2::Surface2(const Transform2& transform = Transform2(), bool isNormalFlipped = false)
	{
		this->transform = transform;
		this->isNormalFlipped = isNormalFlipped;
	}

	Surface2::Surface2(const Surface2& other)
	{
		this->transform = other.transform;
		this->isNormalFlipped = other.isNormalFlipped;
	}

	Surface2::~Surface2()
	{
		// Do nothing
	}

	Vector2D Surface2::ClosestPoint(const Vector2D& otherPoint) const
	{
		double n1 = transform.Translation().x;
		double n2 = transform.Translation().y;
		return Vector2D((n2 * n2 * otherPoint.x - n1 * n2 * otherPoint.y) / (n1 * n1 + n2 * n2), (n1 * n1 * otherPoint.x - n1 * n2 * otherPoint.y) / (n1 * n1 + n2 * n2));
	}

	BoundingBox2D Surface2::BoundingBox() const
	{
		return BoundingBox2D(Vector2D(0, 0), Vector2D(transform.Translation().x, transform.Translation().y));
	}

	bool Surface2::Intersects(const Ray2D& ray) const
	{
		Vector2D p1 = transform.Translation();
				
		double tc = -p1.x * ray.origin.y + p1.y * ray.origin.x;
		double td = -p1.x * ray.direction.y + p1.y * ray.direction.x;
		return tc * td < 0;
	}

	double Surface2::ClosestDistance(const Vector2D& otherPoint) const
	{
		return otherPoint.DistanceTo(ClosestPoint(otherPoint));
	}

	SurfaceRayIntersection2 Surface2::ClosestIntersection(const Ray2D& ray) const
	{
		
	}

	Vector2D Surface2::ClosestNormal(const Vector2D& otherPoint) const
	{
		
	}
}