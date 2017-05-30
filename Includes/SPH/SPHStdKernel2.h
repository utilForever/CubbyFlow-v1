/*************************************************************************
> File Name: SPHStdKernel2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Standard 2-D SPH kernel function object.
> Created Time: 2017/05/30
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

// Adopted from the sample code of:
// Bart Adams and Martin Wicke,
// "Meshless Approximation Methods and Applications in Physics Based Modeling
// and Animation", Eurographics 2009 Tutorial

#ifndef CUBBYFLOW_SPH_STD_KERNEL2_H
#define CUBBYFLOW_SPH_STD_KERNEL2_H

#include <Vector/Vector2.h>

namespace CubbyFlow
{
	//!
	//! \brief Standard 2-D SPH kernel function object.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!     "Particle-based fluid simulation for interactive applications."
	//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!     animation. Eurographics Association, 2003.
	//!
	struct SphStdKernel2
	{
		//! Kernel radius.
		double h;

		//! Square of the kernel radius.
		double h2;

		//! Cubic of the kernel radius.
		double h3;

		//! Fourth-power of the kernel radius.
		double h4;

		//! Constructs a kernel object with zero radius.
		SphStdKernel2();

		//! Constructs a kernel object with given radius.
		explicit SphStdKernel2(double kernelRadius);

		//! Copy constructor
		SphStdKernel2(const SphStdKernel2& other);

		//! Returns kernel function value at given distance.
		double operator()(double distance) const;

		//! Returns the first derivative at given distance.
		double FirstDerivative(double distance) const;

		//! Returns the gradient at a point.
		Vector2D Gradient(const Vector2D& point) const;

		//! Returns the gradient at a point defined by distance and direction.
		Vector2D Gradient(double distance, const Vector2D& direction) const;

		//! Returns the second derivative at given distance.
		double SecondDerivative(double distance) const;
	};

	//!
	//! \brief Spiky 2-D SPH kernel function object.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!     "Particle-based fluid simulation for interactive applications."
	//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!     animation. Eurographics Association, 2003.
	//!
	struct SphSpikyKernel2
	{
		//! Kernel radius.
		double h;

		//! Square of the kernel radius.
		double h2;

		//! Cubic of the kernel radius.
		double h3;

		//! Fourth-power of the kernel radius.
		double h4;

		//! Fifth-power of the kernel radius.
		double h5;

		//! Constructs a kernel object with zero radius.
		SphSpikyKernel2();

		//! Constructs a kernel object with given radius.
		explicit SphSpikyKernel2(double kernelRadius);

		//! Copy constructor
		SphSpikyKernel2(const SphSpikyKernel2& other);

		//! Returns kernel function value at given distance.
		double operator()(double distance) const;

		//! Returns the first derivative at given distance.
		double FirstDerivative(double distance) const;

		//! Returns the gradient at a point.
		Vector2D Gradient(const Vector2D& point) const;

		//! Returns the gradient at a point defined by distance and direction.
		Vector2D Gradient(double distance, const Vector2D& direction) const;

		//! Returns the second derivative at given distance.
		double SecondDerivative(double distance) const;
	};
}

#endif