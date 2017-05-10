/*************************************************************************
> File Name: ArraySamplers2-Impl.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 2-D nearest array sampler class.
> Created Time: 2017/05/01
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_SAMPLERS2_IMPL_H
#define CUBBYFLOW_ARRAY_SAMPLERS2_IMPL_H

namespace CubbyFlow
{
	template <typename T, typename R>
	NearestArraySampler<T, R, 2>::NearestArraySampler(
		const ConstArrayAccessor2<T>& accessor,
		const Vector2<R>& gridSpacing,
		const Vector2<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}

	template <typename T, typename R>
	NearestArraySampler<T, R, 2>::NearestArraySampler(const NearestArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T NearestArraySampler<T, R, 2>::operator()(const Vector2<R>& pt) const
	{
		ssize_t i, j;
		R fx, fy;
		
		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		
		R normalized = (*pt - m_origin) / m_gridSpacing;

		Vector2<T> iSize = static_cast<ssize_t>(m_accessor.Size());

		GetBarycentric(normalizedX, 0, iSize, &i, &fx);

		i = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);

		return m_accessor[i];
	}

	template <typename T, typename R>
	void NearestArraySampler<T, R, 2>::GetCoordinate(
		const Vector2<R>& pt,
		Point2UI* index) const
	{
		R fx;

		assert(m_gridSpacing > std::numeric_limits<R>::epsilon());
		R normalizedX = (x - m_origin) / m_gridSpacing;

		ssize_t iSize = static_cast<ssize_t>(m_accessor.Size());

		ssize_t _i;
		GetBarycentric(normalizedX, 0, iSize, &_i, &fx);

		*i = std::min(static_cast<ssize_t>(_i + fx + 0.5), iSize - 1);
	}

	template <typename T, typename R>
	std::function<T(const Vector2<R>&)> NearestArraySampler<T, R, 2>::Functor() const
	{
		NearestArraySampler sampler(*this);
		return std::bind(&NearestArraySampler::operator(), sampler, std::placeholders::_1);
	}

	template <typename T, typename R>
	LinearArraySampler<T, R, 2>::LinearArraySampler(
		const ConstArrayAccessor2<T>& accessor,
		const Vector2<R>& gridSpacing,
		const Vector2<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}

	template <typename T, typename R>
	LinearArraySampler<T, R, 2>::LinearArraySampler(const LinearArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T LinearArraySampler<T, R, 2>::operator()(const Vector2<R>& pt) const
	{
		ssize_t i;
		R fx;

		assert(m_gridSpacing > std::numeric_limits<R>::epsilon());
		R normalizedX = (x - m_origin) / m_gridSpacing;

		ssize_t iSize = static_cast<ssize_t>(m_accessor.Size());

		GetBarycentric(normalizedX, 0, iSize, &i, &fx);

		ssize_t ip1 = std::min(i + 1, iSize - 1);

		return Lerp(m_accessor[i], m_accessor[ip1], fx);
	}

	template <typename T, typename R>
	void LinearArraySampler<T, R, 2>::GetCoordinatesAndWeights(
		const Vector2<R>& pt, 
		std::array<Point2UI, 4>* indices,
		std::array<R, 4>* weights) const
	{
		ssize_t i;
		R fx;

		assert(m_gridSpacing > std::numeric_limits<R>::epsilon());
		R normalizedX = (x - m_origin) / m_gridSpacing;

		ssize_t iSize = static_cast<ssize_t>(m_accessor.Size());

		GetBarycentric(normalizedX, 0, iSize, &i, &fx);

		ssize_t ip1 = std::min(i + 1, iSize - 1);

		*i0 = i;
		*i1 = ip1;
		*weight0 = 1 - fx;
		*weight1 = fx;
	}

	template <typename T, typename R>
	std::function<T(const Vector2<R>&)> LinearArraySampler<T, R, 2>::Functor() const
	{
		LinearArraySampler sampler(*this);
		return std::bind(&LinearArraySampler::operator(), sampler, std::placeholders::_1);
	}

	template <typename T, typename R>
	CubicArraySampler<T, R, 2>::CubicArraySampler(
		const ConstArrayAccessor2<T>& accessor,
		const Vector2<R>& gridSpacing,
		const Vector2<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}

	template <typename T, typename R>
	CubicArraySampler<T, R, 2>::CubicArraySampler(const CubicArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T CubicArraySampler<T, R, 2>::operator(const Vector2<R>& pt) const
	{
		ssize_t i;
		ssize_t iSize = static_cast<ssize_t>(m_accessor.Size());
		R fx;

		assert(m_gridSpacing > std::numeric_limits<R>::epsilon());
		R normalizedX = (x - m_origin) / m_gridSpacing;

		GetBarycentric(normalizedX, 0, iSize, &i, &fx);

		ssize_t im1 = std::max(i - 1, ZERO_SSIZE);
		ssize_t ip1 = std::min(i + 1, iSize - 1);
		ssize_t ip2 = std::min(i + 2, iSize - 1);

		return MonotonicCatmullRom(
			m_accessor[im1],
			m_accessor[i],
			m_accessor[ip1],
			m_accessor[ip2],
			fx);
	}

	template <typename T, typename R>
	std::function<T(const Vector2<R>&)> CubicArraySampler<T, R, 2>::Functor() const
	{
		CubicArraySampler sampler(*this);
		return std::bind(&CubicArraySampler::operator(), sampler, std::placeholders::_1);
	}
}

#endif