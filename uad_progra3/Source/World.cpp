//#include "../stdafx.h"
#include "../Include/World.h"
#include "../Include/Globals.h"
//#include "../Include/CApp.h"
//#include "../Include/C3DObj.h"
#include "../Include//MathHelper.h"
#include <iostream>
using std::cout;
using std::endl;

World::~World()
{
	m_OpenGLRenderer->freeGraphicsMemoryForObject(&GridProperties.BaseHex.geometryID);

}

void World::loadWorld(string jsonGridFile)
{
	//ifstream gridFile(jsonGridFile);
	ifstream gridFile("Resources\\MEDIA\\HEXGRID\\hexgrid_cfg.json");
	if (!gridFile)
	{
		cout << endl << "Failed to load json file!" << endl;
		return;
	}

	json data = json::parse(gridFile);

	//"numCols"     : 12,
	//"numRows"     : 12,
	//"cellSize"    : 1.75,
	//"orientation" : "pointy"

	auto hexgrid = data["HexGrid"];

	GridProperties.numCols = hexgrid["numCols"];
	GridProperties.numRows = hexgrid["numRows"];
	GridProperties.cellSize = hexgrid["cellSize"];
	string orientation = hexgrid["orientation"];
	GridProperties.orientation = orientation;

	if (GridProperties.orientation == "pointy")
	{
		GridProperties.horizontalSpacing = GridProperties.cellSize * sqrt(3);
		GridProperties.verticalSpacing = GridProperties.cellSize * (3 * 0.5);
	}
	else if (GridProperties.orientation == "flat")
	{
		GridProperties.horizontalSpacing = GridProperties.cellSize * (3 * 0.5);
		GridProperties.verticalSpacing = GridProperties.cellSize * sqrt(3);
	}

	loadHexGrid();
	//void loadModels();
	//void loadModelInstances();
	//allocateGraphicMemory();
}

void World::loadHexGrid()
{
	loadBaseHex();
	
	if (GridProperties.orientation == "pointy")
	{
		oddR();
	}
	else if (GridProperties.orientation == "flat")
	{
		oddQ();
	}

}

void World::loadBaseHex()
{
	CVector3 cellCenter = CVector3::ZeroVector();

	for (int v = 0; v < 6; ++v)
	{
		BaseHexVrtx(cellCenter, GridProperties.cellSize, v);
	}
	BaseHexNormals();
	BaseHexUVs();
	BaseHexFaces();
}

void World::oddR()
{
	CVector3 cellCenter = CVector3::ZeroVector();

	CVector3 initCellCenter = CVector3::ZeroVector();
	initCellCenter.X -= (GridProperties.numCols -1) * GridProperties.horizontalSpacing * 0.5;
	initCellCenter.Z -= (GridProperties.numRows - 1) * GridProperties.verticalSpacing * 0.5;

	cellCenter = initCellCenter;

	for (int row = 0; row < GridProperties.numRows; ++row)
	{
		cellCenter.X = initCellCenter.X;

		//Using odd-r horizontal layout
		if (row % 2 != 0)
		{
			cellCenter.X += GridProperties.horizontalSpacing * 0.5f;
		}

		for (int column = 0; column < GridProperties.numCols; ++column)
		{
			if (column != 0)
			{
				cellCenter.X += GridProperties.horizontalSpacing;
			}
			MathHelper::Matrix4 modelMatrixScale = MathHelper::ScaleMatrix(0.95f, 1.0f, 0.95f);
			MathHelper::Matrix4 modelMatrixTranslation = MathHelper::TranslationMatrix(cellCenter.X, cellCenter.Y, cellCenter.Z);
			MathHelper::Matrix4 modelMatrix = MathHelper::Multiply(modelMatrixScale, modelMatrixTranslation);

			Hex gridHex;

			gridHex.m_center = cellCenter;
			gridHex.m_column = column;
			gridHex.m_row = row;
			gridHex.m_modelMatrix = modelMatrix;

			m_hexes.push_back(gridHex);
		}
		cellCenter.Z += GridProperties.verticalSpacing;
	}
}

