/*************************************************************************
> File Name: FdmBlas.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D Finite difference method basic linear algebra subprograms.
> Created Time: 2017/06/19
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_FDM_BLAS_H
#define CUBBYFLOW_FDM_BLAS_H

#include "Blas.h"
#include "Fdm.h"
namespace CubbyFlow
{
	//!
	//! \brief Fdm Blas class.
	//!
	//! This class implements some matrix-vector calculation in fdm.
	//!

	template<typename T, typename FdmMatrixRow3>
	class FdmBlas3
	{
		typedef double ScalarType;
		typedef FdmVector3 VectorType;
		typedef FdmMatrix3 MatrixType;

		// Sets the given scalar value to the output vector.
		static void set(double s, FdmVector3* result);

		// Sets the given vector to the output vector.
		static void set(const FdmVector3& v, FdmVector3* result);

		// Sets the given scalar value to the output matrix.
		static void set(const double s, FdmMatrix3* result);

		// Sets the given matrix to the output matrix.
		static void set(const FdmMatrix3& m, FdmMatrix3* result);

		// Performs dot product.
		static double dot(const FdmVector3& a, const FdmVector3& b);

		// Calculate a*x + y.
		static void axpy(
			double a,
			const FdmVector3& x,
			const FdmVector3& y,
			FdmVector3* result);

		// Performs matrix-vector multiplication.
		static void mvm(const FdmMatrix3& m, const FdmVector3& v, FdmVector3* result);

		// Calculate b - A*x.
		static void residual(
			const FdmMatrix3& a, 
			const FdmVector3& x, 
			const FdmVector3& b, 
			FdmVector3* result);

		// Returns the length of the vector.
		static double l2Norm(const FdmVector3& v);

		// Returns the absolute maximum value among the vector elements.
		static double lInfNorm(const FdmVector3& v);
	};
}

#endif