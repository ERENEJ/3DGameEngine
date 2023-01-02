#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

//gpu only excepts 16 byte chunks forcing to 16 byte dividable
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	float m_time = 0.0f;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateSkyBox();

}
void AppWindow::updateModel()
{
	constant cc;

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	//45 degrees
	m_light_rot_y += 0.507f * m_delta_time;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	//cc.m_world *= temp;
	/*
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));
	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;
	*/

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	
	InputSystem::get()->addListener(this);
	
	m_play_state = true;
	InputSystem::get()->showCursor(false);

	 m_earthcolor_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	 m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	 m_clouds_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clouds.jpg");
	 m_earth_night_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_night.jpg");

	 m_earth_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");

	 m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere_hq.obj");
	 m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -1));
	
	Vector3D position_list[] =
	{

		{Vector3D(-0.5f,-0.5f,-0.5f)},
		{Vector3D(-0.5f,0.5f,-0.5f)},
		{Vector3D(0.5f,0.5f,-0.5f)},
		{Vector3D(0.5f,-0.5f,-0.5f)},

		{ Vector3D(0.5f,-0.5f,0.5f)},
		{ Vector3D(0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f)}
	};

	Vector2D texcoord_list[] =
	{

		{ Vector2D(0.0f, 0.0f)},
		{ Vector2D(0.0f, 1.0f)},
		{ Vector2D(1.0f, 0.0f)},
		{ Vector2D(1.0f, 1.0f)}
	};


	vertex vertex_list[] =
	{
		{position_list[0], texcoord_list[1]},
		{position_list[1], texcoord_list[0]},
		{position_list[2], texcoord_list[2]},
		{position_list[3], texcoord_list[3]},

		{position_list[4], texcoord_list[1]},
		{position_list[5], texcoord_list[0]},
		{position_list[6], texcoord_list[2]},
		{position_list[7], texcoord_list[3]},

		{position_list[1], texcoord_list[1]},
		{position_list[6], texcoord_list[0]},
		{position_list[5], texcoord_list[2]},
		{position_list[2], texcoord_list[3]},

		{position_list[7], texcoord_list[1]},
		{position_list[0], texcoord_list[0]},
		{position_list[3], texcoord_list[2]},
		{position_list[4], texcoord_list[3]},

		{position_list[3], texcoord_list[1]},
		{position_list[2], texcoord_list[0]},
		{position_list[5], texcoord_list[2]},
		{position_list[4], texcoord_list[3]},

		{position_list[7], texcoord_list[1]},
		{position_list[6], texcoord_list[0]},
		{position_list[1], texcoord_list[2]},
		{position_list[0], texcoord_list[3]},
	};

	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);
	

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	
	//m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);


	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	//cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	m_sky_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));


	m_world_cam.setTranslation(Vector3D(0, 0, -2));
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::render()
{


	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//COMPUTE TRANSFORM MATRICES
	update();

	//Render model
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);

	//Sending multiple textures for earth model
	TexturePtr list_tex[4];
	list_tex[0] = m_earthcolor_tex;
	list_tex[1] = m_earth_spec_tex;
	list_tex[2] = m_clouds_tex;
	list_tex[3] = m_earth_night_tex;

	drawMesh(m_mesh, m_vs, m_ps, m_cb, list_tex, 4);

	//Render sphere Skybox worked for same pixel shader variable 
	//result was 2 different texture wirt same variable as intended (like in the example above)
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	
	list_tex[0] = m_sky_tex;
	
	drawMesh(m_sky_mesh, m_vs, m_sky_ps, m_sky_cb, list_tex, 1);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;

	m_time += m_delta_time;
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	this->render(); 

}
void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + (world_cam.getZDirection() * (m_forward * 0.1f));

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	// world to view translation
	world_cam.inverse();

	m_view_cam = world_cam;
	/*
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);
	*/
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps,
	const ConstantBufferPtr& cb, const TexturePtr* list_texture, unsigned int num_textures)
{

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->SetConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->SetConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->SetVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->SetPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, list_texture, num_textures);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//SET THE indicies OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);

}

void AppWindow::onKeyDown(int key)
{
	
	if (key == 'W')
	{
		//m_rot_x -= 3.707f * m_delta_time;
		//m_forward += 0.1f * m_delta_time;
		m_forward += 1.0f ;
	}
	else if (key == 'S')
	{
		//m_rot_x += 3.707f * m_delta_time;
		//m_forward += -0.1 * m_delta_time;
		m_forward -= 1.0f ;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.707f * m_delta_time;
		//m_rightward = -1.f * m_delta_time;
		m_rightward = -1.f ;
	}
	else if (key == 'D')
	{	
		m_rightward = +1.f;
		//m_rightward = +1.f * m_delta_time;;
		//m_rot_y -= 3.707f * m_delta_time;
	}
	
}
void AppWindow::onMouseMove(const Point& mouse_pos)
{	
	if (!m_play_state) return;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time*0.1f;


	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
	
	
}
void AppWindow::onKeyUp(int key)
{	
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}

	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;	
		RECT size_screen = this->getSizeScreen();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
		
	}
	
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}