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
#include "Include/CApp_Parcial3.h"
#include "Include/CVector3.h"
#include "Include/Plane.h"
#include "Include/Frustum.h"

int main(int argc, char** argv)
{
	CApp *app = NULL;					// Pointer to BASE class CApp

	int argumentsNeeded = 5;

	string path(argv[0]);
	string projectName("uad_progra3");
	string resourcesDirectory("Resources\\MEDIA\\");
	string jsonPath("Resources\\MEDIA\\HEXGRID\\hexgrid_cfg.json");

	string currentDirectory;
	string checkProject;;
	string mediaDirectory;
	string jsonFilePath;

	istringstream completePath{ path };

	while (currentDirectory != projectName)
	{
		getline(completePath, currentDirectory, '\\');
		mediaDirectory += currentDirectory;
		mediaDirectory += "\\";

		jsonFilePath += currentDirectory;
		jsonFilePath += "\\";
	}
	//Check if the project name is the same as repository's
	getline(completePath, checkProject, '\\');

	if (currentDirectory == checkProject)
	{
		mediaDirectory += projectName + "\\";
		jsonFilePath += projectName + "\\";
	}
	
	mediaDirectory += resourcesDirectory;
	jsonFilePath += jsonPath;

	int width = 1200;
	int height = 720;

	if (argc >= argumentsNeeded)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		string instantiateClass(argv[3]);
		string fullscreen(argv[4]);

		cout << "\nInstantiating " << instantiateClass << "(" << width << ", " << height << ")" << " by command line.\n\n";
		if (instantiateClass == "CApp_Parcial3")
		{
			app = new CApp_Parcial3(width, height, jsonFilePath, mediaDirectory);
		}
		else if (instantiateClass == "CApp_Parcial2")
		{
			app = new CApp_Parcial2(width, height, jsonFilePath, mediaDirectory);
		}
		else if (instantiateClass == "CApp_Parcial1")
		{
			app = new CApp_Parcial1(width, height);
		}

		if (fullscreen == "True" && app != NULL)
		{
			app->m_fullscreen = true;
		}
	}
	else
	{
		//app = new CAppEmpty(800, 600);		// Using pointer to base class, create a new object of DERIVED class
		cout << "\nInstantiating CApp_Parcial3(" << width << ", " << height << ")" << " by default.\n\n";

		CVector3 v0{ 3.0f, 6.0f, 0.0f };
		CVector3 v1{ 3.0f, 3.0f, 0.0f };
		CVector3 v2{ 3.0f, 6.0f, -3.0f };

		CVector3 v3{ 1.0f, 5.0f, -5.0f };

		Plane plane(v0, v1, v2);

		cout << "Normal.X: " << plane.A << endl;
		cout << "Normal.Y: " << plane.B << endl;
		cout << "Normal.Z: " << plane.C << endl;
		cout << "Distance to origin: " << plane.D << endl;

		cout << endl;
		cout << "Distance to vertex 0: " << plane.distanceToPoint(v0) << endl;
		cout << "Distance to vertex 1: " << plane.distanceToPoint(v1) << endl;
		cout << "Distance to vertex 2: " << plane.distanceToPoint(v2) << endl;
		cout << "Distance to an external vertex: " << plane.distanceToPoint(v3) << endl;

		CVector3 eyePos{ 0.0f, 0.0f, 0.0f };
		CVector3 lookAtPos{ 0.0f, 0.0f, -5.0f };
		CVector3 upVector{ 0.0f, 1.0f, 0.0f };
		CVector3 rightVector{ 1.0f, 0.0f, 0.0f };

		ViewFrustum frustum;
		frustum.update(eyePos, lookAtPos, upVector, rightVector, 0.001f, 1000.0f, 75.0f, (1024.0f / 768.0f));

		cout << endl;
		cout << "Top Plane:\nX: " << frustum.m_planes[0].A << ". Y: " << frustum.m_planes[0].B << ". Z: " << frustum.m_planes[0].C << endl;
		cout << endl;
		cout << "Bottom Plane:\nX: " << frustum.m_planes[1].A << ". Y: " << frustum.m_planes[1].B << ". Z: " << frustum.m_planes[1].C << endl;
		cout << endl;
		cout << "Left Plane:\nX: " << frustum.m_planes[2].A << ". Y: " << frustum.m_planes[2].B << ". Z: " << frustum.m_planes[2].C << endl;
		cout << endl;
		cout << "Right Plane:\nX: " << frustum.m_planes[3].A << ". Y: " << frustum.m_planes[3].B << ". Z: " << frustum.m_planes[3].C << endl;
		cout << endl;
		cout << "Near Plane:\nX: " << frustum.m_planes[4].A << ". Y: " << frustum.m_planes[4].B << ". Z: " << frustum.m_planes[4].C << endl;
		cout << endl;
		cout << "Far Plane:\nX: " << frustum.m_planes[5].A << ". Y: " << frustum.m_planes[5].B << ". Z: " << frustum.m_planes[5].C << endl;
		cout << endl;

		CVector3 point{ 0.0f, 0.0f, -500.0f };
		CVector3 point2{ 0.0f, 0.0f, -10.0f };

		cout << endl;
		cout << "Is point visible: " << frustum.isPointVisible(point2);

		app = new CApp_Parcial3(width, height, jsonFilePath, mediaDirectory);
	}
	app->run();							// Run the app
	delete app;							// Delete pointer
	app = NULL;							// Set pointer to NULL

	return 0;
}

