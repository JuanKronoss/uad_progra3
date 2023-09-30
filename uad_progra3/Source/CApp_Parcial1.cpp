#include "../stdafx.h"
#include "windows.h"
#include <iostream>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CApp_Parcial1.h"

/* */
CApp_Parcial1::CApp_Parcial1() :
	CApp_Parcial1(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CApp_Parcial1::CApp_Parcial1(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CApp_Parcial1(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CApp_Parcial1::~CApp_Parcial1()
{
	cout << "Destructor: ~CApp_Parcial1()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
}

/* */
void CApp_Parcial1::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	// ==================================
}

/* */
void CApp_Parcial1::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CApp_Parcial1::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	// ===============================
}

/* */
void CApp_Parcial1::render()
{
	CGameMenu* menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		// =================================
	}
}

/* */
void CApp_Parcial1::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CApp_Parcial1::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}


void CApp_Parcial1::onF2(int mods)
{
	setMenuActive(true);

	std::wstring wideStringBuffer = L"";
	wideStringBuffer.resize(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L" Obj Files\0*.obj\0 Stl Files\0*.stl\0 3DS Files\0*.3ds\0 All files\0*.*\0";
	ofn.lpstrFile = &wideStringBuffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		int size_needed = WideCharToMultiByte(CP_ACP, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), NULL, 0, NULL, NULL);
		std::string multibyteString(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), &multibyteString[0], size_needed, NULL, NULL);
		cout << "Filename to load: " << multibyteString.c_str() << endl;


		if (!m_currentObject.readObjFile(multibyteString.c_str()))
		{
				cout << "\a\nUnable to load 3D model\n";
		}
		else
		{
			setMenuActive(false);
		}

		//if (!load3DModel(multibyteString.c_str()))
		//{
		//	cout << "Unable to load 3D model" << endl;
		//}
		//else
		//{
		//	setMenuActive(false);
		//}
	}
}