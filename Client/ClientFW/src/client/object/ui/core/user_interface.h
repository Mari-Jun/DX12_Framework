#pragma once
namespace client_fw
{
	class Texture;

	class UITexture
	{
	public:
		UITexture(const Vec2& position, const Vec2& size, const SPtr<Texture>& texture,
			const Vec4& color = Vec4(1.f, 1.f, 1.f, 1.f),
			const Vec2& coord = vec2::ZERO, const Vec2& tilling = Vec2(1.f, 1.f))
			: m_position(position), m_size(size), m_texture(texture)
			, m_brush_color(color), m_coordinate(coord), m_tilling(tilling)
		{}

	private:
		Vec2 m_position;
		Vec2 m_size;
		Vec4 m_brush_color;
		Vec2 m_coordinate;
		Vec2 m_tilling;
		SPtr<Texture> m_texture;

	public:
		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		const Vec4& GetBrushColor() const { return m_brush_color; }
		void SetBrushColor(const Vec4& color) { m_brush_color = color; }
		void SetBrushColor(float r, float g, float b, float a) { m_brush_color = Vec4(r, g, b, a); }
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coordinate) { m_coordinate = coordinate; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
	};

	class UserInterface : public std::enable_shared_from_this<UserInterface>
	{
	protected:
		// num of visible texture는 이 UI가 한번에 그리는 texture의 수이다.
		UserInterface(const std::string& name, const Vec2& size, size_t num_of_visible_texture);
		virtual ~UserInterface();

	public:
		virtual bool Initialize() { return true; }
		virtual void Shutdown() {}
		virtual void Update(float delta_time) {}

	protected:
		std::string m_name;
		bool m_is_activate = true;
		bool m_is_visible = true;
		Vec2 m_position;
		Vec2 m_pivot_position;
		Vec2 m_size;
		Vec2 m_pivot = Vec2(0.5f, 0.5f);

	private:
		std::vector<SPtr<UITexture>> m_visible_textures;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		bool IsActivate() const { return m_is_activate; }
		void SetActivate(bool value) { m_is_activate = value;}
		bool IsVisible() const { return m_is_visible; }
		void SetVisible(bool state) { m_is_visible = state; }
		const Vec2& GetPosition() const { return m_position; }
		const Vec2& GetPivotPosition() const { return m_pivot_position; }
		virtual void SetPosition(const Vec2& position);
		const Vec2& GetSize() const { return m_size; }
		virtual void SetSize(const Vec2& size);
		const Vec2& GetPivot() const { return m_pivot; }
		void SetPivot(const Vec2& pivot);

		UINT GetNumOfVisibleTexture() const { return static_cast<UINT>(m_visible_textures.size()); }
		const std::vector<SPtr<UITexture>> GetVisibleTextures() const { return m_visible_textures; }

		bool IsHovered() const;

	protected:
		// 생성자에서 한번에 그릴 수 있는 texture의 수를 지정했다면
		// 여기는 그 texture의 수의 index를 지정하고(1개면 index : 0) (2개면 index : 0 or 1)
		// 그릴 Texture를 설정한다.
		void SetVisibleTexture(const SPtr<UITexture>& texture, UINT index);
	};
}

