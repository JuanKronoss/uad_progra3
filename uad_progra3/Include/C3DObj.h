#pragma once

#include "MaterialFaces.h"
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
	void readMtlLib(const string file, string directory);
	void readFaces(const string line, string material);
	void clear();

	bool isModelLoaded() { return m_modelLoaded; }

	vector<float>* getVertices() { return &m_vertices; }
	vector<float>* getNormals() { return &m_normals; }
	vector<float>* getTextureCoords() { return &m_textureCoords; }

	unordered_map<string, string>* getMaterialFiles() { return &m_materialFiles; }
	unordered_map<string, MaterialFaces*>* getFacesPerMaterial() { return &m_facesPerMaterial; }

	bool hasUVs() { return m_hasUVs; }
	bool hasTextures() { return m_hasTextures; }

private:

	fstream m_objectFile;

	vector<string> m_parametersLoaded; //"v", "vn", "vt", "f"

	vector<float> m_vertices;
	vector<float> m_normals;	//0.0	0.0		0.0
	vector<float> m_textureCoords; //0.0	0.0

	unordered_map<string, string> m_materialFiles;

	MaterialFaces* m_currentMaterial;

	unordered_map<string, MaterialFaces*> m_facesPerMaterial;

	bool m_hasUVs = true;
	bool m_hasTextures = true;

	bool m_modelLoaded = true; //True until is no longer possible
};