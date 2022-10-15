#include "Window.h"


bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASS);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = LPCWSTR("MyWindowClass");
	wc.lpszMenuName = LPCWSTR("");
	wc.style = NULL;


	if(!::RegisterClassEx(&wc))
	{
		return false;
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, LPCWSTR("MyWindowClass"), LPCWSTR("DirectXApplication"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if(!m_hwnd)
	{
		return false;
	}

	//TODO second parameter is different than the video 11.30
	::ShowWindow(m_hwnd, 1);
	::UpdateWindow(m_hwnd);


	return true;
}

bool Window::release()
{

	if(m_hwnd)
	{
		::DestroyWindow(m_hwnd);
	}
	else
	{
		return false;
	}

	return true;
}
