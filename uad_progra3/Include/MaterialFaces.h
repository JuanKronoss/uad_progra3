#pragma once

#include <string>
#include <vector>

using namespace std;

class MaterialFaces
{
public:

	vector<unsigned short> getFacesVerticesIdx() { return m_facesVerticesIdx; }
	vector<unsigned short> getFacesTexturesIdx() { return m_facesTexturesIdx; }
	vector<unsigned short> getFacesNormalsIdx() { return m_facesNormalsIdx; }

	string m_materialFile;

	vector<unsigned short> m_facesVerticesIdx;
	vector<unsigned short> m_facesTexturesIdx;
	vector<unsigned short> m_facesNormalsIdx;
};