#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <wrl.h>
#include <shellapi.h>

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <dwrite.h>
#include <d2d1_3.h>
#include <d3d11on12.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")