void World::oddQ()
{
	CVector3 cellCenter = CVector3::ZeroVector();

	CVector3 initCellCenter = CVector3::ZeroVector();
	initCellCenter.X -= (GridProperties.numCols - 1) * GridProperties.horizontalSpacing * 0.5;
	initCellCenter.Z -= (GridProperties.numRows - 1) * GridProperties.verticalSpacing * 0.5;

	cellCenter = initCellCenter;

	for (int row = 0; row < GridProperties.numRows; ++row)
	{
		cellCenter.X = initCellCenter.X;

		for (int column = 0; column < GridProperties.numCols; ++column)
		{
			//Using odd-q vertical layout
			float downOffset = 0.0f;

			if (column % 2 != 0)
			{
				downOffset += GridProperties.verticalSpacing * 0.5f;
			}

			if (column != 0)
			{
				cellCenter.X += GridProperties.horizontalSpacing;
				cellCenter.Z +=downOffset;
			}
			MathHelper::Matrix4 modelMatrixScale = MathHelper::ScaleMatrix(0.95f, 1.0f, 0.95f);
			MathHelper::Matrix4 modelMatrixTranslation = MathHelper::TranslationMatrix(cellCenter.X, cellCenter.Y, cellCenter.Z);
			MathHelper::Matrix4 modelMatrix = MathHelper::Multiply(modelMatrixScale, modelMatrixTranslation);

			Hex gridHex;

			gridHex.m_center = cellCenter;
			gridHex.m_column = column;
			gridHex.m_row = row;
			gridHex.m_modelMatrix = modelMatrix;

			m_hexes.push_back(gridHex);

			cellCenter.Z -= downOffset;
		}
		cellCenter.Z += GridProperties.verticalSpacing;
	}
}

void World::BaseHexVrtx(CVector3 cellCenter, float cellSize, int numVertex)
{
	float PI = std::acos(-1);
	float angle = 30.0f;

	if (GridProperties.orientation == "pointy")
	{
		angle = 30.0f;
	}
	else if (GridProperties.orientation == "flat")
	{
		angle = 0.0f;
	}

	float angleDeg = 60 * numVertex - angle;
	float angleRad = PI / 180 * angleDeg;

	Point vertex(cellCenter.X + cellSize * cos(angleRad), CVector3::ZeroVector().Y, cellCenter.Z + cellSize * sin(angleRad));
	//Point vertex(cellCenter.X + cellSize * cos(angleRad), cellCenter.Z + cellSize * sin(angleRad), 0.0f);

	GridProperties.BaseHex.vertices.push_back(vertex.X);
	GridProperties.BaseHex.vertices.push_back(vertex.Y);
	GridProperties.BaseHex.vertices.push_back(vertex.Z);
}

void World::BaseHexNormals()
{
	GridProperties.BaseHex.normals = { 0.0f, 1.0f, 0.0f };

	//GridProperties.BaseHex.normals.push_back(0.0f);
	//GridProperties.BaseHex.normals.push_back(1.0f);
	//GridProperties.BaseHex.normals.push_back(0.0f);
}

void World::BaseHexUVs()
{
	GridProperties.BaseHex.UVs = { 0.0f, 0.0f };

	//GridProperties.BaseHex.UVs.push_back(0.0f);
	//GridProperties.BaseHex.UVs.push_back(0.0f);
}

void World::BaseHexFaces()
{
	GridProperties.BaseHex.faces.faceVerticesIdx = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 };
	GridProperties.BaseHex.faces.faceTexturesIdx = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	GridProperties.BaseHex.faces.faceNormalsIdx = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}



void World::allocateGraphicMemory()
{
	allocateHex();
}

void World::allocateHex()
{
	// Load color shader
	GridProperties.BaseHex.shaderID = m_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	bool loadedToGraphicsCard;
	unsigned int geometryID;

	// Allocate graphics memory for object
	loadedToGraphicsCard = m_OpenGLRenderer->allocateGraphicsMemoryForObject(
		&GridProperties.BaseHex.shaderID,
		&geometryID,
		GridProperties.BaseHex.vertices.data(),
		GridProperties.BaseHex.vertices.size() / 3,
		GridProperties.BaseHex.normals.data(),
		GridProperties.BaseHex.normals.size() / 3,
		GridProperties.BaseHex.UVs.data(),
		GridProperties.BaseHex.UVs.size() / 2,
		GridProperties.BaseHex.faces.faceVerticesIdx.data(),
		GridProperties.BaseHex.faces.faceVerticesIdx.size() / 3,
		GridProperties.BaseHex.faces.faceNormalsIdx.data(),
		GridProperties.BaseHex.faces.faceNormalsIdx.size() / 3,
		GridProperties.BaseHex.faces.faceTexturesIdx.data(),
		GridProperties.BaseHex.faces.faceTexturesIdx.size() / 3
	);

	if (!loadedToGraphicsCard)
	{
		cout << "\a\nUnable to allocate graphic memory!\n";
	}
	else
	{
		GridProperties.BaseHex.geometryID = geometryID;
		GridProperties.BaseHex.numFaces = GridProperties.BaseHex.faces.faceVerticesIdx.size() / 3; 
	}
}



void World::render()
{
	float color[3] = { 1.0f, 1.0f, 1.0f };

	for (auto hex : m_hexes)
	{
		unsigned int modelTexture = 0;

		m_OpenGLRenderer->renderObject(
			&GridProperties.BaseHex.shaderID,
			&GridProperties.BaseHex.geometryID,
			&modelTexture,
			GridProperties.BaseHex.numFaces,
			color,
			&hex.m_modelMatrix,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);
	}
}
