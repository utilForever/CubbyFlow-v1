/*************************************************************************
> File Name: GridFluidSolver2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for grid-based 2-D fluid solver.
> Created Time: 2017/08/10
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/

#include <Solver/Grid/GridFluidSolver2.h>

namespace CubbyFlow
{
	GridFluidSolver2::GridFluidSolver2()
	{
		// Do nothing
	}

	GridFluidSolver2::GridFluidSolver2(
		const Size2& resolution, 
		const Vector2D& gridSpacing, 
		const Vector2D& gridOrigin) 
	{
		_grids = std::make_shared<GridSystemData2>();
		_collider = std::make_shared<Collider2>();
		_emitter = std::make_shared<GridEmitter2>();
		
		_advectionSolver = std::make_shared<AdvectionSolver2>();
		_diffusionSolver = std::make_shared<GridDiffusionSolver2>();
		_pressureSolver = std::make_shared<GridPressureSolver2>();
		_boundaryConditionSolver = std::make_shared<GridBoundaryConditionSolver2>();

		resizeGrid(resolution, gridSpacing, gridOrigin);
	}

	GridFluidSolver2::~GridFluidSolver2()
	{
		// Do nothing
	}

	const Vector2D& GridFluidSolver2::gravity() const
	{
		return _gravity;
	}

	void GridFluidSolver2::setGravity(const Vector2D& newGravity)
	{
		_gravity = newGravity;
	}

	double GridFluidSolver2::viscosityCoefficient() const
	{
		return _viscosityCoefficient;
	}

	void GridFluidSolver2::setViscosityCoefficient(double newValue)
	{
		_viscosityCoefficient = std::clamp(newValue, 0., std::numeric_limits<double>::max());
	}

	double GridFluidSolver2::cfl(double timeIntervalInSeconds) const
	{

	}

	double GridFluidSolver2::maxCfl() const
	{
		return _maxCfl;
	}

	void GridFluidSolver2::setMaxCfl(double newCfl)
	{
		_maxCfl = newCfl;
	}

	const AdvectionSolver2Ptr& GridFluidSolver2::advectionSolver() const
	{
		return _advectionSolver;
	}

	void GridFluidSolver2::setAdvectionSolver(const AdvectionSolver2Ptr& newSolver)
	{
		_advectionSolver = newSolver;
	}

	const GridDiffusionSolver2Ptr& GridFluidSolver2::diffusionSolver() const
	{
		return _diffusionSolver;
	}

	void GridFluidSolver2::setDiffusionSolver(const GridDiffusionSolver2Ptr& newSolver)
	{
		_diffusionSolver = newSolver;
	}

	const GridPressureSolver2Ptr& GridFluidSolver2::pressureSolver() const
	{
		return _pressureSolver;
	}

	void GridFluidSolver2::setPressureSolver(const GridPressureSolver2Ptr& newSolver)
	{
		_pressureSolver = newSolver;
	}

	int GridFluidSolver2::closedDomainBoundaryFlag() const
	{
		return _closedDomainBoundaryFlag;
	}

	void GridFluidSolver2::setClosedDomainBoundaryFlag(int flag)
	{
		_closedDomainBoundaryFlag = flag;
	}

	const GridSystemData2Ptr& GridFluidSolver2::gridSystemData() const
	{
		return _grids;
	}

	void GridFluidSolver2::resizeGrid(
		const Size2& newSize,
		const Vector2D& newGridSpacing,
		const Vector2D& newGridOrigin)
	{
		_grids->Resize(newSize, newGridSpacing, newGridOrigin);
	}

	Size2 GridFluidSolver2::resolution() const
	{
		return _grids->GetResolution();
	}

	Vector2D GridFluidSolver2::gridSpacing() const
	{
		return _grids->GetGridSpacing();
	}

	Vector2D GridFluidSolver2::gridOrigin() const
	{
		return _grids->GetOrigin();
	}

	const FaceCenteredGrid2Ptr& GridFluidSolver2::velocity() const
	{
		return _grids->GetVelocity();
	}

	const Collider2Ptr& GridFluidSolver2::collider() const
	{
		return _collider;
	}

	void GridFluidSolver2::setCollider(const Collider2Ptr& newCollider)
	{
		_collider = newCollider;
	}

	const GridEmitter2Ptr& GridFluidSolver2::emitter() const
	{
		return _emitter;
	}

	void GridFluidSolver2::setEmitter(const GridEmitter2Ptr& newEmiiter)
	{
		_emitter = newEmiiter;
	}

	GridFluidSolver2::Builder GridFluidSolver2::builder()
	{
		return builder();
	}


}