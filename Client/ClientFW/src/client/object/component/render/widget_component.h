#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class UserInterfaceLayer;

	// World�� WidgetComponent�� ���� ��Ŀ� ���� �����˴ϴ�.
	// Billboard�� ī�޶� �ٶ󺸰�
	// FixUp�� Y���� �����˴ϴ�.
	enum class eWidgetSpaceType
	{
		kWorld, kBillboard, kFixUpBillboard
	};

	enum class eWidgetBlendMode
	{
		kOpaque, kMasked, kTransparent
	};

	class WidgetComponent : public RenderComponent
	{
	public:
		WidgetComponent(const SPtr<UserInterfaceLayer>& ui_layer, const std::string& name = "widget component",
			eWidgetSpaceType type = eWidgetSpaceType::kBillboard,
			eWidgetBlendMode blend_mode = eWidgetBlendMode::kMasked);
		virtual ~WidgetComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
		virtual void UpdateOrientedBox() override;

	private:
		eWidgetSpaceType m_type;
		eWidgetBlendMode m_blend_mode;
		SPtr<UserInterfaceLayer> m_user_interface_layer;
		Vec2 m_size;
		Vec2 m_pivot;

	public:
		eWidgetSpaceType GetWidgetSpaceType() const { return m_type; }
		eWidgetBlendMode GetWidgetBlendMode() const { return m_blend_mode; }
		const SPtr<UserInterfaceLayer>& GetUserInterfaceLayer() const { return m_user_interface_layer; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size);

		const Vec2& GetPivot() const { return m_pivot; }
		// 0.0~1.0 ���� �Է� ���� ���� ����, ��� ����, ������ ���İ� ���� ����̶� ���� �ȴ�.
		void SetPivot(const Vec2& pivot);

	protected:
		SPtr<WidgetComponent> SharedFromThis();
	};

}

