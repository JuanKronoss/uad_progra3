#pragma once
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "COpenGLRenderer.h"
#include "CVector3.h"
#include "C3DObj.h"
#include "Hex.h"
#include "ModelInstance.h"

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
	vector<ModelInstance> m_modelInstances;

	struct MODEL
	{
		unsigned int shaderID;
		unordered_map<string, unsigned int> textureIDMap;
		unordered_map<string, unsigned int> geometryIDMap;
		unordered_map<string, unsigned int> numFacesInMtl;

	}Model;

	unordered_map<string, string> m_modelsPath;		//Model name and .obj path
	unordered_map<string, MODEL> m_modelsLoaded;	//Model name and its data

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
			unordered_map<string, MaterialFaces*> facesPerMaterial;

		}BaseHex;

	}GridProperties;

	//Models
	//ModelInstances

	void loadWorld(string jsonGridFile, string mediaDirectory);
	void loadGridProperties(json& data);
	void loadHexGrid();
	void loadBaseHex();
	void oddR();
	void oddQ();
	void BaseHexVrtx(CVector3 cellCenter, float cellSize, int numVertex);
	void BaseHexNormals();
	void BaseHexUVs();
	void BaseHexFaces();
	void loadModels(json& data, string mediaDirectory);
	void loadModelInstances(json& data);

	void allocateGraphicMemory();
	void allocateHex();
	void allocateModels(string model_name, string obj_file);

	void render();
	void renderHexGrid();
	void renderModelInstances();

	COpenGLRenderer* m_OpenGLRenderer; // Pointer to our OpenGL renderer object
};