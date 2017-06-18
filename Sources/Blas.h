/*************************************************************************
> File Name: Blas.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Basic Linear Algebra Subprograms.
> Created Time: 2017/06/18
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_BLAS_H
#define CUBBYFLOW_BLAS_H

namespace CubbyFlow
{
	template<typename S, typename V, typename M>
	struct Blas
	{
		typedef S ScalarType;
		typedef V VectorType;
		typedef M MatrixType;
		
		// Sets the given scalar value to the output vector.
		static void set(ScalarType s, VectorType* result);

		// Sets the given vector to the output vector.
		static void set(VectorType& v, MatrixType& result);

		// Sets the given matrix to the output matrix.
		static void set(const MatrixType& m, MatrixType& result);

		// Performs dot product.
		static ScalarType dot(const VectorType& a, const VectorType& b);

		// Calculates a * x + y.
		static void axpy(
			ScalarType a,
			const VectorType& x,
			const VectorType& y,
			VectorType* result);

		// Performs matrix-vector multiplication.
		static void mvm(
			const MatrixType& m,
			const VectorType& v,
			VectorType* result);

		// Calculates b - A*x.
		static void residual(
			const MatrixType& a,
			const VectorType& x,
			const VectorType& b,
			VectorType* result);

		// Returns the length of the vector.
		static ScalarType l2Norm(const VectorType& v);

		// Returns the absolute maximum value among the vector elements.
		static ScalarType lInfNorm(const VectorType& v);

		template <typename BlasType> BlasType = Blas;
	};
}

#endif
