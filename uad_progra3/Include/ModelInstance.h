#pragma once

#include "CVector3.h"
#include "MathHelper.h"

#include <string>

using std::string;


class ModelInstance
{
public:

	ModelInstance() = default;
	~ModelInstance() = default;

	string m_modelName;

	unsigned int m_numFaces = 0;

	CVector3 m_center = CVector3::ZeroVector();
	unsigned int m_column = 0;
	unsigned int m_row = 0;
	float m_scale = 0.0f;
	CVector3 m_rotation;

	MathHelper::Matrix4 m_modelMatrix;

	float m_color[3] = { 84.0f / 256.0f, 234.0f / 256.0f, 40.0f / 256.0f };
};