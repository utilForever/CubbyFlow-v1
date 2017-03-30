/*************************************************************************
> File Name: Transform2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Represents 2-D rigid body transform.
> Created Time: 2017/03/30
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Transform\Transform2.h>

namespace CubbyFlow
{
	Transform2::Transform2()
	{

	}

	Transform2::Transform2(const Vector2D& translation, double orientation)
	{
		m_translation = translation;
		m_orientation = orientation;
	}

	const Vector2D& Transform2::Translation() const
	{
		return m_translation;
	}

	void Transform2::SetTranslation(const Vector2D& translation)
	{
		m_translation = translation;
	}

	void Transform2::SetOrientation(double orientation)
	{
		m_orientation = orientation;
	}

	Vector2D Transform2::ToLocal(const Vector2D& pointInWorld) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(pointInWorld-m_translation);
	}

	Vector2D Transform2::ToLocalDirection(const Vector2D& dirInWorld) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(dirInWorld);
	}

	Ray2D Transform2::ToLocal(const Ray2D& rayInWorld) const
	{
		return Ray2D(ToLocal(rayInWorld.origin), ToLocal(rayInWorld.direction));
	}

	BoundingBox2D Transform2::ToLocal(const BoundingBox2D& bboxInWorld) const
	{
		
	}

	Vector2D Transform2::ToWorld(const Vector2D& pointInLocal) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(pointInLocal + m_translation);
	}

	Vector2D Transform2::ToWorldDirection(const Vector2D& dirInLocal) const
	{
		return Matrix2x2<double>::MakeRotationMatrix(m_orientation).Mul(dirInLocal);
	}

	Ray2D Transform2::ToWorld(const Ray2D& rayInLocal) const
	{
		return Ray2D(ToWorld(rayInLocal.origin), ToWorld(rayInLocal.direction));
	}

	BoundingBox2D Transform2::ToWorld(const BoundingBox2D& bboxInLocal) const
	{

	}
}