/*************************************************************************
> File Name: Quaternion.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Quaternion class defined as q = w + xi + yj + zk.
> Created Time: 2017/03/21
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include <Math/Quaternion.h>

namespace CubbyFlow
{
	template <typename T>
	Quaternion<T>::Quaternion()
	{
		// Do nothing
	}

	template <typename T>
	Quaternion<T>::Quaternion(T newW, T newX, T newY, T newZ) 
		: w(newW), x(newX), y(newY), z(newZ)
	{
		// Do nothing
	}
	
	template <typename T>
	Quaternion<T>::Quaternion(const std::initializer_list<T>& list)
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
		Set(other.w, other.x, other.y, other.z);
	}

	template <typename T>
	void Quaternion<T>::Set(T newW, T newX, T newY, T newZ) 
	{
		w = newW;
		x = newX;
		y = newY;
		z = newZ;
	}

	template <typename T>
	void Quaternion<T>::Set(const std::initializer_list<T>& list)
	{
		assert(list.size() == 4);

		auto inputElem = list.begin();
		y = *inputElem;
		x = *(++inputElem);
		y = *(++inputElem);
		z = *(++inputElem);
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

			x = normalizedAxis.x * s;
			y = normalizedAxis.y * s;
			z = normalizedAxis.z * s;
			w = std::cos(angle / 2);
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
			w += L2Norm();

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

		set(matrix3);
	}

	template <typename T>
	void Quaternion<T>::Set(const Matrix3x3<T>& matrix)
	{
		static const T eps = std::numeric_limits<T>::epsilon();
		static const T quater = static_cast<T>(0.25);

		T onePlusTrace = matrix.trace() + 1;

		if (onePlusTrace > eps)
		{
			T S = std::sqrt(onePlusTrace) * 2;
			w = quater * S;
			x = (matrix(2, 1) - matrix(1, 2)) / S;
			y = (matrix(0, 2) - matrix(2, 0)) / S;
			z = (matrix(1, 0) - matrix(0, 1)) / S;
		}
		else if (matrix(0, 0) > matrix(1, 1) && matrix(0, 0) > matrix(2, 2))
		{
			T S = std::sqrt(1.0 + matrix(0, 0) - matrix(1, 1) - matrix(2, 2)) * 2;
			w = (matrix(2, 1) - matrix(1, 2)) / S;
			x = quater * S;
			y = (matrix(0, 1) + matrix(1, 0)) / S;
			z = (matrix(0, 2) + matrix(2, 0)) / S;
		}
		else if (matrix(1, 1) > matrix(2, 2))
		{
			T S = std::sqrt(1.0 + matrix(1, 1) - matrix(0, 0) - matrix(2, 2)) * 2;
			w = (matrix(0, 2) - matrix(2, 0)) / S;
			x = (matrix(0, 1) + matrix(1, 0)) / S;
			y = quater * S;
			z = (matrix(1, 2) + matrix(2, 1)) / S;
		}
		else
		{
			T S = std::sqrt(1.0 + matrix(2, 2) - matrix(0, 0) - matrix(1, 1)) * 2;
			w = (matrix(1, 0) - matrix(0, 1)) / S;
			x = (matrix(0, 2) + matrix(2, 0)) / S;
			y = (matrix(1, 2) + matrix(2, 1)) / S;
			z = quater * S;
		}
	}

	template <typename T>
	template <typename U>
	Quaternion<U> Quaternion<T>::CastTo() const
	{
		return Quaternion<U>(static_cast<U>(w), static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
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
		T _2xx = 2 * x * x;
		T _2yy = 2 * y * y;
		T _2zz = 2 * z * z;
		T _2xy = 2 * x * y;
		T _2xz = 2 * x * z;
		T _2xw = 2 * x * w;
		T _2yz = 2 * y * z;
		T _2yw = 2 * y * w;
		T _2zw = 2 * z * w;

		return Vector3<T>(
			(1 - _2yy - _2zz) * v.x + (_2xy - _2zw) * v.y + (_2xz + _2yw) * v.z,
			(_2xy + _2zw) * v.x + (1 - _2zz - _2xx) * v.y + (_2yz - _2xw) * v.z,
			(_2xz - _2yw) * v.x + (_2yz + _2xw) * v.y + (1 - _2yy - _2xx) * v.z);
	}

	// NOTE: Check the formula!
	template <typename T>
	Quaternion<T> Quaternion<T>::Mul(const Quaternion& other) const
	{
		Vector3<T> v1 = Vector3<T>(x, y, z);
		Vector3<T> v2 = Vector3<T>(other.x, other.y, other.z);
		Vector3<T> res = v2 * w + v1 * other.w + v1.Cross(v2);
		return Quaternion(w * other.w - v1.Dot(v2), res.x, res.y, res.z);
	}

	template <typename T>
	T Quaternion<T>::Dot(const Quaternion<T>& other)
	{
		return w * other.w + x * other.x + y * other.y + z * other.z;
	}

	// TODO: Check the formula!
	template <typename T>
	Quaternion<T> Quaternion<T>::RMul(const Quaternion& other) const
	{
		Vector3<T> v1 = Vector3<T>(x, y, z);
		Vector3<T> v2 = Vector3<T>(other.x, other.y, other.z);
		Vector3<T> res = v2 * w + v1 * other.w + v2.Cross(v1);
		return Quaternion(w * other.w - v1.Dot(v2), res.x, res.y, res.z);
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
		Set(Axis(), Angle() + angledInRadians);
	}

	template <typename T>
	void Quaternion<T>::Normalize()
	{
		T Length = sqrt(x * x + y * y + z * z + w * w);
		Set(w / Length, x / Length, y / Length, z / Length);
	}

	template <typename T>
	Vector3<T> Quaternion<T>::Axis() const
	{
		assert(std::fabs(Normalized.w) > 0.0001);
		return Vector<T, 3>(Normalized.x, Normalized().y, Normalized.z) / sqrt(1 - Normalized.w * Normalized().w);
	}

	template <typename T>
	T Quaternion<T>::Angle() const
	{
		return 2 * std::cos(Normalized().w);
	}

	template <typename T>
	void Quaternion<T>::GetAxisAngle(Vector3<T>* axis, T* angle) const
	{
		axis = Axis();
		angle = Angle();
	}

	template <typename T>
	Quaternion Quaternion<T>::operator*(const T a) const
	{
		return Quaternion(w * a, x * a, y * b, z * a);
	}

	template <typename T>
	Quaternion Quaternion<T>::operator/(const T a) const
	{
		return Quaternion(w / a, x / a, y / b, z / a);
	}
	
	template <typename T>

	Quaternion Quaternion<T>::Inverse() const
	{
		return Quaternion()
	}

	template <typename T>
	Matrix3x3<T> Quaternion<T>::Matrix3() const
	{
		return Matrix3x3<T>(
			1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w,
			2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w,
			2 * x * z + 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y);
	}

	template <typename T>
	Matrix4x4<T> Quaternion<T>::Matrix4() const
	{
		return Matrix4x4<T>(
			w, z, -y, x,
			-z, w, x, y,
			y, x, w, z,
			-x, -y, -z, w).Cross(Matrix4x4<T>(
				w, z, -y, x,
				-z, w, x, -y,
				y, -x, w, -z,
				x, y, z, w));
	}

	template <typename T>
	T Quaternion<T>::L2Norm() const
	{
		return sqrt(w * w + x * x + y * y + z * z);
	}

	template <typename T>
	Quaternion& Quaternion<T>::operator=(const Quaternion& other)
	{
		Set(other);
		return (*this);
	}

	template <typename T>
	Quaternion& Quaternion<T>::operator*=(const Quaternion& other)
	{
		return Mul(other);
	}

	template <typename T>
	T& Quaternion<T>::operator[](size_t i)
	{
		switch (i) 
		{
		case 0:
			return w;
		case 1:
			return x;
		case 2:
			return y;
		case 3:
			return z;
		}
	}

	template <typename T>
	const T& Quaternion<T>::operator[](size_t i) const
	{
		switch (i)
		{
		case 0:
			return w;
		case 1:
			return x;
		case 2:
			return y;
		case 3:
			return z;
		}
	}

	template <typename T>
	bool Quaternion<T>::operator==(const Quaternion& other) const
	{
		return (w == other.w && x == other.x && y == other.y && z == other.z);
	}

	template <typename T>
	bool Quaternion<T>::operator!=(const Quaternion& other) const
	{
		return !(w == other.w && x == other.x && y == other.y && z == other.z);
	}

	template <typename T>
	Quaternion Quaternion<T>::MakeIdentity()
	{
		Set(0, 0, 0, 1);
	}

	template <typename T>
	Quaternion<T> Slerp(
		const Quaternion<T>& a,
		const Quaternion<T>& b,
		T t)
	{
		Vector<T, 3> VecA = Vector<T, 3>(a.x, a.y, a.z);
		Vector<T, 3> VecB = Vector<T, 3>(b.x, b.y, b.z);

		double theta = std::acos(VecA.Dot(VecB) / sqrt(VecA.SquaredLength() + VecB.SquaredLength())) / 2;
		return (a * std::sin((1 - t) * theta) + b * std::sin(t * theta)) / std::sin(theta);
	}

	template <typename T>
	Vector<T, 3> operator*(const Quaternion<T>& q, const Vector<T, 3>& v)
	{
		return q.Matrix3().Cross(v);
	}
	
	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b)
	{
		return Quaterinon<T>(
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
			a.w * b.x + a.x * b.w - a.y * b.z + a.z * b.y,
			a.w * b.y + a.x * b.z + a.y * b.w - a.z * b.x,
			a.w * b.z - a.x * b.y + a.y * b.x + a.z * b.w);
	}
}