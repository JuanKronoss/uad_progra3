#pragma once

#ifndef CAPPPARCIAL3_H
#define CAPPPARCIAL3_H

#include "CVector3.h"
#include "Globals.h"
#include "CApp.h"
#include "C3DObj.h"
#include "World.h"

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CApp_Parcial3 : public CApp
{
private:

	// ---------------------------------------------------
	// Private member variables specific to CApp_Parcial1 only
	// ---------------------------------------------------
	//
	// ---------------------------------------------------
	World m_myWorld;
	string m_jsonGridFile;
	string m_mediaDirectory;

	C3DObj m_3dObject;

	unsigned int m_shaderID;
	unordered_map<string, unsigned int> m_textureIDMap;
	unordered_map<string, unsigned int> m_geometryIDMap;
	unordered_map<string, unsigned int> m_numFacesInMtl;

	double m_rotationSpeed = 90.0f; //rotation per second
	double m_currentRotation = 0.0f;
	CVector3 m_currentPosition{ 0.0f, -20.0f, -15.0f };

protected:

	// Method to initialize the menu 
	// (not needed in CApp_Parcial1, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	CApp_Parcial3();
	CApp_Parcial3(int window_width, int window_height, string jsonPath, string media_directory);
	~CApp_Parcial3();

	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// ------------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// -----------------------------------------
	// Public methods specific to CAppEmpty only
	// -----------------------------------------
	//
	// -----------------------------------------
	void onF2(int mods);                       // F2
	void onArrowUp(int mods) override;
	void onArrowDown(int mods) override;
	void onArrowLeft(int mods) override;
	void onArrowRight(int mods) override;


private:

	// ------------------------------------------
	// Private methods specific to CAppEmpty only
	// ------------------------------------------
	//
	// ------------------------------------------

};

#endif // !CApp_Parcial3