#pragma once
#include <DirectXCollision.h>

namespace client_fw
{
	class BBox;
	class BOrientedBox;
	class BFrustum;
	class BSphere;

	/*struct Triangle
	{
		Vec3 v1, v2, v3;
	};*/

	namespace triangle_test
	{
		bool Intersect(const Vec3& a1, const Vec3& a2, const Vec3& a3,
			const Vec3& b1, const Vec3& b2, const Vec3& b3);
		bool Intersect(const Vec3& origin, const Vec3& direction,
			const Vec3& v1, const Vec3& v2, const Vec3& v3, float& distance);
	}

	template <class Type>
	class Bounding
	{
	public:
		void Transform(const Mat4& mat)
		{
			m_bounding.Transform(m_bounding, XMLoadFloat4x4(&mat));
		}
		void Transform(const Bounding<Type>& in, const Mat4& mat)
		{
			in.GetBounding().Transform(m_bounding, XMLoadFloat4x4(&mat));
		}

		template <class AnotherType>
		ContainmentType Contains(const Bounding<AnotherType>& bounding_mesh) const
		{
			return m_bounding.Contains(bounding_mesh.GetBounding());
		}
		template <class AnotherType>
		bool Intersects(const Bounding<AnotherType>& bounding_mesh) const
		{
			return m_bounding.Intersects(bounding_mesh.GetBounding());
		}

		ContainmentType Contains(const Vec3& point) const
		{
			return m_bounding.Contains(XMLoadFloat3(&point));
		}

		ContainmentType Contains(const Vec3& v1, const Vec3& v2, const Vec3& v3) const
		{
			return m_bounding.Contains(XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3));
		}
		bool Intersects(const Vec3& v1, const Vec3& v2, const Vec3& v3) const
		{
			return m_bounding.Intersects(XMLoadFloat3(&v1), XMLoadFloat3(&v2), XMLoadFloat3(&v3));
		}
		bool Intersects(const Vec3& origin, const Vec3& direction, float& distance) const
		{
			return m_bounding.Intersects(XMLoadFloat3(&origin), XMLoadFloat3(&direction), distance);
		}

	protected:
		Type m_bounding;

	public:
		const Type& GetBounding() const { return m_bounding; }
	};

	class BBox : public Bounding<BoundingBox>
	{
	public:
		BBox(Vec3 center = vec3::ZERO, Vec3 extents = Vec3(1.f, 1.f, 1.f));
		BBox(std::vector<Vec3>&& positions);

	public:
		Vec3 GetCenter() const { return Vec3(m_bounding.Center); }
		Vec3 GetExtents() const { return Vec3(m_bounding.Extents); }
		void SetCenter(const Vec3& center) { m_bounding.Center = center; }
		void SetExtents(const Vec3& extents) { m_bounding.Extents = extents; }
	};

	class BOrientedBox : public Bounding<BoundingOrientedBox>
	{
	public:
		BOrientedBox(Vec3 center = vec3::ZERO, Vec3 extents = Vec3(1.f, 1.f, 1.f));
		BOrientedBox(const std::vector<Vec3>& positions, bool box_to_obox = true);
		BOrientedBox(std::vector<Vec3>&& positions, bool box_to_obox = true);
		BOrientedBox(const BoundingBox& box);

		Vec3 GetCenter() const { return Vec3(m_bounding.Center); }
		Vec3 GetExtents() const { return Vec3(m_bounding.Extents); }
		void SetCenter(const Vec3& center) { m_bounding.Center = center; }
		void SetExtents(const Vec3& extents) { m_bounding.Extents = extents; }

		std::array<Vec3, 8> GetCorners() const;
	};

	class BFrustum : public Bounding<BoundingFrustum>
	{
	public:
		BFrustum(Mat4 mat = mat4::IDENTITY);

		std::array<Vec3, 8> GetCorners() const;
	};

	class BSphere : public Bounding<BoundingSphere>
	{
	public:
		BSphere(Vec3 center = vec3::ZERO, float radius = 1.f);
		BSphere(const BFrustum& frustum);

		Vec3 GetCenter() const { return Vec3(m_bounding.Center); }
		float GetRadius() const { return m_bounding.Radius; }
		void SetCenter(const Vec3& center) { m_bounding.Center = center; }
		void SetRadius(float radius) { m_bounding.Radius = radius; }
	};
}



