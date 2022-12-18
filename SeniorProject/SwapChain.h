#pragma once
#include <d3d11.h>
#include "Prerequisites.h"



class SwapChain
{
public:

	SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height);
	~SwapChain();

	bool present(bool vsync);


private:

	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};

