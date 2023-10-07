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

	string vertex = "v";
	string normal = "vn";
	string texture = "vt";
	string materialLibrary = "mtllib";
	string materialUse = "usemtl";
	string face = "f";

	while ((!objFile.eof()) && m_modelLoaded)
	{
		getline(objFile, currentLine);

		string token;
		string material;
		istringstream analyseLine{ currentLine };
		analyseLine >> token >> material;

		if (token == vertex)
		{
			readVertices(currentLine);
		}
		else if (token == normal)
		{
			readNormals(currentLine);
		}
		else if (token == texture)
		{
			readTextureCoords(currentLine);
		}
		else if (token == materialLibrary)
		{
			readMtlLib(currentLine);
		}
		else if (token == materialUse)
		{
			currentMaterial = material;
		}
		else if (token == face)
		{
			readFaces(currentLine, currentMaterial);
		}
	}

	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), vertex) == m_parametersLoaded.end())
	{
		m_modelLoaded = false;
	}
	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), normal) == m_parametersLoaded.end())
	{
		m_normals.push_back(0.0);
		m_normals.push_back(0.0);
		m_normals.push_back(0.0);
	}
	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), texture) == m_parametersLoaded.end())
	{
		m_textureCoords.push_back(0.0);
		m_textureCoords.push_back(0.0);

		m_hasUVs = false;
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
	//istringstream faceLine{ "f 501//1 1/3/ 2/4/1 6/2/1 7/7/7 8/8/8" };

	vector<string> faceVertices;
	
	faceLine >> parameter;
	string vertex;

	while (!faceLine.eof())
	{
		faceLine >> vertex;

		if (!vertex.empty())
		{
			faceVertices.push_back(vertex);
		}
		vertex = "";
	}

	if (m_facesPerMaterial.find(material) == m_facesPerMaterial.end())
	{
		m_facesPerMaterial[material] = new MaterialFaces();
		m_currentMaterial = m_facesPerMaterial[material];
		m_currentMaterial->m_materialFile = material;
	}
	m_currentMaterial = m_facesPerMaterial[material];

	int faceFirstVertex = 0;
	int faceSecondVertex = 1;
	int faceThirdVertex = faceVertices.size() - 1;

	int vertexIdx;
	int uvIdx;
	int normalIdx;

	string idxStr;
	string vertexIdxStr;
	string uvIdxStr;
	string normalIdxStr;

	char divisionChar = '/';

	unsigned short elementIdx = 0;

	enum ELEMENTS
	{
		VERTEX = 0,
		UV,
		NORMAL
	};

	for (faceFirstVertex; faceSecondVertex < faceThirdVertex; ++faceSecondVertex)
	{
		istringstream faceFirstIdx{ faceVertices[faceFirstVertex] };

		while (!faceFirstIdx.eof())
		{
			getline(faceFirstIdx, idxStr, divisionChar);

			switch (elementIdx)
			{
			case VERTEX:

				if (idxStr.empty())
				{
					//cout << "\nNo Vertex\n";
					m_modelLoaded = false;
					m_currentMaterial->m_facesVerticesIdx.push_back(0);
				}
				else
				{
					vertexIdx = stoi(idxStr);
					m_currentMaterial->m_facesVerticesIdx.push_back(vertexIdx - 1);
				}

				++elementIdx;

				break;

			case UV:

				if (idxStr.empty())
				{
					//           cout << "\nNo UV\n"; 
					m_hasUVs = false;
					m_currentMaterial->m_facesTexturesIdx.push_back(0);
				}
				else
				{
					uvIdx = stoi(idxStr);
					m_currentMaterial->m_facesTexturesIdx.push_back(uvIdx - 1);
				}				

				++elementIdx;
				
				break;

			case NORMAL:

				if (idxStr.empty())
				{
					//cout << "\nNo normals\n";
					m_currentMaterial->m_facesNormalsIdx.push_back(0);
				}
				else
				{
					normalIdx = stoi(idxStr);
					m_currentMaterial->m_facesNormalsIdx.push_back(normalIdx - 1);
				}

				++elementIdx;

				break;

			default:

				m_modelLoaded = false;
			}
		}
		elementIdx = VERTEX;


		istringstream faceSecondIdx{ faceVertices[faceSecondVertex] };

		while (!faceSecondIdx.eof())
		{
			getline(faceSecondIdx, idxStr, divisionChar);

			switch (elementIdx)
			{
			case VERTEX:

				if (idxStr.empty())
				{
					//cout << "\nNo Vertex\n";
					m_modelLoaded = false;
					m_currentMaterial->m_facesVerticesIdx.push_back(0);
				}
				else
				{
					vertexIdx = stoi(idxStr);
					m_currentMaterial->m_facesVerticesIdx.push_back(vertexIdx - 1);
				}

				++elementIdx;

				break;

			case UV:

				if (idxStr.empty())
				{
					//cout << "\nNo UV\n";
					m_hasUVs = false;
					m_currentMaterial->m_facesTexturesIdx.push_back(0);
				}
				else
				{
					uvIdx = stoi(idxStr);
					m_currentMaterial->m_facesTexturesIdx.push_back(uvIdx - 1);
				}

				++elementIdx;

				break;

			case NORMAL:

				if (idxStr.empty())
				{
					//cout << "\nNo normals\n";
					m_currentMaterial->m_facesNormalsIdx.push_back(0);
				}
				else
				{
					normalIdx = stoi(idxStr);
					m_currentMaterial->m_facesNormalsIdx.push_back(normalIdx - 1);
				}

				++elementIdx;

				break;

			default:

				m_modelLoaded = false;
			}
		}
		elementIdx = VERTEX;


		istringstream faceThirdIdx{ faceVertices[faceThirdVertex] };

		while (!faceThirdIdx.eof())
		{
			getline(faceThirdIdx, idxStr, divisionChar);

			switch (elementIdx)
			{
			case VERTEX:

				if (idxStr.empty())
				{
					//cout << "\nNo Vertex\n";
					m_modelLoaded = false;
					m_currentMaterial->m_facesVerticesIdx.push_back(0);
				}
				else
				{
					vertexIdx = stoi(idxStr);
					m_currentMaterial->m_facesVerticesIdx.push_back(vertexIdx - 1);
				}

				++elementIdx;

				break;

			case UV:

				if (idxStr.empty())
				{
					//cout << "\nNo UV\n";
					m_hasUVs = false;
					m_currentMaterial->m_facesTexturesIdx.push_back(0);
				}
				else
				{
					uvIdx = stoi(idxStr);
					m_currentMaterial->m_facesTexturesIdx.push_back(uvIdx - 1);
				}

				++elementIdx;

				break;

			case NORMAL:

				if (idxStr.empty())
				{
					//cout << "\nNo normals\n";
					m_currentMaterial->m_facesNormalsIdx.push_back(0);
				}
				else
				{
					normalIdx = stoi(idxStr);
					m_currentMaterial->m_facesNormalsIdx.push_back(normalIdx - 1);
				}

				++elementIdx;

				break;

			default:

				m_modelLoaded = false;
			}
		}
		elementIdx = VERTEX;

		faceFirstVertex = faceSecondVertex;
		//cout << faceVertices[i][i] << "\n";
	}

	/*cout << "\nVertices: \n";
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		cout << m_vertices[i] << "\n";
	}

	cout << "\nUVs: \n";
	for (int i = 0; i < m_textureCoords.size(); ++i)
	{
		cout << m_textureCoords[i] << "\n";
	}

	cout << "\nNormals: \n";
	for (int i = 0; i < m_normals.size(); ++i)
	{
		cout << m_normals[i] << "\n";
	}

	cout << "\n";*/


	if (find(m_parametersLoaded.begin(), m_parametersLoaded.end(), parameter) == m_parametersLoaded.end())
	{
		m_parametersLoaded.push_back(parameter);
	}
}