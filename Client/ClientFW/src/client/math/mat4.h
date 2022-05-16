#pragma once

namespace client_fw
{
	class Mat4 : public XMFLOAT4X4
	{
	public:
		constexpr explicit Mat4() : XMFLOAT4X4() {}
		constexpr explicit Mat4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
			: XMFLOAT4X4(m11, m12, m13, m14, m21, m22, m23, m24, 
				m31, m32, m33, m34, m41, m42, m43, m44) {}
		explicit Mat4(float* mat_array) : XMFLOAT4X4(mat_array) {}

		Mat4& operator*=(const Mat4& rhs);

		void Inverse();
		void Transpose();
		void InverseTranspose();

		std::string ToString() const;
	};

	Mat4 operator*(const Mat4& m1, const Mat4& m2);

	std::ostream& operator<<(std::ostream& os, const Mat4& mat);

	namespace mat4
	{
		constexpr Mat4 IDENTITY{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	}
}


