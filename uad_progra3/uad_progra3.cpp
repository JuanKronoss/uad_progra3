// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
using namespace std;

#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
#include "Include/CApp_Parcial1.h"
#include "Include/CApp_Parcial2.h"

int main(int argc, char** argv)
{
	CApp *app = NULL;					// Pointer to BASE class CApp

	int width = 1200;
	int height = 720;

	if (argc >= 5)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		string instantiateClass(argv[3]);
		string fullscreen(argv[4]);

		cout << "\nInstantiating " << instantiateClass << "(" << width << ", " << height << ")" << " by command line.\n\n";
		if (instantiateClass == "CApp_Parcial2")
		{
			app = new CApp_Parcial2(width, height);
		}
		else if (instantiateClass == "CApp_Parcial1")
		{
			app = new CApp_Parcial1(width, height);
		}

		if (fullscreen == "True")
		{
			app->m_fullscreen = true;
		}
	}
	else
	{
		//app = new CAppEmpty(800, 600);		// Using pointer to base class, create a new object of DERIVED class
		cout << "\nInstantiating CApp_Parcial2(" << width << ", " << height << ")" << " by default.\n\n";
		app = new CApp_Parcial2(width, height);
	}
	app->run();							// Run the app
	delete app;							// Delete pointer
	app = NULL;							// Set pointer to NULL

	return 0;
}

