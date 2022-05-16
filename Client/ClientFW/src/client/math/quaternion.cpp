#include "stdafx.h"
#include "client/math/quaternion.h"

namespace client_fw
{
	constexpr Quaternion::Quaternion(const Vec3& axis, float radian, bool normalize)
	{
		if (normalize)
			*this = quat::CreateQuaternionFromNormal(axis, radian);
		else
			*this = quat::CreateQuaternionFromAxis(axis, radian);
	}

	Quaternion Quaternion::operator*=(const Quaternion& q)
	{
		XMStoreFloat4(this, XMQuaternionMultiply(XMLoadFloat4(this), XMLoadFloat4(&q)));
		return *this;
	}

	void Quaternion::Normalize()
	{
		*this = quat::Normalize(*this);
	}

	float Quaternion::Length() const
	{
		return quat::Length(*this);
	}

	std::string Quaternion::ToString() const
	{
		std::stringstream result;
		result << "Quaternion : {" << x << ", " << y << ", " << z << ", " << w << "}";
		return result.str();
	}

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		Quaternion ret(q1);
		ret *= q2;
		return ret;
	}

	bool operator==(const Quaternion& q1, const Quaternion& q2)
	{
		return XMQuaternionEqual(XMLoadFloat4(&q1), XMLoadFloat4(&q2));
	}

	bool operator!=(const Quaternion& q1, const Quaternion& q2)
	{
		return !(q1 == q2);
	}

	std::ostream& operator<<(std::ostream& os, const Quaternion& q)
	{
		return os << q.ToString();
	}
}
