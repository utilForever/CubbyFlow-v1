/*************************************************************************
> File Name: FDMLinearSystem2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Linear system (Ax=b) for 2-D finite differencing.
> Created Time: 2017/08/12
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM2_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM2_H

#include <Array/Array2.h>

namespace CubbyFlow
{
	//! The row of FDMMatrix2 where row corresponds to (i, j) grid point.
	struct FDMMatrixRow2
	{
		//! Diagonal component of the matrix (row, row).
		double center = 0.0;

		//! Off-diagonal element where column refers to (i+1, j) grid point.
		double right = 0.0;

		//! Off-diagonal element where column refers to (i, j+1) grid point.
		double up = 0.0;
	};

	//! Vector type for 2-D finite differencing.
	using FDMVector2 = Array2<double>;

	//! Matrix type for 2-D finite differencing.
	using FDMMatrix2 = Array2<FDMMatrixRow2>;

	//! Linear system (Ax=b) for 2-D finite differencing.
	struct FDMLinearSystem2
	{
		FDMMatrix2 A;
		FDMVector2 x, b;

		void Clear();
	};

	//! BLAS operator wrapper for 2-D finite differencing.
	struct FDMBlas2
	{
		using ScalarType = double;
		using VectorType = FDMVector2;
		using MatrixType = FDMMatrix2;

		//! Sets entire element of given vector \p result with scalar \p s.
		static void Set(double s, FDMVector2* result);

		//! Copies entire element of given vector \p result with other vector \p v.
		static void Set(const FDMVector2& v, FDMVector2* result);

		//! Sets entire element of given matrix \p result with scalar \p s.
		static void Set(double s, FDMMatrix2* result);

		//! Copies entire element of given matrix \p result with other matrix \p v.
		static void Set(const FDMMatrix2& m, FDMMatrix2* result);

		//! Performs dot product with vector \p a and \p b.
		static double Dot(const FDMVector2& a, const FDMVector2& b);

		//! Performs ax + y operation where \p a is a matrix and \p x and \p y are
		//! vectors.
		static void AXPlusY(double a, const FDMVector2& x, const FDMVector2& y, FDMVector2* result);

		//! Performs matrix-vector multiplication.
		static void MVM(const FDMMatrix2& m, const FDMVector2& v, FDMVector2* result);

		//! Computes residual vector (b - ax).
		static void Residual(const FDMMatrix2& a, const FDMVector2& x, const FDMVector2& b, FDMVector2* result);

		//! Returns L2-norm of the given vector \p v.
		static double L2Norm(const FDMVector2& v);

		//! Returns L-inf-norm of the given vector \p v.
		static double LInfNorm(const FDMVector2& v);
	};
}

#endif