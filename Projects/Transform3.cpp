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

	Vector2D Transform3::ToLocal(const Vector3D& pointInWorld) const
	{
		return Mul(pointInWorld - m_translation);
	}

	Vector2D Transform3::ToLocalDirection(const Vector2D& dirInWorld) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(dirInWorld);
	}

	Ray2D Transform3::ToLocal(const Ray2D& rayInWorld) const
	{
		return Ray2D(ToLocal(rayInWorld.origin), ToLocal(rayInWorld.direction));
	}

	BoundingBox2D Transform3::ToLocal(const BoundingBox2D& bboxInWorld) const
	{

	}

	Vector2D Transform3::ToWorld(const Vector2D& pointInLocal) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(pointInLocal + m_translation);
	}

	Vector2D Transform3::ToWorldDirection(const Vector2D& dirInLocal) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(dirInLocal);
	}

	Ray2D Transform3::ToWorld(const Ray2D& rayInLocal) const
	{
		return Ray2D(ToWorld(rayInLocal.origin), ToWorld(rayInLocal.direction));
	}

	BoundingBox2D Transform3::ToWorld(const BoundingBox2D& bboxInLocal) const
	{

	}
}