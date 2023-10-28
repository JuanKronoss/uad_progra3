#pragma once
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "COpenGLRenderer.h"
#include "CVector3.h"
#include "Hex.h"
#include "C3DObj.h"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>

using std::unordered_map;
using std::vector;
using std::ifstream;
using std::istringstream;

using json = nlohmann::json;
using Point = CVector3;

class World
{
public:

	World() = default;
	~World();
	
	vector<Hex> m_hexes;

	struct Face
	{
		vector<unsigned short> faceVerticesIdx;
		vector<unsigned short> faceTexturesIdx;
		vector<unsigned short> faceNormalsIdx;
	};

	struct
	{
		unsigned int numRows;
		unsigned int numCols;
		string orientation;
		float cellSize;
		float horizontalSpacing;
		float verticalSpacing;

		struct 
		{
			unsigned int shaderID;
			unsigned int geometryID;
			unsigned int numFaces;

			vector<float> vertices;
			vector<float> normals;
			vector<float> UVs;

			Face faces;

			//vector<unsigned short> faceVerticesIdx;
			//vector<unsigned short> faceTexturesIdx;
			//vector<unsigned short> faceNormalsIdx;

			unordered_map<string, MaterialFaces*> facesPerMaterial;
		}BaseHex;

	}GridProperties;

	//Models
	//ModelInstances

	void loadWorld(string jsonGridFile);
	void loadHexGrid();
	void loadBaseHex();
	void oddR();
	void oddQ();
	void BaseHexVrtx(CVector3 cellCenter, float cellSize, int numVertex);
	void BaseHexNormals();
	void BaseHexUVs();
	void BaseHexFaces();
	//void loadModels();
	//void loadModelInstances();

	void allocateGraphicMemory();
	void allocateHex();
	void render();

	COpenGLRenderer* m_OpenGLRenderer; // Pointer to our OpenGL renderer object
};