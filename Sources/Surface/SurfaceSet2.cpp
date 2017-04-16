/*************************************************************************
> File Name: SurfaceSet2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D surface set.
> Created Time: 2017/04/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Surface/SurfaceSet2.h>

namespace CubbyFlow
{
	SurfaceSet2::SurfaceSet2()
	{
		// Do nothing
	}

	SurfaceSet2::SurfaceSet2(
		const std::vector<Surface2Ptr>& others,
		const Transform2& transform,
		bool isNormalFlipped) :
		Surface2(transform, isNormalFlipped), m_surfaces(others)
	{
		// Do nothing
	}

	SurfaceSet2::SurfaceSet2(const SurfaceSet2& other) :
		Surface2(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	size_t SurfaceSet2::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const Surface2Ptr& SurfaceSet2::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void SurfaceSet2::AddSurface(const Surface2Ptr& surface)
	{
		m_surfaces.push_back(surface);
	}

	Vector2D SurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
	{
		Vector2D result(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			Vector2D localClosestPoint = surface->ClosestPoint(otherPoint);
			double localDistance = surface->ClosestDistance(otherPoint);

			if (localDistance < minimumDistance)
			{
				result = localClosestPoint;
				minimumDistance = localDistance;
			}
		}

		return result;
	}

	Vector2D SurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
	{
		Vector2D result(1, 0);

		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			Vector2D localClosestNormal = surface->ClosestNormal(otherPoint);
			double localDistance = surface->ClosestDistance(otherPoint);

			if (localDistance < minimumDistance)
			{
				result = localClosestNormal;
				minimumDistance = localDistance;
			}
		}

		return result;
	}

	double SurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
	{
		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			double localDistance = surface->ClosestDistance(otherPoint);
			minimumDistance = std::min(minimumDistance, localDistance);
		}

		return minimumDistance;
	}

	bool SurfaceSet2::IntersectsLocal(const Ray2D& ray) const
	{
		for (const auto& surface : m_surfaces)
		{
			if (surface->Intersects(ray))
			{
				return true;
			}
		}

		return false;
	}

	SurfaceRayIntersection2 SurfaceSet2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		SurfaceRayIntersection2 intersection;
		double min = std::numeric_limits<double>::min();

		for (const auto& surface : m_surfaces)
		{
			SurfaceRayIntersection2 localResult = surface->ClosestIntersection(ray);

			if (localResult.isIntersecting && localResult.t < min)
			{
				intersection = localResult;
				min = localResult.t;
			}
		}

		return intersection;
	}

	BoundingBox2D SurfaceSet2::BoundingBoxLocal() const
	{
		BoundingBox2D bbox;

		for (const auto& surface : m_surfaces)
		{
			bbox.Merge(surface->BoundingBox());
		}

		return bbox;
	}

	SurfaceSet2::Builder SurfaceSet2::GetBuilder()
	{
		return Builder();
	}
	
	SurfaceSet2::Builder& SurfaceSet2::Builder::WithSurfaces(const std::vector<Surface2Ptr>& others)
	{
		m_surfaces = others;
		return *this;
	}

	SurfaceSet2 SurfaceSet2::Builder::Build() const
	{
		return SurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped);
	}

	SurfaceSet2Ptr SurfaceSet2::Builder::MakeShared() const
	{
		return std::shared_ptr<SurfaceSet2>(new SurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped),
			[](SurfaceSet2* obj)
		{
			delete obj;
		});
	}
}