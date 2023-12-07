#pragma once
#include "Hex.h"
#include "AABB_2D.h"
#include "CCamera.h"

#include <vector>

using std::vector;

class QuadtreeNode
{
public:
	QuadtreeNode(AABB_2D nodeLimits, unsigned int maxFaces, vector<Hex>* hexes);
	~QuadtreeNode();

	void render(Camera* cam, vector<Hex*>& visibleCells);

private:

	QuadtreeNode* m_topLeft = nullptr;
	QuadtreeNode* m_topRight = nullptr;
	QuadtreeNode* m_bottomLeft = nullptr;
	QuadtreeNode* m_bottomRight = nullptr;

	vector<Hex*> m_nodeCells;
	vector<Hex>* m_hexes;

	AABB_2D m_nodeLimits;
	unsigned int m_maxFaces;
};