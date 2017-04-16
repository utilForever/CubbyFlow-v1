/*************************************************************************
> File Name: ImplicitSurfaceSet2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D implicit surface set.
> Created Time: 2017/04/16
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Surface/Implicit/ImplicitSurfaceSet2.h>
#include <Surface/Implicit/SurfaceToImplicit2.h>

namespace CubbyFlow
{
	ImplicitSurfaceSet2::ImplicitSurfaceSet2()
	{
		// Do nothing
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(
		const std::vector<ImplicitSurface2Ptr>& surfaces,
		const Transform2& transform,
		bool isNormalFlipped) :
		ImplicitSurface2(transform, isNormalFlipped), m_surfaces(surfaces)
	{
		// Do nothing
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(
		const std::vector<Surface2Ptr>& surfaces,
		const Transform2& transform,
		bool isNormalFlipped) :
		ImplicitSurface2(transform, isNormalFlipped)
	{
		for (const auto& surface : surfaces)
		{
			AddExplicitSurface(surface);
		}
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other) :
		ImplicitSurface2(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	size_t ImplicitSurfaceSet2::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const ImplicitSurface2Ptr& ImplicitSurfaceSet2::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void ImplicitSurfaceSet2::AddExplicitSurface(const Surface2Ptr& surface)
	{
		m_surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
	}

	void ImplicitSurfaceSet2::AddSurface(const ImplicitSurface2Ptr& surface)
	{
		m_surfaces.push_back(surface);
	}

	Vector2D ImplicitSurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
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

	double ImplicitSurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
	{
		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			double localDistance = surface->ClosestDistance(otherPoint);
			minimumDistance = std::min(localDistance, minimumDistance);
		}

		return minimumDistance;
	}

	Vector2D ImplicitSurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
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

	bool ImplicitSurfaceSet2::IntersectsLocal(const Ray2D& ray) const
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

	SurfaceRayIntersection2 ImplicitSurfaceSet2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		SurfaceRayIntersection2 intersection;
		double min = std::numeric_limits<double>::max();

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

	BoundingBox2D ImplicitSurfaceSet2::BoundingBoxLocal() const
	{
		BoundingBox2D bbox;

		for (const auto& surface : m_surfaces)
		{
			bbox.Merge(surface->BoundingBox());
		}

		return bbox;
	}

	double ImplicitSurfaceSet2::SignedDistanceLocal(const Vector2D& otherPoint) const
	{
		double sdf = std::numeric_limits<double>::max();
		
		for (const auto& surface : m_surfaces)
		{
			sdf = std::min(sdf, surface->SignedDistance(otherPoint));
		}

		return sdf;
	}

	ImplicitSurfaceSet2::Builder ImplicitSurfaceSet2::GetBuilder()
	{
		return Builder();
	}
	
	ImplicitSurfaceSet2::Builder& ImplicitSurfaceSet2::Builder::WithSurfaces(
		const std::vector<ImplicitSurface2Ptr>& surfaces)
	{
		m_surfaces = surfaces;
		return *this;
	}

	ImplicitSurfaceSet2::Builder& ImplicitSurfaceSet2::Builder::WithExplicitSurfaces(
			const std::vector<Surface2Ptr>& surfaces)
	{
		m_surfaces.clear();

		for (const auto& surface : surfaces)
		{
			m_surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
		}

		return *this;
	}

	ImplicitSurfaceSet2 ImplicitSurfaceSet2::Builder::Build() const
	{
		return ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped);
	}

	ImplicitSurfaceSet2Ptr ImplicitSurfaceSet2::Builder::MakeShared() const
	{
		return std::shared_ptr<ImplicitSurfaceSet2>(new ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped),
			[](ImplicitSurfaceSet2* obj)
		{
			delete obj;
		});
	}
}