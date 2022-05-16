#pragma once

namespace client_fw
{
	class Mat3 : public XMFLOAT3X3
	{
	public:
		constexpr explicit Mat3() : XMFLOAT3X3() {}
		constexpr explicit Mat3(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33)
			: XMFLOAT3X3(m11, m12, m13, m21, m22, m23, m31, m32, m33) {}
		explicit Mat3(float* mat_array) : XMFLOAT3X3(mat_array) {}

		
		Mat3& operator*=(const Mat3& rhs);

		std::string ToString() const;
	};

	Mat3 operator*(const Mat3& m1, const Mat3& m2);

	std::ostream& operator<<(std::ostream& os, const Mat3& mat);

	namespace mat3
	{
		constexpr Mat3 Identity{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	}
}


