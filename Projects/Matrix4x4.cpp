/*************************************************************************
> File Name: Matrix4x4.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 4-D matrix class.
> Created Time: 2017/03/08
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Matrix\Matrix4x4.h>

namespace CubbyFlow
{
	template <typename T>
	Matrix<T, 4, 4>::Matrix() : m_elements(1, 0, 0, 0,
										   0, 1, 0, 0,
										   0, 0, 1, 0,
										   0, 0, 0, 1)
	{
		// Do nothing
	}

	template <typename T>
	Matrix<T, 4, 4>::Matrix(T s) : m_elements(s, s, s, s, 
											  s, s, s, s, 
											  s, s, s, s,
											  s, s, s, s)
	{
		// Do nothing
	}

	template <typename T>
	Matrix<T, 4, 4>::Matrix(T m00, T m01, T m02,
							T m10, T m11, T m12,
							T m20, T m21, T m22) : m_elements(m00, m01, m02, 0,
												    		  m10, m11, m12, 0, 
															  m20, m21, m22, 0,
															    0,   0,   0, 1)
	{
		// Do nothing
	}

	template <typename T>
	Matrix<T, 4, 4>::Matrix(
				T m00, T m01, T m02, T m03,
				T m10, T m11, T m12, T m13,
				T m20, T m21, T m22, T m23,
				T m30, T m31, T m32, T m33) : m_elements(
										m00, m01, m02, m03,
										m10, m11, m12, m13,
										m20, m21, m22, m23,
										m30, m31, m32, m33)
	{
		// Do nothing
	}
	template <typename T>
	template <typename U>
	Matrix<T, 4, 4>::Matrix(const std::initializer_list<std::initializer_list<U>>& list)
	{
		Set(list);
	}

	template <typename T>
	Matrix<T, 4, 4>::Matrix(const Matrix<T, 4, 4>& m)
	{
		Set(m);
	}
	
	template <typename T>
	Matrix<T, 4, 4>::Matrix(const Matrix3x3<T>& m33)
	{
		Set(m33);
	}

	template <typename T>
	Matrix<T, 4, 4>::Matrix(const T* arr)
	{
		std::copy(arr.begin(), arr.end(), m_elements.begin());
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(T s)
	{
		m_elements(
			s, s, s, s, 
			s, s, s, s, 
			s, s, s, s,
			s, s, s, s);
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(
		T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33)
	{
		m_elements(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33);
	}

	template <typename T>
	template <typename U>
	void Matrix<T, 4, 4>::Set(const std::initializer_list<std::initializer_list<U>>& list)
	{
		assert(list.size() >= 16);

		auto inputElem = list.begin();

		m_elements[0] = static_cast<T>(*inputElem);
		for (size_t i = 1; i < 16; i++)
		{
			m_elements[i] = static_cast<T>(*(++inputElem));
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(
		T m00, T m01, T m02,
		T m10, T m11, T m12,
		T m20, T m21, T m22)
	{
		m_elements(
			m00, m01, m02, 0,
			m10, m11, m12, 0,
			m20, m21, m22, 0,
			0, 0, 0, 1);
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(const Matrix3x3<T>& m33)
	{
		m_elements(
			m33.elements[0], m33.elements[1], m33.elements[2], 0,
			m33.elements[3], m33.elements[4], m33.elements[5], 0,
			m33.elements[6], m33.elements[7], m33.elements[8], 0,
			0, 0, 0, 1);
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(const Matrix<T, 4, 4>& m)
	{
		std::copy(m.m_elements.begin(), m.m_elements.end(), m_elements.begin());
	}

	template <typename T>
	void Matrix<T, 4, 4>::Set(const T* arr)
	{
		std::copy(std::begin(arr), std::end(arr), m_elements.begin());
	}

	template <typename T>
	void Matrix<T, 4, 4>::SetDiagonal(T s)
	{
		m_elements[0] = s;
		m_elements[5] = s;
		m_elements[10] = s;
		m_elements[15] = s;
	}

	template <typename T>
	void Matrix<T, 4, 4>::SetOffDiagonal(T s)
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (!(i % 5))
			{
				m_elements[i] = s;
			}
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::SetRow(size_t i, const Vector4<T>& row)
	{
		m_elements[i * 4] = row.m_elements[0];
		m_elements[i * 4 + 1] = row.m_elements[1];
		m_elements[i * 4 + 2] = row.m_elements[2];
		m_elements[i * 4 + 3] = row.m_elements[3];
	}

	template <typename T>
	void Matrix<T, 4, 4>::SetColumn(size_t i, const Vector4<T>& col)
	{
		m_elements[i] = col.m_elements[0];
		m_elements[i + 3] = col.m_elements[1];
		m_elements[i + 6] = col.m_elements[2];
		m_elements[i + 9] = col.m_elements[3];
	}

	template <typename T>
	bool Matrix<T, 4, 4>::IsSimilar(const Matrix<T, 4, 4>& m, double tol = std::numeric_limits<double>::epsilon()) const
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (std::fabs(m_elements[i] - m[i]) > epsilon)
			{
				return false;
			}
		}
		return true;
	}

	template <typename T>
	bool Matrix<T, 4, 4>::IsSquare() const
	{

	}

	template <typename T>
	size_t Matrix<T, 4, 4>::Rows() const
	{
		return 4;
	}

	template <typename T>
	size_t Matrix<T, 4, 4>::Cols() const
	{
		return 4;
	}

	template <typename T>
	T* Matrix<T, 4, 4>::Data()
	{
		return m_elements;
	}

	template <typename T>
	const T* Matrix<T, 4, 4>::Data() const
	{
		return m_elements;
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Add(T s) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] + s, m_elements[1] + s, m_elements[2] + s, m_elements[3] + s, 
			m_elements[4] + s, m_elements[5] + s, m_elements[6] + s, m_elements[7] + s, 
			m_elements[8] + s, m_elements[9] + s, m_elements[10] + s, m_elements[11] + s,
			m_elements[12] + s, m_elements[13] + s, m_elements[14] + s, m_elements[15] + s);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Add(const Matrix<T, 4, 4>& m) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] + m.m_elements[0], m_elements[1] + m.m_elements[1], m_elements[2] + m.m_elements[2], m_elements[3] + m.m_elements[3],
			m_elements[4] + m.m_elements[4], m_elements[5] + m.m_elements[5], m_elements[6] + m.m_elements[6], m_elements[7] + m.m_elements[7],
			m_elements[8] + m.m_elements[8], m_elements[9] + m.m_elements[9], m_elements[10] + m.m_elements[10], m_elements[11] + m.m_elements[11],
			m_elements[12] + m.m_elements[12], m_elements[13] + m.m_elements[13], m_elements[14] + m.m_elements[14], m_elements[15] + m.m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Sub(T s) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] - s, m_elements[1] - s, m_elements[2] - s, m_elements[3] - s,
			m_elements[4] - s, m_elements[5] - s, m_elements[6] - s, m_elements[7] - s,
			m_elements[8] - s, m_elements[9] - s, m_elements[10] - s, m_elements[11] - s,
			m_elements[12] - s, m_elements[13] - s, m_elements[14] - s, m_elements[15] - s);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Sub(const Matrix<T, 4, 4>& m) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] - m.m_elements[0], m_elements[1] - m.m_elements[1], m_elements[2] - m.m_elements[2], m_elements[3] - m.m_elements[3],
			m_elements[4] - m.m_elements[4], m_elements[5] - m.m_elements[5], m_elements[6] - m.m_elements[6], m_elements[7] - m.m_elements[7],
			m_elements[8] - m.m_elements[8], m_elements[9] - m.m_elements[9], m_elements[10] - m.m_elements[10], m_elements[11] - m.m_elements[11],
			m_elements[12] - m.m_elements[12], m_elements[13] - m.m_elements[13], m_elements[14] - m.m_elements[14], m_elements[15] - m.m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Mul(T s) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] * s, m_elements[1] * s, m_elements[2] * s, m_elements[3] * s,
			m_elements[4] * s, m_elements[5] * s, m_elements[6] * s, m_elements[7] * s,
			m_elements[8] * s, m_elements[9] * s, m_elements[10] * s, m_elements[11] * s,
			m_elements[12] * s, m_elements[13] * s, m_elements[14] * s, m_elements[15] * s);
	}

	template <typename T>
	Vector4<T> Matrix<T, 4, 4>::Mul(const Vector4<T> & v) const
	{
		std::array<T> arr;
		
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				arr[i] += m_elements[i * 4 + j] * v.m_elements[j];
			}
		}

		return Vector4<T>(arr);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Mul(const Matrix<T, 4, 4>& m) const
	{
		assert(m.Cols == 4);
		std::array<T> tmp;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				tmp[i * 4 + j] = m_elements[i * 4] * m.m_elements[i * 4 + j] + m_elements[i * 4 + 1] * m.m_elements[(i + 1) * 4 + j] + m_elements[i * 4 + 2] * m.m_elements[(i + 2) * 4 + j] + m_elements[i * 4 + j + 3] * m.m_elements[(i + 3) * 4 + j];
			}
		}
		return Matrix<T, 4, 4>(tmp);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Div(T s) const
	{
		return Matrix<T, 4, 4>(
			m_elements[0] / s, m_elements[1] / s, m_elements[2] / s, m_elements[3] / s,
			m_elements[4] / s, m_elements[5] / s, m_elements[6] / s, m_elements[7] / s, 
			m_elements[8] / s, m_elements[9] / s, m_elements[10] / s, m_elements[11] / s,
			m_elements[12] / s, m_elements[13] / s, m_elements[14] / s, m_elements[15] / s);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RAdd(T s) const
	{
		return Add(s);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RAdd(const Matrix<T, 4, 4>& m) const
	{
		return Add(m);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RSub(T s) const
	{
		return Matrix<T, 4, 4>(
			s - m_elements[0], s - m_elements[1], s - m_elements[2], s - m_elements[3],
			s - m_elements[4], s - m_elements[5], s - m_elements[6], s - m_elements[7],
			s - m_elements[8], s - m_elements[9], s - m_elements[10], s - m_elements[11],
			s - m_elements[12], s - m_elements[13], s - m_elements[14], s - m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RSub(const Matrix<T, 4, 4>& m) const
	{
		return Matrix<T, 4, 4>(
			m.m_elements[0] - m_elements[0], m.m_elements[1] - m_elements[1], m.m_elements[2] - m_elements[2], m.m_elements[3] - m_elements[3],
			m.m_elements[4] - m_elements[4], m.m_elements[5] - m_elements[5], m.m_elements[6] - m_elements[6], m.m_elements[7] - m_elements[7], 
			m.m_elements[8] - m_elements[8], m.m_elements[9] - m_elements[9], m.m_elements[10] - m_elements[10], m.m_elements[11] - m_elements[12],
			m.m_elements[12] - m_elements[12], m.m_elements[13] - m_elements[13], m.m_elements[14] - m_elements[14], m.m_elements[15] - m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RMul(T s) const
	{
		return Mul(s);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RMul(const Matrix<T, 4, 4>& m) const
	{
		assert(m.Cols == 4);
		std::array<T> tmp;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				tmp[i * 4 + j] = m.m_elements[i * 4] * m_elements[i * 4 + j] + m.m_elements[i * 4 + 1] * m_elements[(i + 1) * 4 + j] + m.m_elements[i * 4 + 2] * m_elements[(i + 2) * 4 + j] + m.m_elements[i * 4 + 3] * m_elements[(i + 3) * 4 + j];
			}
		}
		return Matrix<T, 4, 4>(tmp);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::RDiv(T s) const
	{
		return Matrix<T, 4, 4>(
			s / m_elements[0], s / m_elements[1], s / m_elements[2], s / m_elements[3],
			s / m_elements[4], s / m_elements[5], s / m_elements[6], s / m_elements[7],
			s / m_elements[8], s / m_elements[9], s / m_elements[10], s / m_elements[11],
			s / m_elements[12], s / m_elements[13], s / m_elements[14], s / m_elements[15]);
	}

	template <typename T>
	void Matrix<T, 4, 4>::IAdd(T s)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] += s;
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::IAdd(const Matrix<T, 4, 4>& m)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] += m.m_elements[i];
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::ISub(T s)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] -= s;
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::ISub(const Matrix<T, 4, 4>& m)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] -= m.m_elements[i];
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::IMul(T s)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] *= s;
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::IMul(const Matrix<T, 4, 4>& m)
	{
		Set(Mul(m));
	}

	template <typename T>
	void Matrix<T, 4, 4>::IDiv(T s)
	{
		for (size_t i = 0; i < 16; i++)
		{
			m_elements[i] /= s;
		}
	}

	template <typename T>
	void Matrix<T, 4, 4>::Transpose()
	{
		std::swap(m_elements[1], m_elements[4]);
		std::swap(m_elements[2], m_elements[8]);
		std::swap(m_elements[3], m_elements[12]);
		std::swap(m_elements[6], m_elements[9]);
		std::swap(m_elements[7], m_elements[13]);
		std::swap(m_elements[11], m_elements[14]);
	}

	template <typename T>
	void Matrix<T, 4, 4>::Invert()
	{
		assert(Determinant());

		std::array<T> tmp, res;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t newI = 0; newI < 4; newI++)
				{
					for (size_t newJ = 0; newJ < 4; newJ++)
					{
						if (i != newI && j != newJ)
						{
							tmp.push(m_elements[newI * 4 + newJ]);
						}
					}
				}
				res[i * 4 + j] = std::pow(-1, i + j) * Matrix<T, 3, 3>(tmp).Determinant();
			}
		}

		Set(res);
	}

	template <typename T>
	T Matrix<T, 4, 4>::Sum() const
	{
		return (
			m_elements[0] + m_elements[1] + m_elements[2] + m_elements[3] + 
			m_elements[4] + m_elements[5] + m_elements[6] + m_elements[7] + 
			m_elements[8] + m_elements[9] + m_elements[10] + m_elements[11] +
			m_elements[12] + m_elements[13] + m_elements[14] + m_elements[15]);
	}

	template <typename T>
	T Matrix<T, 4, 4>::Avg() const
	{
		return Sum() / 16;
	}

	template <typename T>
	T Matrix<T, 4, 4>::Min() const
	{
		return std::min_element(m_elements.begin(), m_elements.end());
	}

	template <typename T>
	T Matrix<T, 4, 4>::Max() const
	{
		return std::max_element(m_elements.begin(), m_elements.end());
	}

	template <typename T>
	T Matrix<T, 4, 4>::AbsMin() const
	{
		return std::min({ 
			std::fabs(m_elements[0]), std::fabs(m_elements[1]), std::fabs(m_elements[2]), std::fabs(m_elements[3]), 
			std::fabs(m_elements[4]), std::fabs(m_elements[5]), std::fabs(m_elements[6]), std::fabs(m_elements[7]), 
			std::fabs(m_elements[8]), std::fabs(m_elements[9]), std::fabs(m_elements[10]), std::fabs(m_elements[11]),
			std::fabs(m_elements[12]),  std::fabs(m_elements[13]),  std::fabs(m_elements[14]),  std::fabs(m_elements[15])});
	}

	template <typename T>
	T Matrix<T, 4, 4>::AbsMax() const
	{
		return std::max({
			std::fabs(m_elements[0]), std::fabs(m_elements[1]), std::fabs(m_elements[2]), std::fabs(m_elements[3]),
			std::fabs(m_elements[4]), std::fabs(m_elements[5]), std::fabs(m_elements[6]), std::fabs(m_elements[7]),
			std::fabs(m_elements[8]), std::fabs(m_elements[9]), std::fabs(m_elements[10]), std::fabs(m_elements[11]),
			std::fabs(m_elements[12]),  std::fabs(m_elements[13]),  std::fabs(m_elements[14]),  std::fabs(m_elements[15]) });
	}

	template <typename T>
	T Matrix<T, 4, 4>::Trace() const
	{
		return m_elements[0] + m_elements[5] + m_elements[10] + m_elements[15];
	}

	template <typename T>
	T Matrix<T, 4, 4>::Determinant() const
	{
		T sum = 0;
		
		for (size_t i = 0; i < 4; i++)
		{
			std::array<T> arr;
			size_t cnt = 0;
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					if ((j - i) * (j - k) != 0)
					{
						arr[cnt++] = m_elements[j * 4 + k];
					}
				}
			}
			sum += pow(-1, i + 1) m_elements[i] * Matrix<T, 3, 3>(arr);
			arr.clear();
		}

		return sum;
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Diagonal() const
	{
		return Matrix<T, 4, 4>(
			m_elements[0], 0, 0, 0, 
			0, m_elements[5], 0, 0, 
			0, 0, m_elements[10], 0,
			0, 0, 0, m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::OffDiagonal() const
	{
		return Matrix<T, 4, 4>(
			0, m_elements[1], m_elements[2], m_elements[3], 
			m_elements[4], 0, m_elements[6], m_elements[7],
			m_elements[8], m_elements[9], 0, m_elements[11],
			m_elements[12], m_elements[13], m_elements[14], 0);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::StrictLowerTriangle() const
	{
		return Matrix<T, 4, 4>(
			0, 0, 0, 0, 
			m_elements[4], 0, 0, 0,
			m_elements[8], m_elements[9], 0, 0,
			m_elements[12], m_elements[13], m_elements[14], m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::StrictUpperTriangle() const
	{
		return Matrix<T, 4, 4>(
			0, m_elements[1], m_elements[2], m_elements[3],
			0, 0, m_elements[6], m_elements[7],
			0, 0, 0, m_elements[11],
			0, 0, 0, 0);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::LowerTriangle() const
	{
		return Matrix<T, 4, 4>(
			m_elements[0], 0, 0, 0,
			m_elements[4], m_elements[5], 0, 0,
			m_elements[8], m_elements[9], m_elements[10], 0,
			m_elements[12], m_elements[13], m_elements[14], m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::UpperTriangle() const
	{
		return Matrix<T, 4, 4>(
			m_elements[0], m_elements[1], m_elements[2], m_elements[3],
			0, m_elements[5], m_elements[6], m_elements[7],
			0, 0, m_elements[10], m_elements[11],
			0, 0, 0, m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Transposed() const
	{
		return Matrix<T, 4, 4>(
			m_elements[0], m_elements[4], m_elements[8], m_elements[12],
			m_elements[1], m_elements[5], m_elements[9], m_elements[13],
			m_elements[2], m_elements[6], m_elements[10], m_element[14],
			m_elements[3], m_elements[7], m_elements[11], m_elements[15]);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::Inverse() const
	{
		assert(Determinant());

		std::array<T> tmp, res;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t newI = 0; newI < 4; newI++)
				{
					for (size_t newJ = 0; newJ < 4; newJ++)
					{
						if (i != newI && j != newJ)
						{
							tmp.push(m_elements[newI * 4 + newJ]);
						}
					}
				}
				res[i * 4 + j] = std::pow(-1, i + j) * Matrix<T, 3, 3>(tmp).Determinant();
				tmp.clear();
			}
		}
		return Matrix<T, 4, 4>(res);
	}

	template <typename T>
	template <typename U>
	Matrix<U, 4, 4> Matrix<T, 4, 4>::CastTo() const
	{
		return Matrix<U, 4, 4>(
			static_cast<U>(m_elements[0]), static_cast<U>(m_elements[1]), static_cast<U>(m_elements[2]), static_cast<U>(m_elements[3]), 
			static_cast<U>(m_elements[4]), static_cast<U>(m_elements[5]), static_cast<U>(m_elements[6]), static_cast<U>(m_elements[7]), 
			static_cast<U>(m_elements[8]), static_cast<U>(m_elements[9]), static_cast<U>(m_elements[10]), static_cast<U>(m_elements[11]), 
			static_cast<U>(m_elements[12]), static_cast<U>(m_elements[13]), static_cast<U>(m_elements[14]), static_cast<U>(m_elements[15]));
	}


	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator=(const Matrix<T, 4, 4>& m)
	{
		Set(m);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator+=(const Matrix<T, 4, 4>& m)
	{
		IAdd(m);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator+=(T s)
	{
		IAdd(s);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator-=(const Matrix<T, 4, 4>& m)
	{
		ISub(m);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator-=(T s)
	{
		ISub(s);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator*=(const T s)
	{
		IMul(s);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator*=(const Matrix3x3<T>& m)
	{
		IMul(m);
		return (*this);
	}

	template <typename T>
	Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator/=(T s)
	{
		IDiv(s);
		return (*this);
	}

	template <typename T>
	T& Matrix<T, 4, 4>::operator[](size_t i)
	{
		return m_elements[i];
	}

	template <typename T>
	const T& Matrix<T, 4, 4>::operator[](size_t i) const
	{
		return m_elements[i];
	}

	template <typename T>
	T& Matrix<T, 4, 4>::operator()(size_t i, size_t j)
	{
		return m_elements[i * 4 + j];
	}

	template <typename T>
	const T& Matrix<T, 4, 4>::operator()(size_t i, size_t j) const
	{
		return m_elements[i * 4 + j];
	}

	template <typename T>
	bool Matrix<T, 4, 4>::operator==(const Matrix& m) const
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (m_elements[i] != m.m_elements[i])
			{
				return false;
			}
		}
		return true;
	}

	template <typename T>
	bool Matrix<T, 4, 4>::operator!=(const Matrix& m) const
	{
		return !((*this) == m);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeZero()
	{
		return Matrix<T, 4, 4>(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeIdentity()
	{
		return Matrix<T, 4, 4>(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeScaleMatrix(T sx, T sy, T sz)
	{
		return Matrix<T, 4, 4>(
			sx, 0, 0, 0, 
			0, sy, 0, 0, 
			0, 0, sz, 0,
			0, 0, 0, 1);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeScaleMatrix(const Vector3<T>& s)
	{
		return MakeScaleMatrix(s.x, s.y, s.z);
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeRotationMatrix(const Vector3<T>& axis, const T rad)
	{
		return Matrix<T, 4, 4>(cos(rad) + axis.x * axis.x * (1 - cos(rad)), axis.x * axis.y * (1 - cos(rad)) - axis.z * sin(rad), axis.x * axis.z * (1 - cos(rad)) + axis.y * sin(rad),
			axis.y * axis.x * (1 - cos(rad)) + axis.z * sin(rad), cos(rad) + axis.y * axis.y * (1 - cos(rad)), axis.y * axis.z * (1 - cos(rad)) - axis.x * sin(rad),
			axis.z * axis.x * (1 - cos(rad)) - axis.y * sin(rad), axis.z * axis.y * (1 - cos(rad)) + axis.x * sin(rad), cos(rad) + axis.z * axis.z * (1 - cos(rad)));
	}

	template <typename T>
	Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeTranslationMatrix(const Vector3<T>& t)
	{
		return Matrix<T, 4, 4>(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1);
	}

	template <typename T>
	Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a)
	{
		return (*this) * -1;
	}

	template <typename T>
	Matrix<T, 4, 4> operator+(const Matrix<T, 4, 4>& a, const Matrix<T, 4, 4>& b)
	{
		return a.Add(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator+(const Matrix<T, 4, 4>& a, const T b)
	{
		return a.Add(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator+(const T a, const Matrix<T, 4, 4>& b)
	{
		return b.Add(a);
	}

	template <typename T>
	Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a, const Matrix<T, 4, 4>& b)
	{
		return a.Sub(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a, const T b)
	{
		return a.Sub(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator-(const T a, const Matrix<T, 4, 4>& b)
	{
		return b.Rsub(a);
	}

	template <typename T>
	Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, const T b)
	{
		return a.Mul(Matrix<T, 4, 4>(b));
	}

	template <typename T>
	Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, const Vector3<T>& b)
	{
		return a.Mul(b);
	}
	
	template <typename T>
	Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, const Vector4<T>& b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, const Matrix3x3<T>& b)
	{
		return a.Mul(b);
	}

	template <typename T>
	Matrix<T, 4, 4> operator*(const Matrix3x3<T>& a, const Matrix<T, 4, 4>& b)
	{
		return b.RMul(a);
	}

	template <typename T>
	Matrix<T, 4, 4> operator/(const Matrix<T, 4, 4>& a, T b)
	{
		return a.Div(Matrix<T, 4, 4>(b));
	}

	template <typename T>
	Matrix<T, 4, 4> operator/(const T& a, const Matrix<T, 4, 4>& b)
	{
		return b.RDiv(Matrix<T, 4, 4>(a));
	}
}

