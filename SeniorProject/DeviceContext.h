#pragma once
#include  <d3d11.h>
#include "Prerequisistes.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	~DeviceContext();

	void clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);

	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);

	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	void setViewportSize(UINT width, UINT height);

	void SetVertexShader(VertexShaderPtr vertex_shader);
	void SetPixelShader(PixelShaderPtr pixel_shader);

	void SetConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer);
	void SetConstantBuffer(PixelShaderPtr Pixel_shader, ConstantBufferPtr buffer);


private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_system = nullptr;
	
	
	friend class ConstantBuffer;
};




