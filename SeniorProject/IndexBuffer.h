#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{

public:
	
	IndexBuffer(RenderSystem* system, void* list_indicies, UINT size_list);
	~IndexBuffer();


	UINT getSizeIndexList();


private:

	UINT m_size_list;
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

