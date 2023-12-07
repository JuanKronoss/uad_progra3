#include "../stdafx.h"
#include "../Include/Quadtree.h"


Quadtree::Quadtree(AABB_2D treeLimits, unsigned int maxFaces, vector<Hex>* hexes):
	m_treeLimits(treeLimits), m_maxFaces(maxFaces), m_hexes(hexes)
{
	m_root = new QuadtreeNode(m_treeLimits, m_maxFaces, m_hexes);
}


Quadtree::~Quadtree()
{
	if (m_root != nullptr)
	{
		delete m_root;
		m_root = nullptr;
	}
}


void Quadtree::render(Camera* cam, vector<Hex*>& visibleCells)
{
	m_root->render(cam, visibleCells);
}
