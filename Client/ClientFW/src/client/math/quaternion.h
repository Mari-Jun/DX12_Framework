#pragma once

namespace client_fw
{
	class Quaternion : public XMFLOAT4
	{
	public:
		constexpr explicit Quaternion() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
		constexpr explicit Quaternion(float x, float y, float z, float w)
			: XMFLOAT4(x, y, z, w) {}
		constexpr explicit Quaternion(const Vec3& axis, float radian, bool normalize = true);

		Quaternion operator*=(const Quaternion& q);

		void Normalize();

		float Length() const;

		std::string ToString() const;
	};

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	bool operator==(const Quaternion& q1, const Quaternion& q2);
	bool operator!=(const Quaternion& q1, const Quaternion& q2);

	std::ostream& operator<<(std::ostream& os, const Quaternion& q);

}

