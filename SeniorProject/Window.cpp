#include "Window.h"
#include <exception>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
				break;
		}
		case WM_SIZE:
		{	
			//events will fired when window is resized
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window)window->onSize();
			break;
		}
		case WM_SETFOCUS:
		{
			//events will be fired when window get focus
		
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if(window)window->onFocus();
			
			break;
		}

		case WM_KILLFOCUS:
		{
			//events will be fired when window lost focus
				// TODO this part of the code taken from the comment section  from the video
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			window->onKillFocus();
			break;
		}

		case WM_DESTROY:
		{
			//events will be fired when window is destroyed
				// TODO this part of the code taken from the comment section from the video
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


Window::Window()
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


	if (!::RegisterClassEx(&wc))
	{
		throw std::exception("Window not created RegisterClassEx(&wc) failed");
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectXApplication", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if (!m_hwnd)
	{
		throw std::exception("Window not created m_hwnd failed");

	}


	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	// this is flag that indicates the window is properly initialized and running
	m_is_run = true;

}


Window::~Window()
{
}


bool Window::broadcast()
{
	MSG msg;

	if (!this->m_is_init)
	{

		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
		
	Sleep(0);

	return true;
}


bool Window::isRun()
{	
	if(m_is_run)broadcast();

	return m_is_run;
}

RECT Window::getClientWindowRect()
{

	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

RECT Window::getSizeScreen()
{
	RECT rc;

	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);

	return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}

void Window::onSize()
{
}
