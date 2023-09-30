#pragma once

#include "../MaterialFaces.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

class C3DObj
{
public:

	C3DObj();
	~C3DObj();

	bool readObjFile(const char* obj_file);
	
	void readVertices(const string line);
	void readNormals(const string line);
	void readTextureCoords(const string line);
	void readTGA(const string line);
	void readMtlLib(const string file);
	void readFaces(const string line, string material);

	bool isModelLoaded() { return m_modelLoaded; }

private:

	fstream m_objectFile;

	vector<string> m_parametersLoaded; //"v", "vn", "vt", "f"

	vector<float> m_vertices;
	vector<float> m_normals;
	vector<float> m_textureCoords;

	unordered_map<string, string> m_materialFiles;

	MaterialFaces* m_currentMaterial;

	unordered_map<string, MaterialFaces*> m_facesPerMaterial;
	
	bool m_modelLoaded = true; //True until is no longer possible
};