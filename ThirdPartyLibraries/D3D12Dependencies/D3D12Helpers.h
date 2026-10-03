/**
* @file D3D12Helpers.h
* @author Andrew Fagan
* @date 3/24/2026
*/


#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif
#include <windows.h>

#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;
