#pragma once
#include <d3d11.h>
#include "Prerequisistes.h"
#include "RenderSystem.h"




class GraphicsEngine
{
public:

	GraphicsEngine();
	~GraphicsEngine();
	bool init();
	bool release();

	RenderSystem* getRenderSystem();

	static GraphicsEngine* get();

private:
	RenderSystem* m_render_system = nullptr;
};

