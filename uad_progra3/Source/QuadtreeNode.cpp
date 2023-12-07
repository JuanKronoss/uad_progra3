#include "../stdafx.h"
#include "../Include/QuadtreeNode.h"
#include "../Include/CVector3.h"

#include <iostream>

using std::cout;
using std::endl;

QuadtreeNode::QuadtreeNode(AABB_2D nodeLimits, unsigned int maxFaces, vector<Hex>* hexes) :
	m_nodeLimits(nodeLimits), m_maxFaces(maxFaces), m_hexes(hexes)
{
	unsigned int facesInNode = 0;
	bool partition = false;

	CVector3 topLeft = m_nodeLimits.m_points[0];
	CVector3 topRight = m_nodeLimits.m_points[1];
	CVector3 bottomLeft = m_nodeLimits.m_points[2];
	CVector3 bottomRight = m_nodeLimits.m_points[3];

	float leftmostX = m_nodeLimits.m_points[0].X;
	float rightmostX = m_nodeLimits.m_points[3].X;
	float uppermostZ = m_nodeLimits.m_points[0].Z;
	float lowermostZ = m_nodeLimits.m_points[3].Z;

	CVector3 center = { leftmostX + ((rightmostX - leftmostX) * 0.5f),
						0.0f,
						lowermostZ + ((uppermostZ - lowermostZ) * 0.5f) };

	CVector3 leftCenter = { leftmostX, 0.0f, center.Z};
	CVector3 rightCenter = { rightmostX, 0.0f, center.Z };
	CVector3 upperCenter = { center.X, 0.0f, uppermostZ };
	CVector3 lowerCenter = { center.X, 0.0f, lowermostZ };


	for (auto& cell : *m_hexes)
	{		
		bool cellInNode =
			cell.m_center.X >= leftmostX &&
			cell.m_center.X <= rightmostX &&
			cell.m_center.Z >= uppermostZ &&
			cell.m_center.Z <= lowermostZ;

		if (cellInNode)
		{
			facesInNode += cell.m_numTotalFaces;

			m_nodeCells.push_back(&cell);
		}
		if (facesInNode > m_maxFaces)
		{
			if (m_nodeCells.size() != 1)
			{
				partition = true;
			}
			break;
		}
	}
	if (partition)
	{
		m_nodeCells.clear();

		CVector3 topLeftBoundingBox[4];
		topLeftBoundingBox[0] = topLeft;
		topLeftBoundingBox[1] = upperCenter;
		topLeftBoundingBox[2] = leftCenter;
		topLeftBoundingBox[3] = center;

		CVector3 topRightBoundingBox[4];
		topRightBoundingBox[0] = upperCenter;
		topRightBoundingBox[1] = topRight;
		topRightBoundingBox[2] = center;
		topRightBoundingBox[3] = rightCenter;

		CVector3 bottomLeftBoundingBox[4];
		bottomLeftBoundingBox[0] = leftCenter;
		bottomLeftBoundingBox[1] = center;
		bottomLeftBoundingBox[2] = bottomLeft;
		bottomLeftBoundingBox[3] = lowerCenter;

		CVector3 bottomRightBoundingBox[4];
		bottomRightBoundingBox[0] = center;
		bottomRightBoundingBox[1] = rightCenter;
		bottomRightBoundingBox[2] = lowerCenter;
		bottomRightBoundingBox[3] = bottomRight;

		AABB_2D topLeftLimits;
		topLeftLimits.setPoints(topLeftBoundingBox);

		AABB_2D topRightLimits;
		topRightLimits.setPoints(topRightBoundingBox);

		AABB_2D bottomLeftLimits;
		bottomLeftLimits.setPoints(bottomLeftBoundingBox);

		AABB_2D bottomRightLimits;
		bottomRightLimits.setPoints(bottomRightBoundingBox);

		m_topLeft = new QuadtreeNode(topLeftLimits, m_maxFaces, m_hexes);
		m_topRight = new QuadtreeNode(topRightLimits, m_maxFaces, m_hexes);
		m_bottomLeft = new QuadtreeNode(bottomLeftLimits, m_maxFaces, m_hexes);
		m_bottomRight = new QuadtreeNode(bottomRightLimits, m_maxFaces, m_hexes);
	}
	if (m_nodeCells.size() > 0)
	{
		cout << endl << "---------------Node------------------" << endl;
		for (auto& hex : m_nodeCells)
		{
			cout << "(" << hex->m_column << ", " << hex->m_row << ")" << endl;

			if (hex->m_column == 3 && hex->m_row == 0)
			{
				bool a = false;
			}
		}
	}
}


QuadtreeNode::~QuadtreeNode()
{
	if (m_topLeft != nullptr)
	{
		delete m_topLeft;
		m_topLeft = nullptr;
	}
	if (m_topRight != nullptr)
	{
		delete m_topRight;
		m_topRight = nullptr;
	}
	if (m_bottomLeft != nullptr)
	{
		delete m_bottomLeft;
		m_bottomLeft = nullptr;
	}
	if (m_bottomRight != nullptr)
	{
		delete m_bottomRight;
		m_bottomRight = nullptr;
	}
}

void QuadtreeNode::render(Camera* cam, vector<Hex*>& visibleCells)
{
	if (cam->isAABBVisible(m_nodeLimits))
	{
		if (m_nodeCells.size() > 0)
		{
			for (auto& cell : m_nodeCells)
			{
				visibleCells.push_back(cell);
			}
		}
		else
		{
			if (m_topLeft != nullptr)
			{
				m_topLeft->render(cam, visibleCells);
			}
			if (m_topRight != nullptr)
			{
				m_topRight->render(cam, visibleCells);
			}
			if (m_bottomLeft != nullptr)
			{
				m_bottomLeft->render(cam, visibleCells);
			}
			if (m_bottomRight != nullptr)
			{
				m_bottomRight->render(cam, visibleCells);
			}
		}
	}
}

