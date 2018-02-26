/*************************************************************************
> File Name: Quaternion-Impl.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Quaternion class defined as q = w + xi + yj + zk.
> Created Time: 2017/03/21
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_QUATERNION_IMPL_H
#define CUBBYFLOW_QUATERNION_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	Quaternion<T>::Quaternion()
	{
		SetIdentity();
	}

	template <typename T>
	Quaternion<T>::Quaternion(T newW, T newX, T newY, T newZ) 
	{
		Set(newW, newX, newY, newZ);
	}
	
	template <typename T>
	Quaternion<T>::Quaternion(std::initializer_list<T>& list)
	{
		Set(list);
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Vector3<T>& axis, T angle)
	{
		Set(axis, angle);
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Vector3<T>& from, const Vector3<T>& to)
	{
		Set(from, to);
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Vector3<T>& axis0, const Vector3<T>& axis1, const Vector3<T>& axis2)
	{
		Set(axis0, axis1, axis2);
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Matrix3x3<T>& m33)
	{
		Set(m33);
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Quaternion& other)
	{
		Set(other);
	}

	template <typename T>
	void Quaternion<T>::Set(const Quaternion& other)
	{
		Set(other.data.w, other.data.x, other.data.y, other.data.z);
	}

	template <typename T>
	void Quaternion<T>::Set(T newW, T newX, T newY, T newZ) 
	{
		data.w = newW;
		data.x = newX;
		data.y = newY;
		data.z = newZ;
	}

	template <typename T>
	void Quaternion<T>::Set(std::initializer_list<T>& list)
	{
		assert(list.size() == 4);
		auto inputElem = list.begin();

		Data* arrData = &data;
		for (size_t i = 0; i < 4; ++i)
		{
			arrData[i] = inputElem[i];
		}
	}

	template <typename T>
	void Quaternion<T>::Set(const Vector3<T>& axis, T angle)
	{
		static const T eps = std::numeric_limits<T>::epsilon();

		T axisLengthSquared = axis.LengthSquared();

		if (axisLengthSquared < eps)
		{
			SetIdentity();
		}
		else
		{
			Vector3<T> normalizedAxis = axis.Normalized();
			T s = std::sin(angle / 2);

			data.x = normalizedAxis.x * s;
			data.y = normalizedAxis.y * s;
			data.z = normalizedAxis.z * s;
			data.w = std::cos(angle / 2);
		}
	}

	template <typename T>
	void Quaternion<T>::Set(const Vector3<T>& from, const Vector3<T>& to)
	{
		static const T eps = std::numeric_limits<T>::epsilon();

		Vector3<T> axis = from.Cross(to);

		T fromLengthSquared = from.LengthSquared();
		T toLengthSquared = to.LengthSquared();

		if (fromLengthSquared < eps || toLengthSquared < eps)
		{
			SetIdentity();
		}
		else
		{
			T axisLengthSquared = axis.LengthSquared();

			// In case two vectors are exactly the opposite, pick orthogonal vector for axis.
			if (axisLengthSquared < eps)
			{
				axis = std::get<0>(from.Tangential());
			}

			Set(from.Dot(to), axis.x, axis.y, axis.z);
			data.w += L2Norm();

			Normalize();
		}
	}

	template <typename T>
	void Quaternion<T>::Set(const Vector3<T>& rotationBasis0, const Vector3<T>& rotationBasis1, const Vector3<T>& rotationBasis2)
	{
		Matrix3x3<T> matrix3;

		matrix3.SetColumn(0, rotationBasis0.Normalized());
		matrix3.SetColumn(1, rotationBasis1.Normalized());
		matrix3.SetColumn(2, rotationBasis2.Normalized());

		Set(matrix3);
	}

	template <typename T>
	void Quaternion<T>::Set(const Matrix3x3<T>& m)
	{
		static const T eps = std::numeric_limits<T>::epsilon();
		static const T quarter = static_cast<T>(0.25);

		T onePlusTrace = m.Trace() + 1;

		if (onePlusTrace > eps)
		{
			T S = std::sqrt(onePlusTrace) * 2;
			data.w = quarter * S;
			data.x = (m(2, 1) - m(1, 2)) / S;
			data.y = (m(0, 2) - m(2, 0)) / S;
			data.z = (m(1, 0) - m(0, 1)) / S;
		}
		else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
		{
			T S = std::sqrt(1 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
			data.w = (m(2, 1) - m(1, 2)) / S;
			data.x = quarter * S;
			data.y = (m(0, 1) + m(1, 0)) / S;
			data.z = (m(0, 2) + m(2, 0)) / S;
		}
		else if (m(1, 1) > m(2, 2))
		{
			T S = std::sqrt(1 + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
			data.w = (m(0, 2) - m(2, 0)) / S;
			data.x = (m(0, 1) + m(1, 0)) / S;
			data.y = quarter * S;
			data.z = (m(1, 2) + m(2, 1)) / S;
		}
		else
		{
			T S = std::sqrt(1 + m(2, 2) - m(0, 0) - m(1, 1)) * 2;
			data.w = (m(1, 0) - m(0, 1)) / S;
			data.x = (m(0, 2) + m(2, 0)) / S;
			data.y = (m(1, 2) + m(2, 1)) / S;
			data.z = quarter * S;
		}
	}

	template <typename T>
	template <typename U>
	Quaternion<U> Quaternion<T>::CastTo() const
	{
		return Quaternion<U>(static_cast<U>(data.w), static_cast<U>(data.x), static_cast<U>(data.y), static_cast<U>(data.z));
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Normalized() const
	{
		Quaternion q(*this);
		q.Normalize();
		return q;
	}

	template <typename T>
	Vector3<T> Quaternion<T>::Mul(const Vector3<T>& v) const
	{
		T _2xx = 2 * data.x * data.x;
		T _2yy = 2 * data.y * data.y;
		T _2zz = 2 * data.z * data.z;
		T _2xy = 2 * data.x * data.y;
		T _2xz = 2 * data.x * data.z;
		T _2xw = 2 * data.x * data.w;
		T _2yz = 2 * data.y * data.z;
		T _2yw = 2 * data.y * data.w;
		T _2zw = 2 * data.z * data.w;

		return Vector3<T>(
			(1 - _2yy - _2zz) * v.x + (_2xy - _2zw) * v.y + (_2xz + _2yw) * v.z,
			(_2xy + _2zw) * v.x + (1 - _2zz - _2xx) * v.y + (_2yz - _2xw) * v.z,
			(_2xz - _2yw) * v.x + (_2yz + _2xw) * v.y + (1 - _2yy - _2xx) * v.z);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Mul(const Quaternion& other) const
	{
		return Quaternion(
			data.w * other.data.w - data.x * other.data.x - data.y * other.data.y - data.z * other.data.z,
			data.w * other.data.x + data.x * other.data.w + data.y * other.data.z - data.z * other.data.y,
			data.w * other.data.y - data.x * other.data.z + data.y * other.data.w + data.z * other.data.x,
			data.w * other.data.z + data.x * other.data.y - data.y * other.data.x + data.z * other.data.w);
	}

	template <typename T>
	T Quaternion<T>::Dot(const Quaternion<T>& other)
	{
		return data.w * other.data.w + data.x * other.data.x + data.y * other.data.y + data.z * other.data.z;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::RMul(const Quaternion& other) const
	{
		return Quaternion(
			other.data.w * data.w - other.data.x * data.x - other.data.y * data.y - other.data.z * data.z,
			other.data.w * data.x + other.data.x * data.w + other.data.y * data.z - other.data.z * data.y,
			other.data.w * data.y - other.data.x * data.z + other.data.y * data.w + other.data.z * data.x,
			other.data.w * data.z + other.data.x * data.y - other.data.y * data.x + other.data.z * data.w);
	}

	template <typename T>
	void Quaternion<T>::IMul(const Quaternion& other)
	{
		*this = Mul(other);
	}

	template <typename T>
	void Quaternion<T>::SetIdentity()
	{
		Set(1, 0, 0, 0);
	}

	template <typename T>
	void Quaternion<T>::Rotate(T angleInRadians)
	{
		Vector3<T> axis;
		T currentAngle;

		GetAxisAngle(&axis, &currentAngle);

		currentAngle += angleInRadians;

		Set(axis, currentAngle);
	}

	template <typename T>
	void Quaternion<T>::Normalize()
	{
		T norm = L2Norm();

		if (norm > 0)
		{
			data.w /= norm;
			data.x /= norm;
			data.y /= norm;
			data.z /= norm;
		}
	}

	template <typename T>
	Vector3<T> Quaternion<T>::Axis() const
	{
		Vector3<T> result(data.x, data.y, data.z);
		result.Normalize();

		if (2 * std::acos(data.w) < PI<T>())
		{
			return result;
		}
		
		return -result;
	}

	template <typename T>
	T Quaternion<T>::Angle() const
	{
		T result = 2 * std::acos(data.w);

		if (result < PI<T>())
		{
			return result;
		}

		// Wrap around
		return 2 * PI<T>() - result;
	}

	template <typename T>
	void Quaternion<T>::GetAxisAngle(Vector3<T>* axis, T* angle) const
	{
		axis->Set(data.x, data.y, data.z);
		axis->Normalize();
		*angle = 2 * std::acos(data.w);

		if (*angle > PI<T>())
		{
			// Wrap around
			(*axis) = -(*axis);
			*angle = 2 * PI<T>() - (*angle);
		}
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::Inverse() const
	{
		T denom = data.w * data.w + data.x * data.x + data.y * data.y + data.z * data.z;
		return Quaternion(data.w / denom, -data.x / denom, -data.y / denom, -data.z / denom);
	}

	template <typename T>
	Matrix3x3<T> Quaternion<T>::Matrix3() const
	{
		T _2xx = 2 * data.x * data.x;
		T _2yy = 2 * data.y * data.y;
		T _2zz = 2 * data.z * data.z;
		T _2xy = 2 * data.x * data.y;
		T _2xz = 2 * data.x * data.z;
		T _2xw = 2 * data.x * data.w;
		T _2yz = 2 * data.y * data.z;
		T _2yw = 2 * data.y * data.w;
		T _2zw = 2 * data.z * data.w;

		Matrix3x3<T> m(
			1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
			_2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw,
			_2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx);

		return m;
	}

	template <typename T>
	Matrix4x4<T> Quaternion<T>::Matrix4() const
	{
		T _2xx = 2 * data.x * data.x;
		T _2yy = 2 * data.y * data.y;
		T _2zz = 2 * data.z * data.z;
		T _2xy = 2 * data.x * data.y;
		T _2xz = 2 * data.x * data.z;
		T _2xw = 2 * data.x * data.w;
		T _2yz = 2 * data.y * data.z;
		T _2yw = 2 * data.y * data.w;
		T _2zw = 2 * data.z * data.w;

		Matrix4x4<T> m(
			1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw, 0,
			_2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw, 0,
			_2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx, 0,
			0, 0, 0, 1);

		return m;
	}

	template <typename T>
	T Quaternion<T>::L2Norm() const
	{
		return std::sqrt(data.w * data.w + data.x * data.x + data.y * data.y + data.z * data.z);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator=(const Quaternion& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& other)
	{
		IMul(other);
		return *this;
	}

	template <typename T>
	T& Quaternion<T>::operator[](size_t i)
	{
		return (&data.w)[i];
	}

	template <typename T>
	const T& Quaternion<T>::operator[](size_t i) const
	{
		return (&data.w)[i];
	}

	template <typename T>
	bool Quaternion<T>::operator==(const Quaternion& other) const
	{
		return (data.w == other.data.w && data.x == other.data.x && data.y == other.data.y && data.z == other.data.z);
	}

	template <typename T>
	bool Quaternion<T>::operator!=(const Quaternion& other) const
	{
		return (data.w != other.data.w || data.x != other.data.x || data.y != other.data.y || data.z != other.data.z);
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::MakeIdentity()
	{
		return Quaternion();
	}

	template <typename T>
	Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t)
	{
		static const double threshold = 0.01;
		static const T eps = std::numeric_limits<T>::epsilon();

		T cosHalfAngle = dot(a, b);
		T weightA, weightB;

		// For better accuracy, return lerp result when a and b are close enough.
		if (1.0 - std::fabs(cosHalfAngle) < threshold)
		{
			weightA = 1.0 - t;
			weightB = t;
		}
		else
		{
			T halfAngle = std::acos(cosHalfAngle);
			T sinHalfAngle = std::sqrt(1 - cosHalfAngle * cosHalfAngle);

			// In case of angle ~ 180, pick middle value.
			// If not, perform slerp.
			if (std::fabs(sinHalfAngle) < eps)
			{
				weightA = static_cast<T>(0.5);
				weightB = static_cast<T>(0.5);
			}
			else
			{
				weightA = std::sin((1 - t) * halfAngle) / sinHalfAngle;
				weightB = std::sin(t * halfAngle) / sinHalfAngle;
			}
		}

		return Quaternion<T>(
			weightA * a.w + weightB * b.w,
			weightA * a.x + weightB * b.x,
			weightA * a.y + weightB * b.y,
			weightA * a.z + weightB * b.z);
	}

	template <typename T>
	Vector<T, 3> operator*(const Quaternion<T>& q, const Vector<T, 3>& v)
	{
		return q.Mul(v);
	}
	
	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b)
	{
		return a.Mul(b);
	}
}

#endif