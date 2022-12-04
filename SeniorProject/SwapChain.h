#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"



class SwapChain
{
public:

	SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height);
	~SwapChain();

	bool present(bool vsync);


private:

	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;
	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};

