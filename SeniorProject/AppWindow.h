#pragma once

#include "Window.h"

class AppWindow: public Window
{

public:

	//inherited from Window class
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
};

