#pragma once
#include <Windows.h>


class Window
{

public:

	bool init();
	bool broadcast();
	bool release();
	bool isRun();


	//EVENTS
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

	
protected:
	HWND m_hwnd;
	bool m_is_run;

};

