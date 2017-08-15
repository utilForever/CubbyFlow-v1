/*************************************************************************
> File Name: GridFluidSolver2.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for grid-based 2-D fluid solver.
> Created Time: 2017/08/10
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/

#include <Solver/Grid/GridFluidSolver2.h>
#include <Utils/Timer.h>
#include <Utils/Logger.h>

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
		auto u = velocity()->GetUAccessor();
		Vector2D maxU(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());

		u.ForEach([&](Vector2D vel) {
			maxU.x = std::max(maxU.x, std::abs(vel.x));
			maxU.y = std::max(maxU.y, std::abs(vel.y));
		});

		return std::max(maxU.x * timeIntervalInSeconds / gridSpacing().x, maxU.y * timeIntervalInSeconds / gridSpacing().y);
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

	void GridFluidSolver2::OnInitialize()
	{
		Timer timer;
		updateCollider(0.0);
		CUBBYFLOW_INFO << "Update collider took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		updateEmitter(0.0);
		CUBBYFLOW_INFO << "Update emitter took "
			<< timer.DurationInSeconds() << " seconds";
	}

	void GridFluidSolver2::OnAdvanceTimeStep(double timeIntervalInSeconds)
	{
		beginAdvanceTimeStep(timeIntervalInSeconds);

		Timer timer;
		computeExternalForces(timeIntervalInSeconds);

		CUBBYFLOW_INFO << "Compute external forces took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		computeAdvection(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "Compute advection took "
			<< timer.DurationInSeconds() << " seconds";
		
		timer.Reset();
		computePressure(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "Compute pressure took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		computeGravity(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "Compute gravity took "
			<< timer.DurationInSeconds() << " seconds";

		timer.Reset();
		computeViscosity(timeIntervalInSeconds);
		CUBBYFLOW_INFO << "Compute viscosity took "
			<< timer.DurationInSeconds() << " seconds";

		endAdvanceTimeStep(timeIntervalInSeconds);
	}

	unsigned int GridFluidSolver2::NumberOfSubTimeSteps(double timeIntervalInSeconds) const
	{
		return std::ceil(cfl(timeIntervalInSeconds) / maxCfl());
	}

	void GridFluidSolver2::onBeginAdvanceTimeStep(double timeIntervalInSeconds)
	{
		// Do nothing
	}

	void GridFluidSolver2::onEndAdvanceTimeStep(double timeIntervalInSeconds)
	{
		// Do nothing
	}

	void GridFluidSolver2::computeExternalForces(double timeIntervalInSeconds)
	{
	}

	void GridFluidSolver2::computeGravity(double timeIntervalInSeconds)
	{
		if (_gravity.LengthSquared() > 1e-4)
		{
			auto vel = velocity();
			auto u = vel->GetUAccessor();
			auto v = vel->GetVAccessor();
			
			if(std::abs(_gravity.x) > 1e-4)
			{
				vel->ForEachUIndex([&](size_t i, size_t j) {
					u(i, j) += timeIntervalInSeconds * _gravity.x;
				});

				vel->ForEachVIndex([&](size_t i, size_t j) {
					v(i, j) += timeIntervalInSeconds * _gravity.y;
				});
			}

			applyBoundaryCondition();
		}
	}

	void GridFluidSolver2::computeViscosity(double timeIntervalInSeconds)
	{
		if (_diffusionSolver != nullptr && _viscosityCoefficient > 1e-4)
		{
			auto vel = velocity();
			auto vel0 = std::dynamic_pointer_cast<FaceCenteredGrid2>(vel->Clone());

			_diffusionSolver->Solve(*vel0,
				_viscosityCoefficient,
				timeIntervalInSeconds,
				vel.get(),
				colliderSdf(),
				*fluidSdf());
			applyBoundaryCondition();
		}
	}

	void GridFluidSolver2::computeAdvection(double timeIntervalInSeconds)
	{
		auto vel = velocity();
		if (_advectionSolver != nullptr)
		{
			auto vel0 = std::dynamic_pointer_cast<FaceCenteredGrid2>(vel->Clone());
			_advectionSolver->Advect(
				*vel0,
				*vel0,
				timeIntervalInSeconds,
				vel.get(),
				colliderSdf());
			applyBoundaryCondition();
		}
	}

	void GridFluidSolver2::computePressure(double timeIntervalInSeconds)
	{
		auto vel = velocity();
		if (_pressureSolver != nullptr)
		{
			auto vel0 = std::dynamic_pointer_cast<FaceCenteredGrid2>(vel->Clone());
			_pressureSolver->Solve(*vel0, timeIntervalInSeconds, vel.get(),,, *fluidSdf());
		}
	}
}