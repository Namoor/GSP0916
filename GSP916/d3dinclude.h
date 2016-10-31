#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#ifdef _DEBUG
#pragma comment (lib, "DirectXTK.lib")
#else
#pragma comment (lib, "DirectXTK_release.lib")
#endif


#include <WICTextureLoader.h>
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX;
