#pragma once

namespace client_fw
{
	class IVec4 : public XMINT4
	{
	public:
		explicit IVec4();
		explicit IVec4(int x, int y, int z, int w);
		explicit IVec4(const Vec4& vec);

		IVec4& operator+=(const IVec4& rhs);
		IVec4& operator-=(const IVec4& rhs);
		IVec4& operator*=(const IVec4& rhs);
		IVec4& operator*=(int scalar);
		IVec4& operator/=(const IVec4& rhs);
		IVec4& operator/=(int scalar);

		std::string ToString() const;
	};

	IVec4 operator+(const IVec4& v1, const IVec4& v2);
	IVec4 operator-(const IVec4& v1, const IVec4& v2);
	IVec4 operator*(const IVec4& v1, const IVec4& v2);
	IVec4 operator*(const IVec4& v, int scalar);
	IVec4 operator*(int scalar, const IVec4& v);
	IVec4 operator/(const IVec4& v1, const IVec4& v2);
	IVec4 operator/(const IVec4& v, int scalar);
	IVec4 operator/(int scalar, const IVec4& v);

	bool operator==(const IVec4& v1, const IVec4& v2);
	bool operator!=(const IVec4& v1, const IVec4& v2);
	bool operator<(const IVec4& v1, const IVec4& v2);
	bool operator<=(const IVec4& v1, const IVec4& v2);
	bool operator>(const IVec4& v1, const IVec4& v2);
	bool operator>=(const IVec4& v1, const IVec4& v2);

	std::ostream& operator<<(std::ostream& os, const IVec4& v);
}

