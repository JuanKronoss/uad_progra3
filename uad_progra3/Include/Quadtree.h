#pragma once
#include "QuadtreeNode.h"
#include "AABB_2D.h"
#include "CCamera.h"
#include "Hex.h"

#include <vector>

using std::vector;

class Quadtree
{
public:
	Quadtree(AABB_2D treeLimits, unsigned int maxFaces, vector<Hex>* hexes);
	~Quadtree();

	void render(Camera* cam, vector<Hex*>& visibleCells);

private:

	QuadtreeNode* m_root = nullptr;
	vector<Hex>* m_hexes = nullptr;
	AABB_2D m_treeLimits;
	unsigned int m_maxFaces;
};