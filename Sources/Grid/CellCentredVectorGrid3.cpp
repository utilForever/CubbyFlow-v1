/*************************************************************************
> File Name: CellCenteredVectorGrid3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D Cell-centered Vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Grid/CellCenteredVectorGrid3.h>

namespace CubbyFlow
{
	CellCenteredVectorGrid3::CellCenteredVectorGrid3()
	{
		// Do nothing
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(
		size_t resolutionX, size_t resolutionY, size_t resolutionZ,
		double gridSpacingX, double gridSpacingY,
		double originX, double originY, double originZ,
		double initialValueU, double initalValueV, double initalValZ)
	{
		Resize(
			resolutionX, resolutionY, resolutionZ,
			gridSpacingX, gridSpacingY, gridSpacingZ,
			originX, originY, originZ, 
			initialValueU, initalValueV, initalValueW);
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& origin,
		const Vector3D& initialValue)
	{
		Resize(resolution, gridSpacing, origin, initialValue);
	}

	CellCenteredVectorGrid3::CellCenteredVectorGrid3(const CellCenteredVectorGrid3& other)
	{
		Set(other);
	}

	Size3 CellCenteredVectorGrid3::GetDataSize() const
	{
		// The size of the data should be the same as the grid resolution.
		return Resolution();
	}

	Vector3D CellCenteredVectorGrid3::GetDataOrigin() const
	{
		return Origin() + 0.5 * GridSpacing();
	}

	std::shared_ptr<VectorGrid3> CellCenteredVectorGrid3::Clone() const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(*this), [](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	void CellCenteredVectorGrid3::Swap(Grid3* other)
	{
		CellCenteredVectorGrid3* sameType = dynamic_cast<CellCenteredVectorGrid3*>(other);
		if (sameType != nullptr)
		{
			SwapVectorGrid(sameType);
		}
	}

	void CellCenteredVectorGrid3::Set(const CellCenteredVectorGrid3& other)
	{
		SetVectorGrid(other);
	}

	CellCenteredVectorGrid3& CellCenteredVectorGrid3::operator=(const CellCenteredVectorGrid3& other)
	{
		Set(other);
		return *this;
	}

	CellCenteredVectorGrid3::Builder CellCenteredVectorGrid3::GetBuilder()
	{
		return Builder();
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithResolution(const Size3& resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithResolution(size_t resolutionX, size_t resolutionY)
	{
		m_resolution.x = resolutionX;
		m_resolution.y = resolutionY;
		m_resolution.z = resolutionZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithGridSpacing(double gridSpacingX, double gridSpacingY)
	{
		m_gridSpacing.x = gridSpacingX;
		m_gridSpacing.y = gridSpacingY;
		m_gridSpacing.z = gridSpacingZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(const Vector3D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(double gridOriginX, double gridOriginY)
	{
		m_gridOrigin.x = gridOriginX;
		m_gridOrigin.y = gridOriginY;
		m_gridOrigin.z = gridOriginZ;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithInitialValue(const Vector3D& initalVal)
	{
		m_initialVal = initialVal;
		return *this;
	}

	CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithInitialValue(double initalValU, double initalValW)
	{
		m_initialVal.x = initalValU;
		m_initialVal.y = initalValV;
		m_initialVal.z = initalValW;
		return *this;
	}

	CellCenteredVectorGrid3 CellCenteredVectorGrid3::Builder::Build() const
	{
		return CellCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal);
	}

	CellCenteredVectorGrid3Ptr CellCenteredVectorGrid3::Builder::MakeShared() const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(m_resolution, m_gridSpacing, m_gridOrigin, m_initialVal),
			[](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}

	VectorGrid3Ptr CellCenteredVectorGrid3::Builder::Build(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& gridOrigin,
		const Vector3D& initialVal) const
	{
		return std::shared_ptr<CellCenteredVectorGrid3>(
			new CellCenteredVectorGrid3(resolution, gridSpacing, gridOrigin, initialVal),
			[](CellCenteredVectorGrid3* obj)
		{
			delete obj;
		});
	}
}