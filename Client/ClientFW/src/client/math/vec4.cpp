#include "stdafx.h"
#include "client/math/math.h"
#include "vec4.h"

namespace client_fw
{
	Vec4::Vec4(const XMVECTORF32 color)
	{
		XMStoreFloat4(this, color);
	}

	Vec4& Vec4::operator+=(const Vec4& rhs)
	{
		XMStoreFloat4(this, XMVectorAdd(XMLoadFloat4(this), XMLoadFloat4(&rhs)));
		return *this;
	}

	Vec4& Vec4::operator-=(const Vec4& rhs)
	{
		XMStoreFloat4(this, XMVectorSubtract(XMLoadFloat4(this), XMLoadFloat4(&rhs)));
		return *this;
	}

	Vec4& Vec4::operator*=(const Vec4& rhs)
	{
		XMStoreFloat4(this, XMVectorMultiply(XMLoadFloat4(this), XMLoadFloat4(&rhs)));
		return *this;
	}

	Vec4& Vec4::operator*=(float scalar)
	{
		XMStoreFloat4(this, XMVectorScale(XMLoadFloat4(this), scalar));
		return *this;
	}

	Vec4& Vec4::operator/=(const Vec4& rhs)
	{
		XMStoreFloat4(this, XMVectorDivide(XMLoadFloat4(this), XMLoadFloat4(&rhs)));
		return *this;
	}

	Vec4& Vec4::operator/=(float scalar)
	{
		XMStoreFloat4(this, XMVectorScale(XMLoadFloat4(this), (1.0f / scalar)));
		return *this;
	}

	float Vec4::operator[](size_t index) const
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: LOG_WARN("Out of range : {0}", index);  return x;
		}
	}

	float& Vec4::operator[](size_t index)
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: LOG_WARN("Out of range : {0}", index);  return x;
		}
	}

	std::string Vec4::ToString() const
	{
		std::stringstream ret;
		ret << "Vec4 : {" << x << ", " << y << ", " << z << ", " << w << "}";
		return ret.str();
	}

	Vec4 operator+(const Vec4& v1, const Vec4& v2)
	{
		Vec4 ret(v1);
		ret += v2;
		return ret;
	}

	Vec4 operator-(const Vec4& v1, const Vec4& v2)
	{
		Vec4 ret(v1);
		ret -= v2;
		return ret;
	}

	Vec4 operator*(const Vec4& v1, const Vec4& v2)
	{
		Vec4 ret(v1);
		ret *= v2;
		return ret;
	}

	Vec4 operator*(const Vec4& v, float scalar)
	{
		Vec4 ret(v);
		ret *= scalar;
		return ret;
	}

	Vec4 operator*(float scalar, const Vec4& v)
	{
		return v * scalar;
	}

	Vec4 operator/(const Vec4& v1, const Vec4& v2)
	{
		Vec4 ret(v1);
		ret /= v2;
		return ret;
	}

	Vec4 operator/(const Vec4& v, float scalar)
	{
		Vec4 ret(v);
		ret /= scalar;
		return ret;
	}

	Vec4 operator/(float scalar, const Vec4& v)
	{
		return v / scalar;
	}

	bool operator==(const Vec4& v1, const Vec4& v2)
	{
		return XMVector4Equal(XMLoadFloat4(&v1), XMLoadFloat4(&v2));
	}

	bool operator!=(const Vec4& v1, const Vec4& v2)
	{
		return !(v1 == v2);
	}

	bool operator<(const Vec4& v1, const Vec4& v2)
	{
		return XMVector4Less(XMLoadFloat4(&v1), XMLoadFloat4(&v2));
	}

	bool operator<=(const Vec4& v1, const Vec4& v2)
	{
		return XMVector4LessOrEqual(XMLoadFloat4(&v1), XMLoadFloat4(&v2));
	}

	bool operator>(const Vec4& v1, const Vec4& v2)
	{
		return XMVector4Greater(XMLoadFloat4(&v1), XMLoadFloat4(&v2));
	}

	bool operator>=(const Vec4& v1, const Vec4& v2)
	{
		return XMVector4GreaterOrEqual(XMLoadFloat4(&v1), XMLoadFloat4(&v2));
	}

	std::ostream& operator<<(std::ostream& os, const Vec4& v)
	{
		return os << v.ToString();
	}
}
