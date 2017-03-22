/*************************************************************************
> File Name: CustomScalarField2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 2-D scalar field with custom field function.
> Created Time: 2017/03/16
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Field/CustomScalarField2.h>

namespace CubbyFlow
{
	CustomScalarField2::CustomScalarField2(
		const std::function<double(const Vector2D&)>& customFunction, 
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomScalarField2::CustomScalarField2(
		const std::function<double(const Vector2D&)>& customFunction,
		const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_customGradientFunction(customGradientFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomScalarField2::CustomScalarField2(
		const std::function<double(const Vector2D&)>& customFunction,
		const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
		const std::function<double(const Vector2D&)>& customLaplacianFunction) :
		m_customFunction(customFunction),
		m_customGradientFunction(customGradientFunction),
		m_customLaplacianFunction(customLaplacianFunction),
		m_resolution(1e-3)
	{
		// Do nothing
	}

	double CustomScalarField2::Sample(const Vector2D& x) const 
	{
		return m_customFunction(x);
	}

	std::function<double(const Vector2D&)> CustomScalarField2::Sampler() const
	{
		return m_customFunction;
	}

	Vector2D CustomScalarField2::Gradient(const Vector2D& x) const
	{
		return Vector2D();
	}

	double CustomScalarField2::Laplacian(const Vector2D& x) const
	{

	}

	CustomScalarField2::Builder CustomScalarField2::Builder()
	{
		return CustomScalarField2::Builder();
	}

	CustomScalarField2::Builder CustomScalarField2::Builder::WithFunction(const std::function<double(const Vector2D&)>& func)
	{

	}
}