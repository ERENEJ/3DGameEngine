#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>


class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class GraphicsEngine
{
public:
	//
	bool init();
	//
	bool release();

	static GraphicsEngine* get();
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	ConstantBuffer* createConstantBuffer();
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code);


	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void releaseCompiledShader();


private:


	DeviceContext* m_imm_device_context;
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	

	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;

	ID3D11DeviceContext* m_imm_context;

	ID3DBlob* m_blob = nullptr;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;


	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
};

