/*************************************************************************
> File Name: GridFractionalBoundaryConditionSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Fractional 2-D boundary condition solver for grids.
> Created Time: 2017/08/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER2_H
#define CUBBYFLOW_GRID_FRACTIONAL_BOUNDARY_CONDITION_SOLVER2_H

#include <Core/Field/CustomVectorField2.h>
#include <Core/Grid/CellCenteredScalarGrid2.h>
#include <Core/Solver/Grid/GridBoundaryConditionSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief Fractional 2-D boundary condition solver for grids.
	//!
	//! This class constrains the velocity field by projecting the flow to the
	//! signed-distance field representation of the collider. This implementation
	//! should pair up with GridFractionalSinglePhasePressureSolver2 to provide
	//! sub-grid resolution velocity projection.
	//!
	class GridFractionalBoundaryConditionSolver2 : public GridBoundaryConditionSolver2
	{
	public:
		//! Default constructor.
		GridFractionalBoundaryConditionSolver2();

		//! Default destructor.
		virtual ~GridFractionalBoundaryConditionSolver2();

		//!
		//! Constrains the velocity field to conform the collider boundary.
		//!
		//! \param velocity Input and output velocity grid.
		//! \param extrapolationDepth Number of inner-collider grid cells that
		//!     velocity will get extrapolated.
		//!
		void ConstrainVelocity(
			FaceCenteredGrid2* velocity,
			unsigned int extrapolationDepth = 5) override;

		//! Returns the signed distance field of the collider.
		ScalarField2Ptr GetColliderSDF() const override;

		//! Returns the velocity field of the collider.
		VectorField2Ptr GetColliderVelocityField() const override;

	protected:
		//! Invoked when a new collider is set.
		void OnColliderUpdated(
			const Size2& gridSize,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin) override;

	private:
		CellCenteredScalarGrid2Ptr m_colliderSDF;
		CustomVectorField2Ptr m_colliderVel;
	};

	//! Shared pointer type for the GridFractionalBoundaryConditionSolver2.
	using GridFractionalBoundaryConditionSolver2Ptr = std::shared_ptr<GridFractionalBoundaryConditionSolver2>;
}

#endif