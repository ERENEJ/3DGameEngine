#include "Window.h"

#include <iostream>

//Window* window = nullptr;




LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
				//events will be fired when window is created
				// TODO this part of the code taken from the comment section as improvment bot from the video
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

			window->onCreate();
			break;
		}
		case WM_DESTROY:
		{
			//events will be fired when window is destroyed
				// TODO this part of the code taken from the comment section as improvment bot from the video
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}
		default:
		{
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}


bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
	

	if(!::RegisterClassEx(&wc))
	{
		std::cout << "returned from RegisterClassEx Check wc variable \n";
		return false;
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectXApplication", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);
																									//TODO changed from "NULL"
	if(!m_hwnd)
	{
		std::cout << "retuned from 2";
		return false;
	}

	
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	// this is flag that indicates the window is properly initialized and running
	m_is_run = true;
	return true;
}

bool Window::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	this->onUpdate();
	//window->onUpdate();
	Sleep(0);

	return true;
}

bool Window::release()
{
	// destroy window
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

bool Window::isRun()
{

	return m_is_run;
}

void Window::onDestroy()
{
	m_is_run = false;
}
