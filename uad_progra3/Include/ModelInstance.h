#pragma once

#include "../Include/CVector3.h"
#include "../Include/MathHelper.h"

#include <string>

using std::string;


class ModelInstance
{
public:

	ModelInstance() = default;
	~ModelInstance() = default;

	string m_modelName;

	CVector3 m_center = CVector3::ZeroVector();
	unsigned int m_row;
	unsigned int m_column;
	float m_scale;
	CVector3 m_rotation;

	MathHelper::Matrix4 m_modelMatrix;

	float m_color[3] = { 0.0f, 1.0f, 0.0f };
};