/*************************************************************************
> File Name: VectorField2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for 2-D vector field.
> Created Time: 2017/03/14
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Field\VectorField2.h>

namespace CubbyFlow
{
	VectorField2::VectorField2()
	{
		// Do nothing
	}

	VectorField2::~VectorField2()
	{
		// Do nothing
	}

	double VectorField2::Divergence(const Vector2D& x) const
	{
		return 0;
	}

	double VectorField2::Curl(const Vector2D& x) const
	{
		return 0;
	}

	std::function<Vector2D(const Vector2D&)> VectorField2::Sampler() const
	{
		std::function<Vector2D(const Vector2D&)> tmp;
		return tmp;
	}
}