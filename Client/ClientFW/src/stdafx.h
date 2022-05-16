#pragma once
#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "imm32.lib")


#include <wrl.h>
#include <shellapi.h>

#include <dwrite.h>
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>


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

#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <algorithm>
#include <numeric>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <array>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <filesystem>
#include <bitset>

#include "client/core/core.h"
#include "client/core/log.h"
#include "client/math/math.h"



