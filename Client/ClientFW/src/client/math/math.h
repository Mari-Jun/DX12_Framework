#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#include "client/math/vec2.h"
#include "client/math/ivec2.h"

#include "client/math/vec3.h"

#include "client/math/vec4.h"
#include "client/math/ivec4.h"

#include "client/math/mat3.h"
#include "client/math/mat4.h"

#include "client/math/quaternion.h"

namespace client_fw
{
	namespace math
	{
		inline bool NearZero(float value, float spsilon = 0.0001f)
		{
			return (fabs(value) <= spsilon);
		}

		inline float ToRadian(float degrees)
		{
			return degrees * XM_PI / 180.0f;
		}

		inline float ToDegrees(float radian)
		{
			return radian * 180.0f / XM_PI;
		}

		inline float Lerp(float f1, float f2, float value)
		{
			return f1 + (f2 - f1) * value;
		}
	}
	
	namespace vec2
	{
		inline Vec2 Normalize(const Vec2& v)
		{
			Vec2 ret;
			XMStoreFloat2(&ret, XMVector2Normalize(XMLoadFloat2(&v)));
			return ret;
		}

		inline float Length(const Vec2& v)
		{
			float ret;
			XMStoreFloat(&ret, XMVector2Length(XMLoadFloat2(&v)));
			return ret;
		}

		inline float LengthSq(const Vec2& v)
		{
			float ret;
			XMStoreFloat(&ret, XMVector2LengthSq(XMLoadFloat2(&v)));
			return ret;
		}

		inline float BetweenAngle(const Vec2& v1, const Vec2& v2)
		{
			float ret;
			XMStoreFloat(&ret, XMVector2AngleBetweenVectors(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
			return ret;
		}

		inline float Dot(const Vec2& v1, const Vec2& v2)
		{
			float ret;
			XMStoreFloat(&ret, XMVector2Dot(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
			return ret;
		}

		inline Vec2 Reflect(const Vec2& v, const Vec2& n)
		{
			Vec2 ret;
			XMStoreFloat2(&ret, XMVector2Reflect(XMLoadFloat2(&v), XMLoadFloat2(&n)));
			return ret;
		}

		inline Vec2 Lerp(const Vec2& v1, const Vec2& v2, float f)
		{
			return Vec2(v1 + (v2 - v1) * f);
		}

		inline Vec2 TransformNormal(const Vec2& vec, const Mat3& mat)
		{
			Vec2 ret;
			XMStoreFloat2(&ret, XMVector2TransformNormal(XMLoadFloat2(&vec), XMLoadFloat3x3(&mat)));
			return ret;
		}

		inline Vec2 TransformCoord(const Vec2& vec, const Mat3& mat)
		{
			Vec2 ret;
			XMStoreFloat2(&ret, XMVector2TransformCoord(XMLoadFloat2(&vec), XMLoadFloat3x3(&mat)));
			return ret;
		}
	}

	namespace vec3
	{
		inline Vec3 Normalize(const Vec3& v)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3Normalize(XMLoadFloat3(&v)));
			return ret;
		}

		inline float Length(const Vec3& v)
		{
			float ret;
			XMStoreFloat(&ret, XMVector3Length(XMLoadFloat3(&v)));
			return ret;
		}

		inline float LengthSq(const Vec3& v)
		{
			float ret;
			XMStoreFloat(&ret, XMVector3LengthSq(XMLoadFloat3(&v)));
			return ret;
		}

		inline float BetweenAngle(const Vec3& v1, const Vec3& v2)
		{
			float ret;
			XMStoreFloat(&ret, XMVector3AngleBetweenVectors(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
			return ret;
		}

		inline float Dot(const Vec3& v1, const Vec3& v2)
		{
			float ret;
			XMStoreFloat(&ret, XMVector3Dot(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
			return ret;
		}

		inline Vec3 Cross(const Vec3& v1, const Vec3& v2, bool normalize)
		{
			Vec3 ret;
			if (normalize)
				XMStoreFloat3(&ret, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&v1), XMLoadFloat3(&v2))));
			else
				XMStoreFloat3(&ret, XMVector3Cross(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
			return ret;
		}

		inline Vec3 Reflect(const Vec3& v, const Vec3& n)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3Reflect(XMLoadFloat3(&v), XMLoadFloat3(&n)));
			return ret;
		}

		inline Vec3 Lerp(const Vec3& v1, const Vec3& v2, float f)
		{
			return Vec3(v1 + (v2 - v1) * f);
		}

		inline Vec3 TransformNormal(const Vec3& vec, const Mat4& mat)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3TransformNormal(XMLoadFloat3(&vec), XMLoadFloat4x4(&mat)));
			return ret;
		}

		inline Vec3 TransformCoord(const Vec3& vec, const Mat4& mat)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3TransformCoord(XMLoadFloat3(&vec), XMLoadFloat4x4(&mat)));
			return ret;
		}

