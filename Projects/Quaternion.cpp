/*************************************************************************
> File Name: Quaternion.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Quaternion class defined as q = w + xi + yj + zk.
> Created Time: 2017/03/21
> Copyright (c) 2017, Dongmin Kim
*************************************************************************/
#include<Math\Quaternion.h>

namespace CubbyFlow
{
	template <typename T>
	Quaternion::Quaternion()
	{
		// Do nothing
	}

	template <typename T>
	Quaternion::Quaternion(T newW, T newX, T newY, T newZ) 
		: w(newW), x(newX), y(newY), z(newZ)
	{
		// Do nothing
	}
	
	template <typename T>
	Quaternion::Quaternion(const std::initializer_list<T>& list)
	{
		Set(list);
	}

	template <typename T>
	Quaternion::Quaternion(const Vector3<T>& axis, T angle)
	{
		Set(axis, angle);
	}

	template <typename T>
	Quaternion::Quaternion(const Vector3<T>& from, const Vector3<T>& to)
	{
		Set(from, to);
	}

	template <typename T>
	Quaternion::Quaternion(const Vector3<T>& axis0, const Vector3<T>& axis1, const Vector3<T>& axis2)
	{
		Set(axis0, axis1, axis2);
	}

	template <typename T>
	Quaternion::Quaternion(const Matrix3x3<T>& m33)
	{
		Set(m33);
	}

	template <typename T>
	Quaternion::Quaternion(const Quaternion& other)
	{
		Set(other);
	}

	template <typename T>
	void Quaternion::Set(const Quaternion& other)
	{
		Set(other.w, other.x, other.y, other.z);
	}

	template <typename T>
	void Quaternion::Set(T newW, T newX, T newY, T newZ) 
		: w(newW), x(newX), y(newY), z(newZ)
	{
		// Do nothing
	}

