/*************************************************************************
> File Name: MathUtils.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Mathematics util functions for CubbyFlow.
> Created Time: 2017/02/26
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MATH_UTILS_H
#define CUBBYFLOW_MATH_UTILS_H

namespace CubbyFlow
{
	//!
	//! \brief      Returns the absolute minimum value among the two inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute minimum.
	//!
	template <typename T>
	inline T AbsMin(T x, T y);

	//!
	//! \brief      Returns the absolute minimum value among the three inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//! \param[in]  z     The third value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute minimum.
	//!
	template <typename T>
	inline T AbsMin(T x, T y, T z);

	//!
	//! \brief      Returns the absolute maximum value among the two inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute maximum.
	//!
	template <typename T>
	inline T AbsMax(T x, T y);

	//!
	//! \brief      Returns the absolute maximum value among the three inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//! \param[in]  y     The third value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute maximum.
	//!
	template <typename T>
	inline T AbsMax(T x, T y, T z);

	//!
	//! \brief      Returns the square of \p x.
	//!
	//! \param[in]  x     The input.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The squared value.
	//!
	template <typename T>
	inline T Square(T x);

	//!
	//! \brief      Returns the cubic of \p x.
	//!
	//! \param[in]  x     The input.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The cubic of \p x.
	//!
	template <typename T>
	inline T Cubic(T x);

	//!
	//! \brief      Returns the clamped value.
	//!
	//! \param[in]  val   The value.
	//! \param[in]  low   The low value.
	//! \param[in]  high  The high value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The clamped value.
	//!
	template <typename T>
	inline T Clamp(T val, T low, T high);
}

#endif