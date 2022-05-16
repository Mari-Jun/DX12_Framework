#include "stdafx.h"
#include "client/math/math.h"

namespace client_fw
{
	IVec2::IVec2() : XMINT2(0, 0) {}

	IVec2::IVec2(int x, int y) : XMINT2(x, y) {}

	IVec2::IVec2(const Vec2& vec) : XMINT2(static_cast<INT>(vec.x), static_cast<INT>(vec.y)) {}

	IVec2& IVec2::operator+=(const IVec2& rhs)
	{
		XMStoreSInt2(this, XMVectorAdd(XMLoadSInt2(this), XMLoadSInt2(&rhs)));
		return *this;
	}

	IVec2& IVec2::operator-=(const IVec2& rhs)
	{
		XMStoreSInt2(this, XMVectorSubtract(XMLoadSInt2(this), XMLoadSInt2(&rhs)));
		return *this;
	}

	IVec2& IVec2::operator*=(const IVec2& rhs)
	{
		XMStoreSInt2(this, XMVectorMultiply(XMLoadSInt2(this), XMLoadSInt2(&rhs)));
		return *this;
	}

	IVec2& IVec2::operator*=(int scalar)
	{
		XMStoreSInt2(this, XMVectorScale(XMLoadSInt2(this), static_cast<float>(scalar)));
		return *this;
	}

	IVec2& IVec2::operator/=(const IVec2& rhs)
	{
		XMStoreSInt2(this, XMVectorDivide(XMLoadSInt2(this), XMLoadSInt2(&rhs)));
		return *this;
	}

	IVec2& IVec2::operator/=(int scalar)
	{
		XMStoreSInt2(this, XMVectorScale(XMLoadSInt2(this), (1.0f / static_cast<float>(scalar))));
		return *this;
	}

	std::string IVec2::ToString() const
	{
		std::stringstream ret;
		ret << "Vec2 : {" << x << ", " << y << "}";
		return ret.str();
	}

	IVec2 operator+(const IVec2& v1, const IVec2& v2)
	{
		IVec2 ret(v1);
		ret += v2;
		return ret;
	}

	IVec2 operator-(const IVec2& v1, const IVec2& v2)
	{
		IVec2 ret(v1);
		ret -= v2;
		return ret;
	}

	IVec2 operator*(const IVec2& v1, const IVec2& v2)
	{
		IVec2 ret(v1);
		ret *= v2;
		return ret;
	}

	IVec2 operator*(const IVec2& v, int scalar)
	{
		IVec2 ret(v);
		ret *= scalar;
		return ret;
	}

	IVec2 operator*(int scalar, const IVec2& v)
	{
		return v * scalar;
	}

	IVec2 operator/(const IVec2& v1, const IVec2& v2)
	{
		IVec2 ret(v1);
		ret /= v2;
		return ret;
	}

	IVec2 operator/(const IVec2& v, int scalar)
	{
		IVec2 ret(v);
		ret /= scalar;
		return ret;
	}

	IVec2 operator/(int scalar, const IVec2& v)
	{
		return v / scalar;
	}

	bool operator==(const IVec2& v1, const IVec2& v2)
	{
		return XMVector2Equal(XMLoadSInt2(&v1), XMLoadSInt2(&v2));
	}

	bool operator!=(const IVec2& v1, const IVec2& v2)
	{
		return !(v1 == v2);
	}

	bool operator<(const IVec2& v1, const IVec2& v2)
	{
		return XMVector2Less(XMLoadSInt2(&v1), XMLoadSInt2(&v2));
	}

	bool operator<=(const IVec2& v1, const IVec2& v2)
	{
		return XMVector2LessOrEqual(XMLoadSInt2(&v1), XMLoadSInt2(&v2));
	}

	bool operator>(const IVec2& v1, const IVec2& v2)
	{
		return XMVector2Greater(XMLoadSInt2(&v1), XMLoadSInt2(&v2));
	}

	bool operator>=(const IVec2& v1, const IVec2& v2)
	{
		return XMVector2GreaterOrEqual(XMLoadSInt2(&v1), XMLoadSInt2(&v2));
	}

	std::ostream& operator<<(std::ostream& os, const IVec2& v)
	{
		return os << v.ToString();
	}
}