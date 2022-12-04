#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height):m_system(system)
{

	ID3D11Device* device = m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;


	HRESULT hresult = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
	if (FAILED(hresult))
	{
		throw std::exception(" SwapChain m_system->m_dxgi_factory->CreateSwapChain not created successfully");
	}

	ID3D11Texture2D* buffer = NULL;
	hresult = m_swap_chain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hresult))
	{
		throw std::exception(" SwapChain m_swap_chain->GetBuffer not created successfully");
	}
	hresult = device->CreateRenderTargetView(buffer, NULL, &m_render_target_view);
	buffer->Release();

	if (FAILED(hresult))
	{
		throw std::exception(" SwapChain CreateRenderTargetView not created successfully");
	}

}

SwapChain::~SwapChain()
{	
	m_render_target_view->Release();
	m_swap_chain->Release();
}



bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);

	return true;
}
