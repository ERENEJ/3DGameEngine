#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"

class ConstantBuffer
{
public:
	
	ConstantBuffer(RenderSystem* system, void* buffer, UINT size_buffer);
	~ConstantBuffer();

	void update(DeviceContextPtr context, void* buffer);



private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};

