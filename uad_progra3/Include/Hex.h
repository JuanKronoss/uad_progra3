#pragma once

#include "CVector3.h"
#include "MathHelper.h"
#include "ModelInstance.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

using Point = CVector3;

class Hex
{
public:
	Hex() = default;
	~Hex() = default;

	string m_orientation;
	CVector3 m_center = CVector3::ZeroVector();
	vector<Point> m_vertices;

	unsigned int m_column = 0;
	unsigned int m_row = 0;
	float m_size = 0.0f;

	unsigned int m_numFaces = 0;

	MathHelper::Matrix4 m_modelMatrix;

	ModelInstance* m_modelInstance = nullptr;
	unsigned int m_numTotalFaces = 0;
};