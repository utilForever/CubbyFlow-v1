/*************************************************************************
> File Name: ImplicitSurfaceSet3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D implicit surface set.
> Created Time: 2017/04/18
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Surface/Implicit/ImplicitSurfaceSet3.h>
#include <Surface/Implicit/SurfaceToImplicit3.h>

namespace CubbyFlow
{
	ImplicitSurfaceSet3::ImplicitSurfaceSet3()
	{
		// Do nothing
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(
		const std::vector<ImplicitSurface3Ptr>& surfaces,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped), m_surfaces(surfaces)
	{
		// Do nothing
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(
		const std::vector<Surface3Ptr>& surfaces,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped)
	{
		for (const auto& surface : surfaces)
		{
			AddExplicitSurface(surface);
		}
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(const ImplicitSurfaceSet3& other) :
		ImplicitSurface3(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	size_t ImplicitSurfaceSet3::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const ImplicitSurface3Ptr& ImplicitSurfaceSet3::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void ImplicitSurfaceSet3::AddExplicitSurface(const Surface3Ptr& surface)
	{
		m_surfaces.push_back(std::make_shared<SurfaceToImplicit3>(surface));
	}

	void ImplicitSurfaceSet3::AddSurface(const ImplicitSurface3Ptr& surface)
	{
		m_surfaces.push_back(surface);
	}

	Vector3D ImplicitSurfaceSet3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		Vector3D result(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			Vector3D localClosestPoint = surface->ClosestPoint(otherPoint);
			double localDistance = surface->ClosestDistance(otherPoint);

			if (localDistance < minimumDistance)
			{
				result = localClosestPoint;
				minimumDistance = localDistance;
			}
		}

		return result;
	}

	double ImplicitSurfaceSet3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		double minimumDistance = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			double localDistance = surface->ClosestDistance(otherPoint);
			minimumDistance = std::min(localDistance, minimumDistance);
		}

		return minimumDistance;
	}

	Vector3D ImplicitSurfaceSet3::ClosestNormalLocal(const Vector3D& otherPoint) const
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

	bool ImplicitSurfaceSet3::IntersectsLocal(const Ray3D& ray) const
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

	SurfaceRayIntersection3 ImplicitSurfaceSet3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		SurfaceRayIntersection3 intersection;
		double min = std::numeric_limits<double>::max();

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

	BoundingBox3D ImplicitSurfaceSet3::BoundingBoxLocal() const
	{
		BoundingBox3D bbox;

		for (const auto& surface : m_surfaces)
		{
			bbox.Merge(surface->BoundingBox());
		}

		return bbox;
	}

	double ImplicitSurfaceSet3::SignedDistanceLocal(const Vector3D& otherPoint) const
	{
		double sdf = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			sdf = std::min(sdf, surface->SignedDistance(otherPoint));
		}

		return sdf;
	}

	ImplicitSurfaceSet3::Builder ImplicitSurfaceSet3::GetBuilder()
	{
		return Builder();
	}

	ImplicitSurfaceSet3::Builder& ImplicitSurfaceSet3::Builder::WithSurfaces(
		const std::vector<ImplicitSurface3Ptr>& surfaces)
	{
		m_surfaces = surfaces;
		return *this;
	}

	ImplicitSurfaceSet3::Builder& ImplicitSurfaceSet3::Builder::WithExplicitSurfaces(
		const std::vector<Surface3Ptr>& surfaces)
	{
		m_surfaces.clear();

		for (const auto& surface : surfaces)
		{
			m_surfaces.push_back(std::make_shared<SurfaceToImplicit3>(surface));
		}

		return *this;
	}

	ImplicitSurfaceSet3 ImplicitSurfaceSet3::Builder::Build() const
	{
		return ImplicitSurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped);
	}

	ImplicitSurfaceSet3Ptr ImplicitSurfaceSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<ImplicitSurfaceSet3>(new ImplicitSurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped),
			[](ImplicitSurfaceSet3* obj)
		{
			delete obj;
		});
	}
}