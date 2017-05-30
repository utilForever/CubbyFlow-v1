/*************************************************************************
> File Name: SPHStdKernel3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Standard 3-D SPH kernel function object.
> Created Time: 2017/05/30
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/

// Adopted from the sample code of:
// Bart Adams and Martin Wicke,
// "Meshless Approximation Methods and Applications in Physics Based Modeling
// and Animation", Eurographics 2009 Tutorial

#ifndef CUBBYFLOW_SPH_STD_KERNEL3_H
#define CUBBYFLOW_SPH_STD_KERNEL3_H

#include <Vector/Vector3.h>

namespace CubbyFlow
{
	//!
	//! \brief Standard 3-D SPH kernel function object.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!     "Particle-based fluid simulation for interactive applications."
	//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!     animation. Eurographics Association, 2003.
	//!
	struct SphStdKernel3
	{
		//! Kernel radius.
		double h;

		//! Square of the kernel radius.
		double h2;

		//! Cubic of the kernel radius.
		double h3;

		//! Fifth-power of the kernel radius.
		double h5;

		//! Constructs a kernel object with zero radius.
		SphStdKernel3();

		//! Constructs a kernel object with given radius.
		explicit SphStdKernel3(double kernelRadius);

		//! Copy constructor
		SphStdKernel3(const SphStdKernel3& other);

		//! Returns kernel function value at given distance.
		double operator()(double distance) const;

		//! Returns the first derivative at given distance.
		double FirstDerivative(double distance) const;

		//! Returns the gradient at a point.
		Vector3D Gradient(const Vector3D& point) const;

		//! Returns the gradient at a point defined by distance and direction.
		Vector3D Gradient(double distance, const Vector3D& direction) const;

		//! Returns the second derivative at given distance.
		double SecondDerivative(double distance) const;
	};

	//!
	//! \brief Spiky 3-D SPH kernel function object.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!     "Particle-based fluid simulation for interactive applications."
	//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!     animation. Eurographics Association, 2003.
	//!
	struct SphSpikyKernel3
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
		SphSpikyKernel3();

		//! Constructs a kernel object with given radius.
		explicit SphSpikyKernel3(double kernelRadius);

		//! Copy constructor
		SphSpikyKernel3(const SphSpikyKernel3& other);

		//! Returns kernel function value at given distance.
		double operator()(double distance) const;

		//! Returns the first derivative at given distance.
		double FirstDerivative(double distance) const;

		//! Returns the gradient at a point.
		Vector3D Gradient(const Vector3D& point) const;

		//! Returns the gradient at a point defined by distance and direction.
		Vector3D Gradient(double distance, const Vector3D& direction) const;

		//! Returns the second derivative at given distance.
		double SecondDerivative(double distance) const;
	};
}

#endif