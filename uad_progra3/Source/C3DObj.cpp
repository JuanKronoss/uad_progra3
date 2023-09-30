#include "../stdafx.h"

#include "../Include/C3DObj.h"

C3DObj::C3DObj()
{
}

C3DObj::~C3DObj()
{
	for (auto material : m_facesPerMaterial)
	{
		if (nullptr != material.second)
		{
			delete material.second;
		}
	}
}

bool C3DObj::readObjFile(const char* obj_file)
{
	ifstream objFile;
	objFile.open(obj_file);

	string currentLine;
	string currentMaterial;

	while ((!objFile.eof()) && m_modelLoaded)
	{
		getline(objFile, currentLine);

		string token;
		string material;
		istringstream analyseLine{ currentLine };
		analyseLine >> token >> material;

		if (token == "v")
		{
			readVertices(currentLine);
		}
		else if (token == "vn")
		{
			readNormals(currentLine);
		}
		else if (token == "vt")
		{
			readTextureCoords(currentLine);
		}
		else if (token == "mtllib")
		{
			readMtlLib(currentLine);
		}
		else if (token == "usemtl")
		{
			currentMaterial = material;
		}
		else if (token == "f")
		{
			readFaces(currentLine, currentMaterial);
		}
	}
	
	int parametersNeeded{ 4 };
	
	if (parametersNeeded != m_parametersLoaded.size())
	{
		m_modelLoaded = false;
	}
	else
	{
		m_modelLoaded = true;
	}

	return m_modelLoaded;
}


void C3DObj::readVertices(const string line)
{
	string parameter;
	float x;
	float y;
	float z;

	istringstream vertex{ line };

	vertex >> parameter >> x >> y >> z;

	m_vertices.push_back(x);
	m_vertices.push_back(y);
	m_vertices.push_back(z);

	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), parameter) == m_parametersLoaded.end())
	{
		m_parametersLoaded.push_back(parameter);
	}
}


void C3DObj::readNormals(const string line)
{
	string parameter;
	float x;
	float y;
	float z;

	istringstream normal{ line };

	normal >> parameter >> x >> y >> z;

	m_normals.push_back(x);
	m_normals.push_back(y);
	m_normals.push_back(z);

	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), parameter) == m_parametersLoaded.end())
	{
		m_parametersLoaded.push_back(parameter);
	}
}


void C3DObj::readTextureCoords(const string line)
{
	string parameter;
	float u;
	float v;

	istringstream textureUV{ line };

	textureUV >> parameter >> u >> v;

	m_textureCoords.push_back(u);
	m_textureCoords.push_back(v);

	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), parameter) == m_parametersLoaded.end())
	{
		m_parametersLoaded.push_back(parameter);
	}
}


void C3DObj::readMtlLib(const string line)
{
	string parameter;
	string fileName;

	istringstream materialLine{ line };

	materialLine >> parameter >> fileName;

	readTGA(fileName);
}


void C3DObj::readTGA(const string file)
{
	ifstream tgaFile;
	tgaFile.open(file);

	string currentLine;

	string materialName;

	while (!tgaFile.eof())
	{
		getline(tgaFile, currentLine);

		string token;
		string data;
		istringstream analyseLine{ currentLine };
		analyseLine >> token >> data;


		if (token == "newmtl")
		{
			materialName = data;
			m_materialFiles[materialName];
		}
		else if (token == "map_Kd")
		{
			m_materialFiles[materialName] = data;
		}
	}
}


