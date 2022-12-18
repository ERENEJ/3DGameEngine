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

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	hresult = device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if (FAILED(hresult))
	{
		throw std::exception(" SwapChain CreateTexture2D not created successfully");
	}

	hresult = device->CreateDepthStencilView(buffer, NULL, &m_depth_stencil_view);
	if (FAILED(hresult))
	{
		throw std::exception(" SwapChain CreateDepthStencilView not created successfully");
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
