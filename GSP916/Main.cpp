#include <Windows.h>

#include "System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//HINSTANCE hInstance: Instanz-Handle auf die eigene Anwendung
	//HINSTANCE hPrevInstance: Instanz-Handle auf die Anwendung, welche diese gestartet hat
	//LPSTR lpCmdLine: Commandline argumente (fr�her char** argv, int argc)
	//int nCmdShow: Minimized/Maximized/Normal

	System* _System = new System();
	_System->Init();

	_System->Run();
}