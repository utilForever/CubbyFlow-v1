/*************************************************************************
> File Name: Transform3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Represents 3-D rigid body transform.
> Created Time: 2017/03/30
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Transform\Transform3.h>

namespace CubbyFlow
{
	Transform3::Transform3()
	{
		// Do nothing
	}

	Transform3::Transform3(const Vector3D& translation, const QuaternionD& orientation)
	{
		m_translation = translation;
		m_orientation = orientation;
	}

	const Vector3D& Transform3::Translation() const
	{
		return m_translation;
	}

	void Transform3::SetTranslation(const Vector3D& translation)
	{
		m_translation = translation;
	}

	void Transform3::SetOrientation(const QuaternionD& orientation)
	{
		m_orientation = orientation;
	}

	Vector3D Transform3::ToLocal(const Vector3D& pointInWorld) const
	{
		return m_orientation.Inverse.RMul(m_orientation*(pointInWorld - m_translation));
	}

	Vector3D Transform3::ToLocalDirection(const Vector3D& dirInWorld) const
	{
		return m_orientation.Inverse.RMul(m_orientation*(dirInWorld));
	}

	Ray3D Transform3::ToLocal(const Ray3D& rayInWorld) const
	{
		return Ray3D(ToLocal(rayInWorld.origin), ToLocal(rayInWorld.direction));
	}

	BoundingBox3D Transform3::ToLocal(const BoundingBox3D& bboxInWorld) const
	{
	}

	Vector3D Transform3::ToWorld(const Vector3D& pointInLocal) const
	{
		return m_orientation.Inverse.RMul(m_orientation*(pointInLocal + m_translation));
	}

	Vector3D Transform3::ToWorldDirection(const Vector3D& dirInLocal) const
	{
		return m_orientation.Inverse.RMul(m_orientation*(dirInLocal));
	}

	Ray3D Transform3::ToWorld(const Ray3D& rayInLocal) const
	{
		return Ray3D(ToWorld(rayInLocal.origin), ToWorld(rayInLocal.direction));
	}

	BoundingBox3D Transform3::ToWorld(const BoundingBox3D& bboxInLocal) const
	{

	}
}