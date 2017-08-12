/*************************************************************************
> File Name: FDMLinearSystem3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Linear system (Ax=b) for 3-D finite differencing.
> Created Time: 2017/08/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM3_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM3_H

#include <Array/Array3.h>

namespace CubbyFlow
{
	//! The row of FDMMatrix3 where row corresponds to (i, j, k) grid point.
	struct FDMMatrixRow3
	{
		//! Diagonal component of the matrix (row, row).
		double center = 0.0;

		//! Off-diagonal element where column refers to (i+1, j, k) grid point.
		double right = 0.0;

		//! Off-diagonal element where column refers to (i, j+1, k) grid point.
		double up = 0.0;

		//! OFf-diagonal element where column refers to (i, j, k+1) grid point.
		double front = 0.0;
	};

	//! Vector type for 3-D finite differencing.
	using FDMVector3 = Array3<double>;

	//! Matrix type for 3-D finite differencing.
	using FDMMatrix3 = Array3<FDMMatrixRow3>;

	//! Linear system (Ax=b) for 3-D finite differencing.
	struct FDMLinearSystem3
	{
		FDMMatrix3 A;
		FDMVector3 x, b;
	};

	//! BLAS operator wrapper for 3-D finite differencing.
	struct FDMBlas3
	{
		using ScalarType = double;
		using VectorType = FDMVector3;
		using MatrixType = FDMMatrix3;

		//! Sets entire element of given vector \p result with scalar \p s.
		static void Set(double s, FDMVector3* result);

		//! Copies entire element of given vector \p result with other vector \p v.
		static void Set(const FDMVector3& v, FDMVector3* result);

		//! Sets entire element of given matrix \p result with scalar \p s.
		static void Set(double s, FDMMatrix3* result);

		//! Copies entire element of given matrix \p result with other matrix \p v.
		static void Set(const FDMMatrix3& m, FDMMatrix3* result);

		//! Performs dot product with vector \p a and \p b.
		static double Dot(const FDMVector3& a, const FDMVector3& b);

		//! Performs ax + y operation where \p a is a matrix and \p x and \p y are
		//! vectors.
		static void AXPlusY(double a, const FDMVector3& x, const FDMVector3& y, FDMVector3* result);

		//! Performs matrix-vector multiplication.
		static void MVM(const FDMMatrix3& m, const FDMVector3& v, FDMVector3* result);

		//! Computes residual vector (b - ax).
		static void residual(const FDMMatrix3& a, const FDMVector3& x, const FDMVector3& b, FDMVector3* result);

		//! Returns L2-norm of the given vector \p v.
		static double L2Norm(const FDMVector3& v);

		//! Returns L-inf-norm of the given vector \p v.
		static double LInfNorm(const FDMVector3& v);
	};
}

#endif