	template <typename T>
	void Quaternion::Set(const std::initializer_list<T>& list)
	{
		assert(list.size() == 4);

		auto inputElem = list.begin();
		y = static_cast<T>(*(inputElem));
		x = static_cast<T>(*(++inputElem);
		y = static_cast<T>(*(++inputElem));
		z = static_cast<T>(*(++inputElem));
	}

	template <typename T>
	void Quaternion::Set(const Vector3<T>& axis, T angle)
	{
		Set(std::cos(angle / 2), axis.normalized() * std::sin(angle / 2));
	}

	template <typename T>
	void Quaternion::Set(const Vector3<T>& from, const Vector3<T>& to)
	{
		T theta = acos(from.Dot(to) / sqrt(from.SquaredLength + to.SquaredLength));
		Vector3<T> axis = from.Cross(to);
		Set(axis, theta);
	}

	template <typename T>
	void Quaternion::Set(const Matrix3x3<T>& matrix)
	{
		w = sqrt(1 + matrix(0, 0) + matrix(1, 1) + matrix(2, 2)) / 2;
		x = (matrix(2, 1) - matrix(1, 2)) / (4 * w);
		y = (matrix(0, 2) - matrix(2, 0)) / (4 * w);
		z = (matrix(1, 0) - matrix(0, 1)) / (4 * w);
	}

	template <typename T>
	void Quaternion::Set(const Vector3<T>& roationBasis0, const Vector3<T>& rotationBasis1, const Vector3<T>& rotationBasis2)
	{

	}

	template <typename T>
	template <typename U>
	Quaternion<U> Quaternion::CastTo() const
	{
		return Quaternion<U>(static_cast<U>(w), static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
	}

	template <typename T>
	Quaternion Quaternion::Normalized() const
	{
		T Length = sqrt(w * w + x * x + y * y + z * z);
		return Quaternion<T>(w / Length, x / Length, y / Length, z / Length);
	}

	template <typename T>
	Vector3<T> Quaternion::Mul(const Vector3<T>& v) const
	{
		return this * v * conjugate();
	}

	template <typename T>
	Quaternion Quaternion::Mul(const Quaternion& other) const
	{
		Vector3<T> v1 = Vector3<T>(x, y, z);
		Vector3<T> v2 = Vector3<T>(other.x, other.y, other.z);
		Vecotr3<T> res = v2 * w + v1 * other.w + v1.Cross(v2);
		return (w * other.w - v1.Dot(v2), res.x, res.y, res.z);
	}

	template <typename T>
	T Quaternion::Dot(const Quaternion<T>& other)
	{
		return w * other.w + x * other.x + y * other.y + z * other.z;
	}

	template <typename T>
	Quaternion Quaternion::RMul(const Quaternion& other) const
	{
		Vector3<T> v1 = Vector3<T>(x, y, z);
		Vector3<T> v2 = Vector3<T>(other.x, other.y, other.z);
		Vecotr3<T> res = v2 * w + v1 * other.w + v2.Cross(v1);
		return (w * other.w - v1.Dot(v2), res.x, res.y, res.z);
	}

	template <typename T>
	void Quaternion::IMul(const Quaternion& other)
	{
		Vector3<T> v1 = Vector3<T>(x, y, z);
		Vector3<T> v2 = Vector3<T>(other.x, other.y, other.z);
		Vecotr3<T> res = v2 * w + v1 * other.w + v1.Cross(v2);
		Set(w * other.w - v1.Dot(v2), res.x, res.y, res.z);
	}

	template <typename T>
	void Quaternion::SetIdentity()
	{
		Set(1, 0, 0, 0);
	}

	template <typename T>
	void Quaternion::Rotate(T angleInRadians)
	{
		Set(Axis(), Angle() + angledInRadians);
	}

	template <typename T>
	void Quaternion::Normalize()
	{
		T Length = sqrt(x * x + y * y + z * z + w * w);
		Set(w / Length, x / Length, y / Length, z / Length);
	}

	template <typename T>
	Vector3<T> Quaternion::Axis() const
	{
		assert(std::fabs(Normalized.w) > 0.0001);
		return Vector<T, 3>(Normalized.x, Normalized().y, Normalized.z) / sqrt(1 - Normalized.w * Normalized().w);
	}

	template <typename T>
	T Quaternion::Angle() const
	{
		return 2 * std::cos(Normalized().w);
	}

	template <typename T>
	void Quaternion::GetAxisAngle(Vector3<T>* axis, T* angle) const
	{
		axis = Axis();
		angle = Angle();
	}

	template <typename T>
	Quaternion Quaternion::operator*(const T a) const
	{
		return Quaternion(w * a, x * a, y * b, z * a);
	}

	template <typename T>
	Quaternion Quaternion::operator/(const T a) const
	{
		return Quaternion(w / a, x / a, y / b, z / a);
	}
	
	template <typename T>

	Quaternion Quaternion::Inverse() const
	{
		return Quaternion()
	}

	template <typename T>
	Matrix3x3<T> Quaternion::Matrix3() const
	{
		return Matrix3x3<T>(
			1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w,
			2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w,
			2 * x * z + 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y);
	}

	template <typename T>
	Matrix4x4<T> Quaternion::Matrix4() const
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
	T Quaternion::L2Norm() const
	{
		return sqrt(w * w + x * x + y * y + z * z);
	}

	template <typename T>
	Quaternion& Quaternion::operator=(const Quaternion& other)
	{
		Set(other);
		return (*this);
	}

	template <typename T>
	Quaternion& Quaternion::operator*=(const Quaternion& other)
	{
		return Mul(other);
	}

	template <typename T>
	T& Quaternion::operator[](size_t i)
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
	const T& Quaternion::operator[](size_t i) const
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
	bool Quaternion::operator==(const Quaternion& other) const
	{
		return (w == other.w && x == other.x && y == other.y && z == other.z);
	}

	template <typename T>
	bool Quaternion::operator!=(const Quaternion& other) const
	{
		return !(w == other.w && x == other.x && y == other.y && z == other.z);
	}

	template <typename T>
	Quaternion Quaternion::MakeIdentity()
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