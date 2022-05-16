#pragma once

namespace client_fw
{
	class Mat3;

	class Vec2 : public XMFLOAT2
	{
	public:
		constexpr explicit Vec2() : XMFLOAT2(0.0f, 0.0f) {}
		constexpr explicit Vec2(float x, float y) : XMFLOAT2(x, y) {}

		Vec2& operator+=(const Vec2& rhs);
		Vec2& operator-=(const Vec2& rhs);
		Vec2& operator*=(const Vec2& rhs);
		Vec2& operator*=(float scalar);
		Vec2& operator/=(const Vec2& rhs);
		Vec2& operator/=(float scalar);
		
		void Normalize();
		
		float Length() const;
		float LengthSq() const;
		
		void TransformNormal(const Mat3& mat);
		void TransformCoord(const Mat3& mat);

		std::string ToString() const;
	};

	Vec2 operator+(const Vec2& v1, const Vec2& v2);
	Vec2 operator-(const Vec2& v1, const Vec2& v2);
	Vec2 operator*(const Vec2& v1, const Vec2& v2);
	Vec2 operator*(const Vec2& v, float scalar);
	Vec2 operator*(float scalar, const Vec2& v);
	Vec2 operator/(const Vec2& v1, const Vec2& v2);
	Vec2 operator/(const Vec2& v, float scalar);
	Vec2 operator/(float scalar, const Vec2& v);

	bool operator==(const Vec2& v1, const Vec2& v2);
	bool operator!=(const Vec2& v1, const Vec2& v2);
	bool operator<(const Vec2& v1, const Vec2& v2);
	bool operator<=(const Vec2& v1, const Vec2& v2);
	bool operator>(const Vec2& v1, const Vec2& v2);
	bool operator>=(const Vec2& v1, const Vec2& v2);

	std::ostream& operator<<(std::ostream& os, const Vec2& v);	

	namespace vec2
	{
		constexpr Vec2 ZERO{ 0.0f, 0.0f };
		constexpr Vec2 AXIS_X{ 1.0f, 0.0f };
		constexpr Vec2 AXIS_Y{ 0.0f, 1.0f };
		constexpr Vec2 NEG_AXIS_X{ -1.0f, 0.0f };
		constexpr Vec2 NEG_AXIS_Y{ 0.0f, -1.0f };
	}

}

