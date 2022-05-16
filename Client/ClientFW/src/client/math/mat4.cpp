#include "stdafx.h"
#include "client/math/math.h"

namespace client_fw
{
	Mat4& Mat4::operator*=(const Mat4& rhs)
	{
		XMStoreFloat4x4(this, XMMatrixMultiply(XMLoadFloat4x4(this), XMLoadFloat4x4(&rhs)));
		return *this;
	}

	void Mat4::Inverse()
	{
		*this = mat4::Inverse(*this);
	}

	void Mat4::Transpose()
	{
		*this = mat4::Transpose(*this);
	}

	void Mat4::InverseTranspose()
	{
		*this = mat4::InverseTranspose(*this);
	}

	std::string Mat4::ToString() const
	{
		std::stringstream result;
		result << "Mat4\n";
		result << "{" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "}\n";
		result << "{" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "}\n";
		result << "{" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "}\n";
		result << "{" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "}\n";
		return result.str();
	}

	Mat4 operator*(const Mat4& m1, const Mat4& m2)
	{
		Mat4 ret(m1);
		ret *= m2;
		return ret;
	}

	std::ostream& operator<<(std::ostream& os, const Mat4& mat)
	{
		return os << mat.ToString();
	}
}
