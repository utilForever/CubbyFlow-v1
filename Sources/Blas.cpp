/*************************************************************************
> File Name: Blas.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Basic Linear Algebra Subprograms.
> Created Time: 2017/06/18
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include"Blas.h"

namespace CubbyFlow {
	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::set(
		ScalarType s, VectorType* result) {
		result->set(s);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::set(
		const VectorType& v, VectorType* result) {
		result->set(v);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::set(
		ScalarType s, MatrixType* result) {
		result->set(s);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::set(
		const MatrixType& m, MatrixType* result) {
		result->set(m);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType Blas<ScalarType, VectorType, MatrixType>::dot(
		const VectorType& a, const VectorType& b) {
		return a.dot(b);
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::axpy(
		ScalarType a,
		const VectorType& x,
		const VectorType& y,
		VectorType* result) {
		*result = a * x + y;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::mvm(
		const MatrixType& m, const VectorType& v, VectorType* result) {
		*result = m * v;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	void Blas<ScalarType, VectorType, MatrixType>::residual(
		const MatrixType& a,
		const VectorType& x,
		const VectorType& b,
		VectorType* result) {
		*result = b - a * x;
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType Blas<ScalarType, VectorType, MatrixType>::l2Norm(
		const VectorType& v) {
		return std::sqrt(v.dot(v));
	}

	template <typename ScalarType, typename VectorType, typename MatrixType>
	ScalarType Blas<ScalarType, VectorType, MatrixType>::lInfNorm(
		const VectorType& v) {
		return std::fabs(v.absmax());
	}
}