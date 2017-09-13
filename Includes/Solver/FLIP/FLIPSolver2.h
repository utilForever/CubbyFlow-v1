/*************************************************************************
> File Name: FLIPSolver2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D Fluid-Implicit Particle (FLIP) implementation.
> Created Time: 2017/09/13
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FLIP_SOLVER2_H
#define CUBBYFLOW_FLIP_SOLVER2_H

#include <Solver/PIC/PICSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D Fluid-Implicit Particle (FLIP) implementation.
	//!
	//! This class implements 2-D Fluid-Implicit Particle (FLIP) solver from the
	//! SIGGRAPH paper, Zhu and Bridson 2005. By transfering delta-velocity field
	//! from grid to particles, the FLIP solver achieves less viscous fluid flow
	//! compared to the original PIC method.
	//!
	//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
	//!     ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
	//!
	class FLIPSolver2 : public PICSolver2
	{
	public:
		class Builder;

		//! Default constructor.
		FLIPSolver2();

		//! Constructs solver with initial grid size.
		FLIPSolver2(
			const Size2& resolution,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin);

		//! Default destructor.
		virtual ~FLIPSolver2();

		//! Returns builder fox FLIPSolver2.
		static Builder GetBuilder();

	protected:
		//! Transfers velocity field from particles to grids.
		void TransferFromParticlesToGrids() override;

		//! Transfers velocity field from grids to particles.
		void TransferFromGridsToParticles() override;

	private:
		Array2<float> m_uDelta;
		Array2<float> m_vDelta;
	};

	//! Shared pointer type for the FLIPSolver2.
	using FLIPSolver2Ptr = std::shared_ptr<FLIPSolver2>;
	
	//!
	//! \brief Front-end to create FLIPSolver2 objects step by step.
	//!
	class FLIPSolver2::Builder final : public GridFluidSolverBuilderBase2<FLIPSolver2::Builder>
	{
	public:
		//! Builds FLIPSolver2.
		FLIPSolver2 Build() const;

		//! Builds shared pointer of FLIPSolver2 instance.
		FLIPSolver2Ptr MakeShared() const;
	};
}

#endif