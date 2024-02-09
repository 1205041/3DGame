#pragma once
//===============================================
// プリコンパイル済みヘッダー
//  ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる。
//  全てのcppからインクルードされる必要がある。
//===============================================

//===============================================
// 基本
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
// STL
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <fileSystem>

//===============================================
// Direct3D11
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "Lib/Win32/Debug/DirectXTK.lib")
#pragma comment(lib, "Lib/Win32/Debug/Audio/DirectXTKAudioWin8.lib")
#include <DirectXTK/Inc/SimpleMath.h>
#include <DirectXTK/Inc/Audio.h>

// DirectX Tex
#pragma comment(lib, "Lib/Win32/Debug/DirectXTex.lib")
#include <DirectXTex/DirectXTex/DirectXTex.h>

//===============================================
// imGui
//===============================================
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imGui/imgui.h"
#include "imGui/imgui_internal.h"
#include "imGui/backends/imgui_impl_win32.h"
#include "imGui/backends/imgui_impl_dx11.h"
#include "imGui/misc/cpp/imgui_stdlib.h"

//===============================================
// 文字コード変換
//===============================================
#include <strconv.h>

//===============================================
// 自作Framework
//===============================================
#include "Framework/KdFramework.h"