		inline Vec3 TransformNormal(const Vec3& vec, const Quaternion& quat)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3TransformNormal(XMLoadFloat3(&vec), XMMatrixRotationQuaternion(XMLoadFloat4(&quat))));
			return ret;
		}

		inline Vec3 TransformCoord(const Vec3& vec, const Quaternion& quat)
		{
			Vec3 ret;
			XMStoreFloat3(&ret, XMVector3TransformCoord(XMLoadFloat3(&vec), XMMatrixRotationQuaternion(XMLoadFloat4(&quat))));
			return ret;
		}
	}

	namespace vec4
	{
		inline Vec4 Transform(const Vec4& vec, const Mat4& mat)
		{
			Vec4 ret;
			XMStoreFloat4(&ret, XMVector4Transform(XMLoadFloat4(&vec), XMLoadFloat4x4(&mat)));
			return ret;
		}
	}

	namespace mat3
	{
		inline float GetDeterminant(const Mat3& mat)
		{
			float ret;
			XMStoreFloat(&ret, XMMatrixDeterminant(XMLoadFloat3x3(&mat)));
			return ret;
		}

		inline Mat3 CreateScale(float x, float y)
		{
			Mat3 ret;
			XMStoreFloat3x3(&ret, XMMatrixScaling(x, y, 1.0f));
			return ret;
		}

		inline Mat3 CreateScale(float scale)
		{
			return CreateScale(scale, scale);
		}

		inline Mat3 CreateScale(const Vec2& vec)
		{
			Mat3 ret;
			XMStoreFloat3x3(&ret, XMMatrixScalingFromVector(XMLoadFloat2(&vec)));
			return ret;
		}

		inline Mat3 CreateRotation(float radian)
		{
			Mat3 ret;
			XMStoreFloat3x3(&ret, XMMatrixRotationZ(radian));
			return ret;
		}

		inline Mat3 CreateTranslation(float x, float y)
		{
			Mat3 ret;
			XMStoreFloat3x3(&ret, XMMatrixTranslation(x, y, 0.0f));
			return ret;
		}

		inline Mat3 CreateTranslation(const Vec2& vec)
		{
			Mat3 ret;
			XMStoreFloat3x3(&ret, XMMatrixTranslationFromVector(XMLoadFloat2(&vec)));
			return ret;
		}
	}

	namespace mat4
	{
		inline float GetDeterminant(const Mat4& mat)
		{
			float ret;
			XMStoreFloat(&ret, XMMatrixDeterminant(XMLoadFloat4x4(&mat)));
			return ret;
		}

		inline Mat4 CreateScale(float x, float y, float z)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixScaling(x, y, z));
			return ret;
		}

		inline Mat4 CreateScale(float scale)
		{
			return CreateScale(scale, scale, scale);
		}

		inline Mat4 CreateScale(const Vec3& vec)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixScalingFromVector(XMLoadFloat3(&vec)));
			return ret;
		}

		inline Mat4 CreateRotationX(float radian)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixRotationX(radian));
			return ret;
		}

		inline Mat4 CreateRotationY(float radian)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixRotationY(radian));
			return ret;
		}

		inline Mat4 CreateRotationZ(float radian)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixRotationZ(radian));
			return ret;
		}

		inline Mat4 CreateRotationFromQuaternion(const Quaternion& q)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixRotationQuaternion(XMLoadFloat4(&q)));
			return ret;
		}

		inline Mat4 CreateTranslation(float x, float y, float z)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixTranslation(x, y, z));
			return ret;
		}

		inline Mat4 CreateTranslation(const Vec3& vec)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixTranslationFromVector(XMLoadFloat3(&vec)));
			return ret;
		}

		inline Mat4 LookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));
			return ret;
		}

		inline Mat4 Ortho(float left, float right, float bottom, float top, float near_z, float far_z)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixOrthographicLH(right - left, bottom - top, near_z, far_z));
			return ret;
		}

		inline Mat4 Ortho(float width, float height, float near_z, float far_z)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixOrthographicLH(width, height, near_z, far_z));
			return ret;
		}

		inline Mat4 Perspective(float fovy, float aspect, float near_z, float far_z)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixPerspectiveFovLH(fovy, aspect, near_z, far_z));
			return ret;
		}

		inline Mat4 Inverse(const Mat4& mat)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixInverse(nullptr, XMLoadFloat4x4(&mat)));
			return ret;
		}

		inline Mat4 InverseVec(const Mat4& mat)
		{
			XMMATRIX matrix = XMLoadFloat4x4(&mat);
			matrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixInverse(&XMMatrixDeterminant(matrix), matrix));
			return ret;
		}

		inline Mat4 Transpose(const Mat4& mat)
		{
			Mat4 ret;
			XMStoreFloat4x4(&ret, XMMatrixTranspose(XMLoadFloat4x4(&mat)));
			return ret;
		}

		inline Mat4 InverseTranspose(const Mat4& mat)
		{
			return Transpose(Inverse(mat));
		}
	}

	namespace quat
	{
		inline Quaternion CreateQuaternionFromAxis(const Vec3& v, float radian)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionRotationAxis(XMLoadFloat3(&v), radian));
			return ret;
		}

		inline Quaternion CreateQuaternionFromNormal(const Vec3& v, float radian)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionRotationNormal(XMLoadFloat3(&v), radian));
			return ret;
		}

		inline Quaternion CreateQuaternionFromRollPitchYaw(float pitch, float yaw, float roll)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
			return ret;
		}

		inline Quaternion Conjugate(const Quaternion& q)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionConjugate(XMLoadFloat4(&q)));
			return ret;
		}

		inline float Length(const Quaternion& q)
		{
			float ret;
			XMStoreFloat(&ret, XMQuaternionLength(XMLoadFloat4(&q)));
			return ret;
		}

		inline Quaternion Normalize(const Quaternion& q)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionNormalize(XMLoadFloat4(&q)));
			return ret;
		}

		inline float Dot(const Quaternion& q1, const Quaternion& q2)
		{
			float ret;
			XMStoreFloat(&ret, XMQuaternionDot(XMLoadFloat4(&q1), XMLoadFloat4(&q2)));
			return ret;
		}

		inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
		{
			Quaternion ret;
			XMStoreFloat4(&ret, XMQuaternionSlerp(XMLoadFloat4(&q1), XMLoadFloat4(&q2), t));
			return ret;
		}

		inline void GetAxisRadianFromQuaternion(Vec3& axis, float& radian, const Quaternion& q)
		{
			XMQuaternionToAxisAngle(&XMLoadFloat3(&axis), &radian, XMLoadFloat4(&q));
		}

		inline Vec3 QuaternionToEuler(const Quaternion& q)
		{
			float sqx = q.x * q.x;
			float sqy = q.y * q.y;
			float sqz = q.z * q.z;
			float sqw = q.w * q.w;

			float pitch = std::asinf(std::clamp(2.0f * (q.w * q.x - q.y * q.z), -1.0f, 1.0f));
			float yaw = std::atan2(2.0f * (q.x * q.z + q.w * q.y), -sqx - sqy + sqz + sqw);
			float roll = std::atan2(2.0f * (q.x * q.y + q.w * q.z), -sqx + sqy - sqz + sqw);

			return Vec3(pitch, yaw, roll);
		}
	}
}