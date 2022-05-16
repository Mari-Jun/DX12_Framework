#pragma once

namespace client_fw
{
	struct Window
	{
		UINT width, height;
		HWND hWnd;
		HINSTANCE hInstance;
		RECT rect;
		POINT mid_pos;

		Window(UINT width = GetSystemMetrics(SM_CXSCREEN), UINT height = GetSystemMetrics(SM_CYSCREEN))
			: width(width), height(height)
			, hWnd(nullptr), hInstance(nullptr)
			, rect() , mid_pos()
		{}
	};
}

