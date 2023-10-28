#include "../stdafx.h"
#include "windows.h"
#include <iostream>
#include <unordered_map>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CApp_Parcial2.h"
#include "../Include/CTextureLoader.h"
#include "../Include/MaterialFaces.h"
#include "../Include/CVector3.h"
#include "../Include/MathHelper.h"

/* */
CApp_Parcial2::CApp_Parcial2() :
	CApp_Parcial2(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CApp_Parcial2::CApp_Parcial2(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CApp_Parcial2(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CApp_Parcial2::~CApp_Parcial2()
{
	cout << "Destructor: ~CApp_Parcial2()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================

	for (auto id : m_textureIDMap)
	{
		getOpenGLRenderer()->deleteTexture(&id.second);
	}

	for (auto id : m_geometryIDMap)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&id.second);
	}

}

/* */
void CApp_Parcial2::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	// ==================================

	m_myWorld.loadWorld(m_jsonGridFile);
	m_myWorld.m_OpenGLRenderer = getOpenGLRenderer();
	m_myWorld.allocateGraphicMemory();
}

/* */
void CApp_Parcial2::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE, m_fullscreen))
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
void CApp_Parcial2::update(double deltaTime)
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
	deltaTime *= 0.001;

	double degrees = m_rotationSpeed * deltaTime;
	
	m_currentRotation += degrees;

	if (m_currentRotation > 360.0)
	{
		m_currentRotation -= 360.0;
	}
}

/* */
void CApp_Parcial2::render()
{
	m_myWorld.render();


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
		double currentDegreesRad = m_currentRotation * PI_OVER_180;


		float color[3] = { 1.0f, 1.0f, 1.0f };

		CVector3 position = m_currentPosition;
		position.X -= 5.0f;

		// convert total degrees rotated to radians;
		//double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		MathHelper::Matrix4 scale = MathHelper::ScaleMatrix(2.0f, 2.0f, 2.0f);

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 rotationAndTranslation = MathHelper::SimpleModelMatrixRotationTranslation((float)currentDegreesRad, position);

		MathHelper::Matrix4 modelMatrix2 = MathHelper::Multiply(rotationAndTranslation, scale);

		MathHelper::Matrix4 translationMatrix = MathHelper::TranslationMatrix(m_currentPosition.X, m_currentPosition.Y, m_currentPosition.Z);
		MathHelper::Matrix4 rotationMatrix = MathHelper::RotAroundX(currentDegreesRad);

		MathHelper::Matrix4 modelMatrix = MathHelper::Multiply(rotationMatrix, translationMatrix);

		unsigned int modelShader = m_shaderID;

		for (auto id : m_geometryIDMap)
		{
			unsigned int modelVAO = id.second;
			unsigned int modelTexture = m_textureIDMap[id.first];

			getOpenGLRenderer()->renderObject(
				&modelShader,
				&modelVAO,
				&modelTexture,
				m_numFacesInMtl[id.first],
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);

			//getOpenGLRenderer()->renderObject(
			//	&modelShader,
			//	&modelVAO,
			//	&modelTexture,
			//	m_numFacesInMtl[id.first],
			//	color,
			//	&modelMatrix2,
			//	COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			//	false
			//);
		}



	}
}

/* */
void CApp_Parcial2::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CApp_Parcial2::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}


void CApp_Parcial2::onF2(int mods)
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


		if (!m_3dObject.readObjFile(multibyteString.c_str()))
		{
			cout << "\a\nUnable to load 3D model\n";
		}
		else
		{
			setMenuActive(false);

			if (m_3dObject.hasUVs() && m_3dObject.hasTextures())
			{
				unordered_map<string, string>* materialFiles = m_3dObject.getMaterialFiles();

				// Switch shaders to textured object ones
				m_shaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

				for (auto file : *materialFiles)
				{
					unsigned int newTextureID;

					// LOAD TEXTURE AND ALSO CREATE TEXTURE OBJECT
					if (CTextureLoader::loadTexture(file.second.c_str(), &newTextureID, getOpenGLRenderer()))
					{
						m_textureIDMap[file.first] = newTextureID;
					}
					else
					{
						// Texture could not be loaded, default back to color shader
						m_shaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
					}
				}

			}
			else
			{
				// Load color shader
				m_shaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
			}

			unordered_map<string, MaterialFaces*>* facesPerMaterial = m_3dObject.getFacesPerMaterial();

			for (auto faces : *facesPerMaterial)
			{
				vector<unsigned short> verticesIdx;
				vector<unsigned short> normalsIdx;
				vector<unsigned short> texturesIdx;

				MaterialFaces* currentFaces = faces.second;

				verticesIdx = currentFaces->getFacesVerticesIdx();
				normalsIdx = currentFaces->getFacesNormalsIdx();
				texturesIdx = currentFaces->getFacesTexturesIdx();

				bool loadedToGraphicsCard;
				unsigned int geometryID;

				// Allocate graphics memory for object
				loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
					&m_shaderID,
					&geometryID,
					m_3dObject.getVertices()->data(),
					m_3dObject.getVertices()->size() / 3,
					m_3dObject.getNormals()->data(),
					m_3dObject.getNormals()->size() / 3,
					m_3dObject.getTextureCoords()->data(),
					m_3dObject.getTextureCoords()->size() / 2,
					verticesIdx.data(),
					verticesIdx.size() / 3,
					normalsIdx.data(),
					normalsIdx.size() / 3,
					texturesIdx.data(),
					texturesIdx.size() / 3
				);

				if (!loadedToGraphicsCard)
				{
					cout << "\a\nUnable to allocate graphic memory!\n";
				}
				else
				{
					m_geometryIDMap[faces.first] = geometryID;
					m_numFacesInMtl[faces.first] = verticesIdx.size() / 3;
				}
			}
		}
	}
}