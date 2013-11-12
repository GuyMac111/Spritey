// Spritey.cpp : main project file.

#include "stdafx.h"
#include "SpriteyForm.h"
#include <gl/glut.h>

using namespace Spritey;

//static extern bool AllocConsole();

void glutInitFunc()
{
	//we need to create some fake values to pass to GLUT
	char* myargv[1];
	int myargc = 1;
	myargv[0] = strdup("Spritey");
	glutInit(&myargc, myargv);
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	
	AllocConsole();
	AttachConsole( GetCurrentProcessId());
	freopen( "CON", "w", stdout );
	printf("Debugging Console Started");

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	glutInitFunc();

	// Create the main window and run it
	Application::Run(gcnew SpriteyForm());
	
	
	return 0;
}
