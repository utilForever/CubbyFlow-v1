/*************************************************************************
> File Name: SurfaceSet3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D surface set.
> Created Time: 2017/04/14
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Geometry\SurfaceSet3.h>

namespace CubbyFlow
{
	SurfaceSet3::SurfaceSet3()
	{
		// Do nothing
	}

	SurfaceSet3::SurfaceSet3(
		const std::vector<Surface3Ptr>& others,
		const Transform3& transform,
		bool isNormalFlipped) :
		Surface3(transform, isNormalFlipped), m_surfaces(others)
	{
		// Do nothing
	}

	SurfaceSet3::SurfaceSet3(const SurfaceSet3& other) :
		Surface3(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	size_t SurfaceSet3::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const Surface3Ptr& SurfaceSet3::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void SurfaceSet3::AddSurface(const Surface3Ptr& surface)
	{
		m_surfaces.push_back(surface);
	}

	Vector3D SurfaceSet3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		Vector3D result(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
		
		double minimum_distance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			Vector3D localClosetPoint = surface->ClosestPoint(otherPoint);
			double localDistance = surface->ClosestDistance(otherPoint);

			if (localDistance < minimum_distance)
			{
				result = localClosetPoint;
				minimum_distance = localDistance;
			}
		}

		return result;
	}

	Vector3D SurfaceSet3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		Vector3D result(1, 0, 0);

		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			Vector3D localClosestNormal = surface->ClosestNormal(otherPoint);
			double localDistance = surface->ClosestDistance(otherPoint);

			if (localDistance < minimumDistance)
			{
				result = localClosestNormal;
				minimumDistance = localDistance;
			}
		}

		return result;
	}

	double SurfaceSet3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			double localDistance = surface->ClosestDistance(otherPoint);
			minimumDistance = std::min(minimumDistance, localDistance);
		}

		return minimumDistance;
	}

	bool SurfaceSet3::IntersectsLocal(const Ray3D& ray) const
	{
		for (const auto& surface : m_surfaces)
		{
			if (surface->Intersects(ray))
				return true;
		}

		return false;
	}

	SurfaceRayIntersection3 SurfaceSet3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		SurfaceRayIntersection3 intersection;
		double min = std::numeric_limits<double>::min();

		for (const auto& surface : m_surfaces)
		{
			SurfaceRayIntersection3 localResult = surface->ClosestIntersection(ray);

			if (localResult.isIntersecting && localResult.t < min)
			{
				intersection = localResult;
				min = localResult.t;
			}
		}

		return intersection;
	}

	BoundingBox3D SurfaceSet3::BoundingBoxLocal() const
	{
		BoundingBox3D bbox;

		for (const auto& surface : m_surfaces)
		{
			bbox.Merge(surface->BoundingBox());
		}

		return bbox;
	}
	
	SurfaceSet3::Builder SurfaceSet3::GetBuilder()
	{
		return Builder();
	}

	SurfaceSet3::Builder& SurfaceSet3::Builder::WithSurfaces(const std::vector<Surface3Ptr>& others)
	{
		m_surfaces = others;
		return *this;
	}

	SurfaceSet3 SurfaceSet3::Builder::Build() const
	{
		return SurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped);
	}

	SurfaceSet3Ptr SurfaceSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<SurfaceSet3>(new SurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped),
			[](SurfaceSet3* obj) {
			delete obj;
		});
	}
}