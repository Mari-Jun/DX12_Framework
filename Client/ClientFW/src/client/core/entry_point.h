#pragma once
#include <dxgi1_4.h>
#include <dxgidebug.h>

extern client_fw::UPtr<client_fw::Application> client_fw::CreateApplication();

void ExecuteApplication()
{
	const auto& app = client_fw::CreateApplication();
	if (app->Initialize())
	{
		app->Run();
		app->Shutdown();
	}
	else
	{
		LOG_ERROR("Could not initialize app");
	}
}

void DX12ReportLiveObject()
{
#if defined(_DEBUG)
	IDXGIDebug1* giDebug = nullptr;
	DXGIGetDebugInterface1(0, IID_PPV_ARGS(&giDebug));
	HRESULT result = giDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	giDebug->Release();
#endif
}

int main(int argc, char** argv)
{
	client_fw::Log::Initialize();

	ExecuteApplication();
	DX12ReportLiveObject();
}