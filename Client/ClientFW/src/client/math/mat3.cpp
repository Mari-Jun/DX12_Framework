#include "stdafx.h"
#include "client/math/math.h"

namespace client_fw
{
	Mat3& Mat3::operator*=(const Mat3& rhs)
	{
		XMStoreFloat3x3(this, XMMatrixMultiply(XMLoadFloat3x3(this), XMLoadFloat3x3(&rhs)));
		return *this;
	}

	std::string Mat3::ToString() const
	{
		std::stringstream result;
		result << "Mat3\n";
		result << "{" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "}\n";
		result << "{" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << "}\n";
		result << "{" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << "}\n";
		return result.str();
	}

	Mat3 operator*(const Mat3& m1, const Mat3& m2)
	{
		Mat3 ret(m1);
		ret *= m2;
		return ret;
	}

	std::ostream& operator<<(std::ostream& os, const Mat3& mat)
	{
		return os << mat.ToString();
	}
}
