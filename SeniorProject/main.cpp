#include <iostream>

#include "AppWindow.h"


int main()
{
	AppWindow app;

	if(app.init())
	{
		while(app.isRun())
		{

			app.broadcast();
		}
	}
	else
	{
		std::cout << "init failed for AppWindow \n";
	}
	return 0;
}
