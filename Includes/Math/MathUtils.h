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

#include <Utils/Macros.h>

namespace CubbyFlow
{
	//!
	//! \brief      Returns the sign of the value.
	//!
	//! \param[in]  x     Input value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The sign.
	//!
	template <typename T>
	inline T Sign(T x);

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
	//! \brief      Returns the absolute minimum value among the four inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//! \param[in]  z     The third value.
	//! \param[in]  z     The fourth value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute minimum.
	//!
	template <typename T>
	inline T AbsMin(T x, T y, T z, T w);

	//!
	//! \brief      Returns the absolute minimum value among the elements in std::array.
	//!
	//! \param[in]  arr     The array.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute minimum.
	//!
	template <typename T, int N>
	inline T AbsMin(std::array<T, N> arr);

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
	//! \brief      Returns the absolute maximum value among the fourth inputs.
	//!
	//! \param[in]  x     The first value.
	//! \param[in]  y     The second value.
	//! \param[in]  y     The third value.
	// !\param[in]  w     The fourth value.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute maximum.
	//!
	template <typename T>
	inline T AbsMax(T x, T y, T z, T w);

	//!
	//! \brief      Returns the absolute maximum value among the elements in std::array.
	//!
	//! \param[in]  arr     The array.
	//!
	//! \tparam     T     Value type.
	//!
	//! \return     The absolute maximum.
	//!
	template <typename T, int N>
	inline T AbsMax(std::array<T, N> arr);

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

	//!
	//! \brief      Gets the barycentric coordinate.
	//!
	//! \param[in]  x     The input value.
	//! \param[in]  iLow  The lowest index.
	//! \param[in]  iHigh The highest index.
	//! \param      i     The output index.
	//! \param      t     The offset from \p i.
	//!
	//! \tparam     T     Value type.
	//!
	template<class T>
	inline void GetBarycentric(T x, ssize_t iLow, ssize_t iHigh, ssize_t* i, T* t);

	//!
	//! \brief      Computes linear interpolation.
	//!
	//! \param[in]  f0    The first value.
	//! \param[in]  f1    The second value.
	//! \param[in]  t     Relative offset [0, 1] from the first value.
	//!
	//! \tparam     S     Input value type.
	//! \tparam     T     Offset type.
	//!
	//! \return     The interpolated value.
	//!
	template<typename S, typename T>
	inline S Lerp(const S& f0, const S& f1, T t);

	//! \brief      Computes bilinear interpolation.
	template<typename S, typename T>
	inline S BiLerp(
		const S& f00, const S& f10,
		const S& f01, const S& f11,
		T tx, T ty);

	//! \brief      Computes trilinear interpolation.
	template<typename S, typename T>
	inline S TriLerp(
		const S& f000, const S& f100,
		const S& f010, const S& f110,
		const S& f001, const S& f101,
		const S& f011, const S& f111,
		T tx, T ty, T tz);

	//! \brief      Computes Catmull-Rom interpolation.
	template <typename S, typename T>
	inline S CatmullRom(const S& f0, const S& f1, const S& f2, const S& f3, T t);

	//! \brief      Computes monotonic Catmull-Rom interpolation.
	template <typename T>
	inline T MonotonicCatmullRom(const T& f0, const T& f1, const T& f2, const T& f3, T t);
}

#include <Math/MathUtils-Impl.h>

#endif