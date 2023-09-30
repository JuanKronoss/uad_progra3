#pragma once

#include <string>
#include <vector>

using namespace std;

class MaterialFaces
{
public:

	string m_materialFile;

	vector<unsigned int> m_facesVerticesIdx;
	vector<unsigned int> m_facesTexturesIdx;
	vector<unsigned int> m_facesNormalsIdx;

};