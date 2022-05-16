#include "stdafx.h"
#include "client/math/vec3.h"

namespace client_fw
{
	Vec3& Vec3::operator+=(const Vec3& rhs)
	{
		XMStoreFloat3(this, XMVectorAdd(XMLoadFloat3(this), XMLoadFloat3(&rhs)));
		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& rhs)
	{
		XMStoreFloat3(this, XMVectorSubtract(XMLoadFloat3(this), XMLoadFloat3(&rhs)));
		return *this;
	}

	Vec3& Vec3::operator*=(const Vec3& rhs)
	{
		XMStoreFloat3(this, XMVectorMultiply(XMLoadFloat3(this), XMLoadFloat3(&rhs)));
		return *this;
	}

	Vec3& Vec3::operator*=(float scalar)
	{
		XMStoreFloat3(this, XMVectorScale(XMLoadFloat3(this), scalar));
		return *this;
	}

	Vec3& Vec3::operator/=(const Vec3& rhs)
	{
		XMStoreFloat3(this, XMVectorDivide(XMLoadFloat3(this), XMLoadFloat3(&rhs)));
		return *this;
	}

	Vec3& Vec3::operator/=(float scalar)
	{
		XMStoreFloat3(this, XMVectorScale(XMLoadFloat3(this), (1.0f / scalar)));
		return *this;
	}

	float Vec3::operator[](size_t index) const
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: LOG_WARN("Out of range : {0}", index);  return x;
		}
	}

	float& Vec3::operator[](size_t index)
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: LOG_WARN("Out of range : {0}", index);  return x;
		}
	}

	void Vec3::Normalize()
	{
		*this = vec3::Normalize(*this);
	}

	float Vec3::Length() const
	{
		return vec3::Length(*this);
	}

	float Vec3::LengthSq() const
	{
		return vec3::LengthSq(*this);
	}

	void Vec3::TransformNormal(const Mat4& mat)
	{
		*this = vec3::TransformNormal(*this, mat);
	}

	void Vec3::TransformCoord(const Mat4& mat)
	{
		*this = vec3::TransformCoord(*this, mat);
	}

	void Vec3::TransformNormal(const Quaternion& quat)
	{
		*this = vec3::TransformNormal(*this, quat);
	}

	void Vec3::TransformCoord(const Quaternion& quat)
	{
		*this = vec3::TransformCoord(*this, quat);
	}

	std::string Vec3::ToString() const
	{
		std::stringstream ret;
		ret << "Vec3 : {" << x << ", " << y << ", " << z << "}";
		return ret.str();
	}

	Vec3 operator+(const Vec3& v1, const Vec3& v2)
	{
		Vec3 ret(v1);
		ret += v2;
		return ret;
	}

	Vec3 operator-(const Vec3& v1, const Vec3& v2)
	{
		Vec3 ret(v1);
		ret -= v2;
		return ret;
	}

	Vec3 operator*(const Vec3& v1, const Vec3& v2)
	{
		Vec3 ret(v1);
		ret *= v2;
		return ret;
	}

	Vec3 operator*(const Vec3& v, float scalar)
	{
		Vec3 ret(v);
		ret *= scalar;
		return ret;
	}

	Vec3 operator*(float scalar, const Vec3& v)
	{
		return v * scalar;
	}

	Vec3 operator/(const Vec3& v1, const Vec3& v2)
	{
		Vec3 ret(v1);
		ret /= v2;
		return ret;
	}

	Vec3 operator/(const Vec3& v, float scalar)
	{
		Vec3 ret(v);
		ret /= scalar;
		return ret;
	}

	Vec3 operator/(float scalar, const Vec3& v)
	{
		return v / scalar;
	}

	bool operator==(const Vec3& v1, const Vec3& v2)
	{
		return XMVector3Equal(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	}

	bool operator!=(const Vec3& v1, const Vec3& v2)
	{
		return !(v1 == v2);
	}

	bool operator<(const Vec3& v1, const Vec3& v2)
	{
		return XMVector3Less(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	}

	bool operator<=(const Vec3& v1, const Vec3& v2)
	{
		return XMVector3LessOrEqual(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	}

	bool operator>(const Vec3& v1, const Vec3& v2)
	{
		return XMVector3Greater(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	}

	bool operator>=(const Vec3& v1, const Vec3& v2)
	{
		return XMVector3GreaterOrEqual(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	}

	std::ostream& operator<<(std::ostream& os, const Vec3& v)
	{
		return os << v.ToString();
	}
}
