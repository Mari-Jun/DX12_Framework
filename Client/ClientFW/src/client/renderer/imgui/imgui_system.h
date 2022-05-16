#pragma once

namespace client_fw
{
	class ImGuiSystem final
	{
	public:
		ImGuiSystem();
		~ImGuiSystem();

		bool Initialize(ID3D12Device* device, HWND hwnd);
		void Shutdown();

		void Update(ID3D12Device* device);
		void PreDraw(ID3D12GraphicsCommandList* command_list) const;
		void Draw(ID3D12GraphicsCommandList* command_list) const;

	private:
		void SytleForClientFW();
	};
}



