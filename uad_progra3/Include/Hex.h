#pragma once

#include "../Include/CVector3.h"
#include "../Include/MathHelper.h"

//#include <vector>
#include <string>

//using std::vector;
using std::string;

class Hex
{
public:
	Hex() = default;
	~Hex() = default;

	string m_orientation;

	CVector3 m_center = CVector3::ZeroVector();
	unsigned int m_row;
	unsigned int m_column;

	MathHelper::Matrix4 m_modelMatrix;
};