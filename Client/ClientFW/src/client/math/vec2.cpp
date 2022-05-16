#include "stdafx.h"
#include "client/math/math.h"

namespace client_fw
{

	Vec2& Vec2::operator+=(const Vec2& rhs)
	{	
		XMStoreFloat2(this, XMVectorAdd(XMLoadFloat2(this), XMLoadFloat2(&rhs)));
		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& rhs)
	{
		XMStoreFloat2(this, XMVectorSubtract(XMLoadFloat2(this), XMLoadFloat2(&rhs)));
		return *this;
	}

	Vec2& Vec2::operator*=(const Vec2& rhs)
	{
		XMStoreFloat2(this, XMVectorMultiply(XMLoadFloat2(this), XMLoadFloat2(&rhs)));
		return *this;
	}

	Vec2& Vec2::operator*=(float scalar)
	{
		XMStoreFloat2(this, XMVectorScale(XMLoadFloat2(this), scalar));
		return *this;
	}

	Vec2& Vec2::operator/=(const Vec2& rhs)
	{
		XMStoreFloat2(this, XMVectorDivide(XMLoadFloat2(this), XMLoadFloat2(&rhs)));
		return *this;
	}

	Vec2& Vec2::operator/=(float scalar)
	{
		XMStoreFloat2(this, XMVectorScale(XMLoadFloat2(this), (1.0f / scalar)));
		return *this;
	}

	void Vec2::Normalize()
	{
		*this = vec2::Normalize(*this);
	}

	float Vec2::Length() const
	{
		return vec2::Length(*this);
	}

	float Vec2::LengthSq() const
	{
		return vec2::LengthSq(*this);
	}

	void Vec2::TransformNormal(const Mat3& mat)
	{
		*this = vec2::TransformNormal(*this, mat);
	}

	void Vec2::TransformCoord(const Mat3& mat)
	{
		*this = vec2::TransformCoord(*this, mat);
	}

	std::string Vec2::ToString() const
	{
		std::stringstream ret;
		ret << "Vec2 : {" << x << ", " << y << "}";
		return ret.str();
	}

	Vec2 operator+(const Vec2& v1, const Vec2& v2)
	{
		Vec2 ret(v1);
		ret += v2;
		return ret;
	}

	Vec2 operator-(const Vec2& v1, const Vec2& v2)
	{
		Vec2 ret(v1);
		ret -= v2;
		return ret;
	}

	Vec2 operator*(const Vec2& v1, const Vec2& v2)
	{
		Vec2 ret(v1);
		ret *= v2;
		return ret;
	}

	Vec2 operator*(const Vec2& v, float scalar)
	{
		Vec2 ret(v);
		ret *= scalar;
		return ret;
	}

	Vec2 operator*(float scalar, const Vec2& v)
	{
		return v * scalar;
	}

	Vec2 operator/(const Vec2& v1, const Vec2& v2)
	{
		Vec2 ret(v1);
		ret /= v2;
		return ret;
	}

	Vec2 operator/(const Vec2& v, float scalar)
	{
		Vec2 ret(v);
		ret /= scalar;
		return ret;
	}

	Vec2 operator/(float scalar, const Vec2& v)
	{
		return v / scalar;
	}

	bool operator==(const Vec2& v1, const Vec2& v2)
	{
		return XMVector2Equal(XMLoadFloat2(&v1), XMLoadFloat2(&v2));
	}

	bool operator!=(const Vec2& v1, const Vec2& v2)
	{
		return !(v1 == v2);
	}

	bool operator<(const Vec2& v1, const Vec2& v2)
	{
		return XMVector2Less(XMLoadFloat2(&v1), XMLoadFloat2(&v2));
	}

	bool operator<=(const Vec2& v1, const Vec2& v2)
	{
		return XMVector2LessOrEqual(XMLoadFloat2(&v1), XMLoadFloat2(&v2));
	}

	bool operator>(const Vec2& v1, const Vec2& v2)
	{
		return XMVector2Greater(XMLoadFloat2(&v1), XMLoadFloat2(&v2));
	}

	bool operator>=(const Vec2& v1, const Vec2& v2)
	{
		return XMVector2GreaterOrEqual(XMLoadFloat2(&v1), XMLoadFloat2(&v2));
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& v)
	{
		return os << v.ToString();
	}
}
