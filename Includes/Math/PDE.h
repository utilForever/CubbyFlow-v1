/*************************************************************************
> File Name: PDE.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Partial differential equation functions for CubbyFlow.
> Created Time: 2017/08/31
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PDE_H
#define CUBBYFLOW_PDE_H

#include <array>

namespace CubbyFlow
{
	//!
	//! \brief 1st order upwind differencing.
	//! D0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! D0[0]  D0[1]  D0[2]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	template <typename T>
	std::array<T, 2> Upwind1(T* D0, T dx);

	//!
	//! \brief 1st order upwind differencing.
	//! D0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! D0[0]  D0[1]  D0[2]
	//! \endcode
	//!
	template <typename T>
	T Upwind1(T* D0, T dx, bool isDirectionPositive);

	//!
	//! \brief 2nd order central differencing.
	//! D0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! D0[0]  D0[1]  D0[2]
	//! \endcode
	//!
	template <typename T>
	T CD2(T* D0, T dx);

	//!
	//! \brief 3rd order ENO.
	//! D0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! D0[0]  D0[1]  D0[2]  D0[3]  D0[4]  D0[5]  D0[6]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	template <typename T>
	std::array<T, 2> ENO3(T* D0, T dx);

	//!
	//! \brief 3rd order ENO.
	//! D0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! D0[0]  D0[1]  D0[2]  D0[3]  D0[4]  D0[5]  D0[6]
	//! \endcode
	//!
	//!
	template <typename T>
	T ENO3(T* D0, T dx, bool isDirectionPositive);

	//!
	//! \brief 5th order WENO.
	//! D0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! D0[0]  D0[1]  D0[2]  D0[3]  D0[4]  D0[5]  D0[6]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	//!
	template <typename T>
	std::array<T, 2> WENO5(T* v, T h, T eps = 1.0e-8);

	//!
	//! \brief 5th order WENO.
	//! D0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! D0[0]  D0[1]  D0[2]  D0[3]  D0[4]  D0[5]  D0[6]
	//! \endcode
	//!
	template <typename T>
	T WENO5(T* v, T h, bool is_velocity_positive, T eps = 1.0e-8);
}

#include <Math/PDE-Impl.h>

#endif