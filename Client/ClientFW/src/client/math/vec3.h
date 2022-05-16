#pragma once

namespace client_fw
{
	class Mat4;
	class Quaternion;

	class Vec3 : public XMFLOAT3
	{
	public:
		constexpr explicit Vec3() : XMFLOAT3(0.0f, 0.0f, 0.0f) {}
		constexpr explicit Vec3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
		constexpr explicit Vec3(XMFLOAT3 xmf3) : XMFLOAT3(xmf3) {}

		Vec3& operator+=(const Vec3& rhs);
		Vec3& operator-=(const Vec3& rhs);
		Vec3& operator*=(const Vec3& rhs);
		Vec3& operator*=(float scalar);
		Vec3& operator/=(const Vec3& rhs);
		Vec3& operator/=(float scalar);

		float operator[](size_t index) const;
		float& operator[](size_t index);

		void Normalize();

		float Length() const;
		float LengthSq() const;

		void TransformNormal(const Mat4& mat);
		void TransformCoord(const Mat4& mat);
		void TransformNormal(const Quaternion& quat);
		void TransformCoord(const Quaternion& quat);

		std::string ToString() const;
	};

	Vec3 operator+(const Vec3& v1, const Vec3& v2);
	Vec3 operator-(const Vec3& v1, const Vec3& v2);
	Vec3 operator*(const Vec3& v1, const Vec3& v2);
	Vec3 operator*(const Vec3& v, float scalar);
	Vec3 operator*(float scalar, const Vec3& v);
	Vec3 operator/(const Vec3& v1, const Vec3& v2);
	Vec3 operator/(const Vec3& v, float scalar);
	Vec3 operator/(float scalar, const Vec3& v);

	bool operator==(const Vec3& v1, const Vec3& v2);
	bool operator!=(const Vec3& v1, const Vec3& v2);
	bool operator<(const Vec3& v1, const Vec3& v2);
	bool operator<=(const Vec3& v1, const Vec3& v2);
	bool operator>(const Vec3& v1, const Vec3& v2);
	bool operator>=(const Vec3& v1, const Vec3& v2);

	std::ostream& operator<<(std::ostream& os, const Vec3& v);

	namespace vec3
	{
		constexpr Vec3 ZERO{ 0.0f, 0.0f, 0.0f };
		constexpr Vec3 AXIS_X{ 1.0f, 0.0f, 0.0f };
		constexpr Vec3 AXIS_Y{ 0.0f, 1.0f, 0.0f };
		constexpr Vec3 AXIS_Z{ 0.0f, 0.0f, 1.0f };
		constexpr Vec3 NEG_AXIS_X{ -1.0f, 0.0f, 0.0f };
		constexpr Vec3 NEG_AXIS_Y{ 0.0f, -1.0f, 0.0f };
		constexpr Vec3 NEG_AXIS_Z{ 0.0f, 0.0f, -1.0f };
	}
}


