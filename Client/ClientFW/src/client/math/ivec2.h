#pragma once

namespace client_fw
{
	class IVec2 : public XMINT2
	{
	public:
		explicit IVec2();
		explicit IVec2(int x, int y);
		explicit IVec2(const Vec2& vec);

		IVec2& operator+=(const IVec2& rhs);
		IVec2& operator-=(const IVec2& rhs);
		IVec2& operator*=(const IVec2& rhs);
		IVec2& operator*=(int scalar);
		IVec2& operator/=(const IVec2& rhs);
		IVec2& operator/=(int scalar);

		std::string ToString() const;
	};

	IVec2 operator+(const IVec2& v1, const IVec2& v2);
	IVec2 operator-(const IVec2& v1, const IVec2& v2);
	IVec2 operator*(const IVec2& v1, const IVec2& v2);
	IVec2 operator*(const IVec2& v, int scalar);
	IVec2 operator*(int scalar, const IVec2& v);
	IVec2 operator/(const IVec2& v1, const IVec2& v2);
	IVec2 operator/(const IVec2& v, int scalar);
	IVec2 operator/(int scalar, const IVec2& v);

	bool operator==(const IVec2& v1, const IVec2& v2);
	bool operator!=(const IVec2& v1, const IVec2& v2);
	bool operator<(const IVec2& v1, const IVec2& v2);
	bool operator<=(const IVec2& v1, const IVec2& v2);
	bool operator>(const IVec2& v1, const IVec2& v2);
	bool operator>=(const IVec2& v1, const IVec2& v2);

	std::ostream& operator<<(std::ostream& os, const IVec2& v);
}


