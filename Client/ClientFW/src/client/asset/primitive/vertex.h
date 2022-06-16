#pragma once

namespace client_fw
{
	class Vertex
	{
	public:
		Vertex();
		Vertex(const Vec3& position);

		const Vec3& GetPosition() const { return m_position; }
		void SetPosition(const Vec3& position) { m_position = position; }

	protected:
		Vec3 m_position;

	};

	class LightVertex : public Vertex
	{
	public:
		LightVertex();
		LightVertex(const Vec3& position, const Vec3& normal);

		const Vec3& GetNormal() const { return m_normal; }
		void SetNormal(const Vec3& normal) { m_normal = normal; }

	protected:
		Vec3 m_normal;
	};

	class TextureLightVertex : public LightVertex
	{
	public:
		TextureLightVertex();
		TextureLightVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord);

		const Vec2& GetTexCoord() const { return m_tex_coord; }
		void SetTexCoord(const Vec2& tex_coord) { m_tex_coord = tex_coord; }

	protected:
		Vec2 m_tex_coord;
	};

	class TextureLightNormalMapVertex : public TextureLightVertex
	{
	public:
		TextureLightNormalMapVertex();
		TextureLightNormalMapVertex(const Vec3& position, const Vec2& tex_coord,
			const Vec3& normal, const Vec3& tangent, const Vec3& bitangent);

		const Vec3& GetTangent() const { return m_tangent; }
		void SetTangent(const Vec3& tangent) { m_tangent = tangent; }
		const Vec3& GetBitangent() const { return m_bitangent; }
		void SetBitangent(const Vec3& bitangent) { m_bitangent = bitangent; }

	protected:
		Vec3 m_tangent;
		Vec3 m_bitangent;
	};

	class ShapeVertex : public Vertex
	{
	public:
		ShapeVertex();
		ShapeVertex(const Vec3& position, const Vec3& size,
			const Vec3& forward, const Vec3& right, const Vec3& up);

		const Vec3& GetSize() const { return m_size; }
		void SetSize(const Vec3& size) { m_size = size; }
		const Vec3& GetForward() const { return m_forward; }
		const Vec3& GetRight() const { return m_right; }
		const Vec3& GetUp() const { return m_up; }
		void SetForward(const Vec3& forward) { m_forward = forward; }
		void SetRight(const Vec3& right) { m_right = right; }
		void SetUp(const Vec3& up) { m_up = up; }

	protected:
		Vec3 m_size;
		Vec3 m_forward;
		Vec3 m_right;
		Vec3 m_up;
	};

	class UIVertex
	{
	public:
		UIVertex();
		UIVertex(const Vec2& position, const Vec2& size, UINT index,
			const Vec4& color, const Vec2& coordinate, const Vec2& tilling);

		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		INT GetTextureResourceIndex() const { return m_texture_resource_index; }
		void SetTextureResourceIndex(INT index) { m_texture_resource_index = index; }
		const Vec4& GetColor() const { return m_color; }
		void SetColor(const Vec4& color) { m_color = color; }
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coord) { m_coordinate = coord; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }

	protected:
		Vec2 m_position;
		Vec2 m_size;
		INT m_texture_resource_index;
		Vec4 m_color;
		Vec2 m_coordinate;
		Vec2 m_tilling;
	};

	class BillboardVertex : public Vertex
	{
	public:
		BillboardVertex();
		BillboardVertex(const Vec3& position, const Vec2& size,
			UINT index, const Vec2& offset, const Vec2& tilling);

		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		UINT GetResourceIndex() const { return m_resource_index; }
		void SetResourceIndex(UINT index) { m_resource_index = index; }
		const Vec2& GetOffset() const { return m_offset; }
		void SetOffset(const Vec2& offset) { m_offset = offset; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }

	protected:
		Vec2 m_size;
		UINT m_resource_index;
		Vec2 m_offset;
		Vec2 m_tilling;
	};

	class WidgetVertex : public Vertex
	{
	public:
		WidgetVertex();
		WidgetVertex(const Vec3& position, const Vec2& size, UINT index,
			const Vec4& color, const Vec2& coordinate, const Vec2& tilling);

		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		INT GetTextureResourceIndex() const { return m_texture_resource_index; }
		void SetTextureResourceIndex(INT index) { m_texture_resource_index = index; }
		const Vec4& GetColor() const { return m_color; }
		void SetColor(const Vec4& color) { m_color = color; }
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coord) { m_coordinate = coord; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }

	protected:
		Vec2 m_size;
		INT m_texture_resource_index;
		Vec4 m_color;
		Vec2 m_coordinate;
		Vec2 m_tilling;
	};

	class WorldWidgetVertex : public WidgetVertex
	{
	public:
		WorldWidgetVertex();
		WorldWidgetVertex(const Vec3& position, const Vec2& size, UINT index,
			const Vec4& color, const Vec2& coordinate, const Vec2& tilling,
			const Vec3& right, const Vec3& up);

		const Vec3& GetRight() const { return m_right; }
		void SetRight(const Vec3& right) { m_right = right; }
		const Vec3& GetUp() const { return m_up; }
		void SetUp(const Vec3& up) { m_up = up; }

	protected:
		Vec3 m_right;
		Vec3 m_up;
	};

	class PivotWidgetVertex : public WidgetVertex
	{
	public:
		PivotWidgetVertex();
		PivotWidgetVertex(const Vec3& position, const Vec2& size, UINT index,
			const Vec4& color, const Vec2& coordinate, const Vec2& tilling,
			const Vec2& pivot);

		const Vec2& GetPivot() const { return m_pivot; }
		void SetPivot(const Vec2& pivot) { m_pivot = pivot; }

	protected:
		Vec2 m_pivot;
	};

	class SkySphereVertex
	{
	public:
		SkySphereVertex();
		SkySphereVertex(const Vec3& center_color, const Vec3& apex_color);

		const Vec3& GetCenterColor() const { return m_center_color; }
		void SetCenterColor(const Vec3& color) { m_center_color = color; }
		const Vec3& GetApexColor() const { return m_apex_color; }
		void SetApexColor(const Vec3& color) { m_apex_color = color; }

	protected:
		Vec3 m_center_color;
		Vec3 m_apex_color;
	};
	
	class BoneVertex : public TextureLightVertex
	{
	public:
		BoneVertex();
		BoneVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord, const Vec4& bone_weight, const IVec4& bone_index);

		const Vec4& GetBoneWeight() const { return m_bone_weight; }
		void SetBoneWeight(const Vec4& bone_weight) { m_bone_weight = bone_weight; }
		const IVec4& GetBoneIndex() const { return m_bone_index; }
		void SetBoneIndex(const IVec4& bone_index) { m_bone_index = bone_index; }

	protected:
		Vec4 m_bone_weight;
		IVec4 m_bone_index;
	};
}
