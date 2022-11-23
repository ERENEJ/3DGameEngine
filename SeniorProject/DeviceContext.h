#pragma once
#include  <d3d11.h>


class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;


class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);

	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setIndexBuffer(IndexBuffer* index_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);

	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);

	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	void setViewportSize(UINT width, UINT height);

	void SetVertexShader(VertexShader* vertex_shader);
	void SetPixelShader(PixelShader* pixel_shader);

	void SetConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void SetConstantBuffer(PixelShader* Pixel_shader, ConstantBuffer* buffer);

	bool release();

private:
	ID3D11DeviceContext* m_device_context;
	
	
	
	friend class ConstantBuffer;
};




