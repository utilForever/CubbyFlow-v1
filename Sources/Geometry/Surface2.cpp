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
	Surface2::Surface2(const Transform2& transform_, bool isNormalFlipped_) :
		transform(transform_), isNormalFlipped(isNormalFlipped_)
	{
		this->transform = transform;
		this->isNormalFlipped = isNormalFlipped;
	}

	Surface2::Surface2(const Surface2& other) :
		transform(other.transform), isNormalFlipped(other.isNormalFlipped)
	{
		// Do nothing
	}

	Surface2::~Surface2()
	{
		// Do nothing
	}

	Vector2D Surface2::ClosestPoint(const Vector2D& otherPoint) const
	{
		Vector2D normal = transform.Translation();
		double t = normal.Dot(otherPoint) / normal.LengthSquared();
		return otherPoint - normal * t;
	}

	BoundingBox2D Surface2::BoundingBox() const
	{
		return transform.ToWorld(BoundingBoxLocal());
	}

	bool Surface2::Intersects(const Ray2D& ray) const
	{
		return IntersectsLocal(transform.ToLocal(ray));
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