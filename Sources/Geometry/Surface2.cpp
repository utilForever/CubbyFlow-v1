/*************************************************************************
> File Name: Surface2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D surface.
> Created Time: 2017/04/08
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/Surface2.h>

namespace CubbyFlow
{
	Surface2::Surface2(const Transform2& transform_, bool isNormalFlipped_) :\
		transform(transform_), isNormalFlipped(isNormalFlipped_)
	{
<<<<<<< HEAD
		this->transform = transform;
		this->isNormalFlipped = isNormalFlipped;
	}

	Surface2::Surface2(const Surface2& other)
	{
		this->transform = other.transform;
		this->isNormalFlipped = other.isNormalFlipped;
=======
		// Do nothing
	}

	Surface2::Surface2(const Surface2& other) :
		transform(other.transform), isNormalFlipped(other.isNormalFlipped)
	{
		// Do nothing
>>>>>>> a40eaf72c9810c2b389e0b2ca6fd650d8ac5d87f
	}

	Surface2::~Surface2()
	{
		// Do nothing
	}

	Vector2D Surface2::ClosestPoint(const Vector2D& otherPoint) const
	{
<<<<<<< HEAD
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
		
=======
		return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
	}

	BoundingBox2D Surface2::BoundingBox() const
	{
		return transform.ToWorld(BoundingBoxLocal());
	}

	bool Surface2::Intersects(const Ray2D& ray) const
	{
		return IntersectsLocal(transform.ToLocal(ray));
>>>>>>> a40eaf72c9810c2b389e0b2ca6fd650d8ac5d87f
	}

	double Surface2::ClosestDistance(const Vector2D& otherPoint) const
	{
		return ClosestDistanceLocal(transform.ToLocal(otherPoint));
	}

	SurfaceRayIntersection2 Surface2::ClosestIntersection(const Ray2D& ray) const
	{
		auto result = ClosestIntersectionLocal(transform.ToLocal(ray));

		result.point = transform.ToWorld(result.point);
		result.normal = transform.ToWorldDirection(result.normal);
		result.normal *= (isNormalFlipped) ? -1.0 : 1.0;
		
		return result;
	}

	Vector2D Surface2::ClosestNormal(const Vector2D& otherPoint) const
	{
		auto result = transform.ToWorldDirection(ClosestNormalLocal(transform.ToLocal(otherPoint)));
		result *= (isNormalFlipped) ? -1.0 : 1.0;
		return result;
	}

	bool Surface2::IntersectsLocal(const Ray2D& rayLocal) const
	{
		auto result = ClosestIntersectionLocal(rayLocal);
		return result.isIntersecting;
	}

	double Surface2::ClosestDistanceLocal(const Vector2D& otherPointLocal) const
	{
		return otherPointLocal.DistanceTo(ClosestPointLocal(otherPointLocal));
	}

}