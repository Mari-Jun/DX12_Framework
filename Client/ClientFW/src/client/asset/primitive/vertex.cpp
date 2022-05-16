#include "stdafx.h"
#include "client/asset/primitive/vertex.h"

namespace client_fw
{
	Vertex::Vertex()
		: m_position(vec3::ZERO)
	{
	}

	Vertex::Vertex(const Vec3& position)
		: m_position(position)
	{
	}

	LightVertex::LightVertex()
		: Vertex()
		, m_normal(vec3::AXIS_Y)
	{
	}

	LightVertex::LightVertex(const Vec3& position, const Vec3& normal)
		: Vertex(position)
		, m_normal(normal)
	{
	}

	TextureLightVertex::TextureLightVertex()
		: LightVertex()
		, m_tex_coord(vec2::ZERO)
	{
	}

	TextureLightVertex::TextureLightVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord)
		: LightVertex(position, normal)
		, m_tex_coord(tex_coord)
	{
	}

	TextureLightNormalMapVertex::TextureLightNormalMapVertex()
		: TextureLightVertex()
		, m_tangent(vec3::AXIS_X), m_bitangent(vec3::AXIS_Z)
	{
	}

	TextureLightNormalMapVertex::TextureLightNormalMapVertex(const Vec3& position, const Vec2& tex_coord,
		const Vec3& normal, const Vec3& tangent, const Vec3& bitangent)
		: TextureLightVertex(position, normal, tex_coord)
		, m_tangent(tangent), m_bitangent(bitangent)
	{
	}

	ShapeVertex::ShapeVertex()
		: Vertex(), m_size(Vec3(32.f, 32.f, 32.f))
		, m_forward(vec3::AXIS_Z), m_right(vec3::AXIS_X), m_up(vec3::AXIS_Y)
	{
	}

	ShapeVertex::ShapeVertex(const Vec3& position, const Vec3& size,
		const Vec3& forward, const Vec3& right, const Vec3& up)
		: Vertex(position), m_size(size)
		, m_forward(forward), m_right(right), m_up(up)
	{
	}

	UIVertex::UIVertex()
		: m_position(vec2::ZERO), m_size(Vec2(100.f, 100.f))
		, m_texture_resource_index(0)
		, m_coordinate(vec2::ZERO), m_tilling(Vec2(1.f, 1.f))
	{
	}

	UIVertex::UIVertex(const Vec2& position, const Vec2& size, UINT index,
		const Vec4& color, const Vec2& coordinate, const Vec2& tilling)
		: m_position(position), m_size(size)
		, m_texture_resource_index(index), m_color(color)
		, m_coordinate(coordinate), m_tilling(tilling)
	{
	}

	BillboardVertex::BillboardVertex()
		: Vertex(), m_size(Vec2(100.0f, 100.0f))
		, m_resource_index(0), m_coordinate(vec2::ZERO)
	{
	}

	BillboardVertex::BillboardVertex(const Vec3& position, const Vec2& size,
		UINT index, const Vec2& coordinate)
		: Vertex(position), m_size(size)
		, m_resource_index(index), m_coordinate(coordinate)
	{
	}

	WidgetVertex::WidgetVertex()
		: Vertex(vec3::ZERO), m_size(Vec2(100.f, 100.f))
		, m_texture_resource_index(0)
		, m_coordinate(vec2::ZERO), m_tilling(Vec2(1.f, 1.f))
	{
	}

	WidgetVertex::WidgetVertex(const Vec3& position, const Vec2& size, UINT index,
		const Vec4& color, const Vec2& coordinate, const Vec2& tilling)
		: Vertex(position), m_size(size)
		, m_texture_resource_index(index), m_color(color)
		, m_coordinate(coordinate), m_tilling(tilling)
	{
	}

	WorldWidgetVertex::WorldWidgetVertex()
		: WidgetVertex()
		, m_right(vec3::AXIS_X), m_up(vec3::AXIS_Y)
	{
	}

	WorldWidgetVertex::WorldWidgetVertex(const Vec3& position, const Vec2& size, UINT index,
		const Vec4& color, const Vec2& coordinate, const Vec2& tilling, 
		const Vec3& right, const Vec3& up)
		: WidgetVertex(position, size, index, color, coordinate, tilling)
		, m_right(right), m_up(up)
	{
	}

	PivotWidgetVertex::PivotWidgetVertex()
	{
	}

	PivotWidgetVertex::PivotWidgetVertex(const Vec3& position, const Vec2& size, UINT index,
		const Vec4& color, const Vec2& coordinate, const Vec2& tilling, 
		const Vec2& pivot)
		: WidgetVertex(position, size, index, color, coordinate, tilling)
		, m_pivot(pivot)
	{
	}

	SkySphereVertex::SkySphereVertex()
		: m_center_color(vec3::ZERO)
		, m_apex_color(vec3::ZERO)
	{
	}

	SkySphereVertex::SkySphereVertex(const Vec3& center_color, const Vec3& apex_color)
		: m_center_color(center_color)
		, m_apex_color(apex_color)
	{
	}

	BoneVertex::BoneVertex()
		: TextureLightVertex()
		, m_bone_weight(0.0f, 0.0f, 0.0f, 0.0f)
		, m_bone_index(0, 0, 0, 0)
	{
	}

	BoneVertex::BoneVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord,
		const Vec4& bone_weight, const IVec4& bone_index)
		: TextureLightVertex(position,normal,tex_coord)
		, m_bone_weight(bone_weight)
		, m_bone_index(bone_index)
	{
	}
}