void C3DObj::readFaces(const string line, string material)
{
	string parameter;

	string firstVertex;
	string secondVertex;
	string thirdVertex;
	string fourthVertex;
	string nGon;

	istringstream faceLine{ line };
	//istringstream faceLine{ "f 5/1/1 1/3/1 2/4/1 6/2/1" };
	//istringstream faceLine{ "f 5/1/1 1/3/1 2/4/1 6/2/1 7/7/7" };

	faceLine >> parameter >> firstVertex >> secondVertex >> thirdVertex >> fourthVertex >> nGon;

	//Unable to read N-Gons
	if (nGon != "")
	{
		cout << "\a\nCannot load N-Gons!\n";
		m_modelLoaded = false;
		return;
	}

	if (m_facesPerMaterial.find(material) == m_facesPerMaterial.end())
	{
		m_facesPerMaterial[material] = new MaterialFaces();
		m_currentMaterial = m_facesPerMaterial[material];
		m_currentMaterial->m_materialFile = material;
	}
	m_currentMaterial = m_facesPerMaterial[material];
	

	//This is a Quad
	if (fourthVertex != "")
	{
		//Dividing the Quad into two triangles
		string face1FirstVertex = firstVertex;
		string face1SecondVertex = secondVertex;
		string face1ThirdVertex = fourthVertex;

		int face1FirstVertexIdx;
		int face1SecondVertexIdx;
		int face1ThirdVertexIdx;

		int face1FirstUvIdx;
		int face1SecondUvIdx;
		int face1ThirdUvIdx;

		int face1FirstNormalIdx;
		int face1SecondNormalIdx;
		int face1ThirdNormalIdx;

		face1FirstVertexIdx = face1FirstVertex[0] - '0';
		face1SecondVertexIdx = face1SecondVertex[0] - '0';
		face1ThirdVertexIdx = face1ThirdVertex[0] - '0';
		m_currentMaterial->m_facesVerticesIdx.push_back(face1FirstVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(face1SecondVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(face1ThirdVertexIdx);

		face1FirstUvIdx = face1FirstVertex[2] - '0';
		face1SecondUvIdx = face1SecondVertex[2] - '0';
		face1ThirdUvIdx = face1ThirdVertex[2] - '0';
		m_currentMaterial->m_facesTexturesIdx.push_back(face1FirstUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(face1SecondUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(face1ThirdUvIdx);

		face1FirstNormalIdx = face1FirstVertex[4] - '0';
		face1SecondNormalIdx = face1SecondVertex[4] - '0';
		face1ThirdNormalIdx = face1ThirdVertex[4] - '0';
		m_currentMaterial->m_facesNormalsIdx.push_back(face1FirstNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(face1SecondNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(face1ThirdNormalIdx);

		string face2FirstVertex = secondVertex;
		string face2SecondVertex = thirdVertex;
		string face2ThirdVertex = fourthVertex;

		int face2FirstVertexIdx;
		int face2SecondVertexIdx;
		int face2ThirdVertexIdx;

		int face2FirstUvIdx;
		int face2SecondUvIdx;
		int face2ThirdUvIdx;

		int face2FirstNormalIdx;
		int face2SecondNormalIdx;
		int face2ThirdNormalIdx;

		face2FirstVertexIdx = face2FirstVertex[0] - '0';
		face2SecondVertexIdx = face2SecondVertex[0] - '0';
		face2ThirdVertexIdx = face2ThirdVertex[0] - '0';
		m_currentMaterial->m_facesVerticesIdx.push_back(face2FirstVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(face2SecondVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(face2ThirdVertexIdx);

		face2FirstUvIdx = face2FirstVertex[2] - '0';
		face2SecondUvIdx = face2SecondVertex[2] - '0';
		face2ThirdUvIdx = face2ThirdVertex[2] - '0';
		m_currentMaterial->m_facesTexturesIdx.push_back(face2FirstUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(face2SecondUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(face2ThirdUvIdx);

		face2FirstNormalIdx = face2FirstVertex[4] - '0';
		face2SecondNormalIdx = face2SecondVertex[4] - '0';
		face2ThirdNormalIdx = face2ThirdVertex[4] - '0';
		m_currentMaterial->m_facesNormalsIdx.push_back(face2FirstNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(face2SecondNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(face2ThirdNormalIdx);
	}
	//Triangles
	else
	{
		int firstVertexIdx;
		int secondVertexIdx;
		int thirdVertexIdx;

		int firstUvIdx;
		int secondUvIdx;
		int thirdUvIdx;

		int firstNormalIdx;
		int secondNormalIdx;
		int thirdNormalIdx;

		firstVertexIdx = firstVertex[0] - '0';
		secondVertexIdx = secondVertex[0] - '0';
		thirdVertexIdx = thirdVertex[0] - '0';
		m_currentMaterial->m_facesVerticesIdx.push_back(firstVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(secondVertexIdx);
		m_currentMaterial->m_facesVerticesIdx.push_back(thirdVertexIdx);

		firstUvIdx = firstVertex[2] - '0';
		secondUvIdx = secondVertex[2] - '0';
		thirdUvIdx = thirdVertex[2] - '0';
		m_currentMaterial->m_facesTexturesIdx.push_back(firstUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(secondUvIdx);
		m_currentMaterial->m_facesTexturesIdx.push_back(thirdUvIdx);

		firstNormalIdx = firstVertex[4] - '0';
		secondNormalIdx = secondVertex[4] - '0';
		thirdNormalIdx = thirdVertex[4] - '0';
		m_currentMaterial->m_facesNormalsIdx.push_back(firstNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(secondNormalIdx);
		m_currentMaterial->m_facesNormalsIdx.push_back(thirdNormalIdx);
	}


	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), parameter) == m_parametersLoaded.end())
	{
		m_parametersLoaded.push_back(parameter);
	}
}