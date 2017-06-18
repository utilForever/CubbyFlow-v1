/*************************************************************************
> File Name: cg.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Basic Conjugate Gradient
> Created Time: 2017/06/18
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_CG_H
#define CUBBYFLOW_CG_H

namespace CUBBYFLOW
{
	template <typename BlasType>
	struct NullCgPreconditioner final {
		void build(const typename BlasType::MatrixType&) {}

		void solve(
			const typename BlasType::VectorType& b,
			typename BlasType::VectorType* x) {
			BlasType::set(b, x);
		}
	};

	//!
	//! \brief Solves conjugate gradient.
	//!
	template <typename BlasType>
	void cg(
		const typename BlasType::MatrixType& A,
		const typename BlasType::VectorType& b,
		unsigned int maxNumberOfIterations,
		double tolerance,
		typename BlasType::VectorType* x,
		typename BlasType::VectorType* r,
		typename BlasType::VectorType* d,
		typename BlasType::VectorType* q,
		typename BlasType::VectorType* s,
		unsigned int* lastNumberOfIterations,
		double* lastResidualNorm);

	//!
	//! \brief Solves pre-conditioned conjugate gradient.
	//!
	template <
		typename BlasType,
		typename PrecondType>
		void pcg(
			const typename BlasType::MatrixType& A,
			const typename BlasType::VectorType& b,
			unsigned int maxNumberOfIterations,
			double tolerance,
			PrecondType* M,
			typename BlasType::VectorType* x,
			typename BlasType::VectorType* r,
			typename BlasType::VectorType* d,
			typename BlasType::VectorType* q,
			typename BlasType::VectorType* s,
			unsigned int* lastNumberOfIterations,
			double* lastResidualNorm);
}
#endif