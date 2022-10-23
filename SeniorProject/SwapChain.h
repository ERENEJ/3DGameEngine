#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	bool init(HWND hwnd, UINT width, UINT height);

	bool release();
	bool present(bool vsync);


private:

	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;

	friend class DeviceContext;
};

