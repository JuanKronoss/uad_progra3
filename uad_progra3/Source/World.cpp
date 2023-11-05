//#include "../stdafx.h"
#include "../Include/CTextureLoader.h"
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

	for (auto& model : m_modelsLoaded)
	{
		for (auto& id : model.second.textureIDMap)
		{
			m_OpenGLRenderer->deleteTexture(&id.second);
		}
	}

	for (auto& model : m_modelsLoaded)
	{
		for (auto& id : model.second.geometryIDMap)
		{
			m_OpenGLRenderer->freeGraphicsMemoryForObject(&id.second);
		}
	}
}

void World::loadWorld(string jsonGridFile, string mediaDirectory)
{
	ifstream gridFile(jsonGridFile);

	if (!gridFile)
	{
		cout << endl << "Failed to load json file!" << endl;
		return;
	}

	json data = json::parse(gridFile);

	loadGridProperties(data);
	loadHexGrid();
	loadModels(data, mediaDirectory);
	loadModelInstances(data);
}

void World::loadGridProperties(json& data)
{
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

			gridHex.m_orientation = GridProperties.orientation;
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



void World::loadModels(json& data, string mediaDirectory)
{
	for (auto& model : data["Models"])
	{
		string objName = model["name"];
		string fileName = model["filename"];
		string objPath(mediaDirectory + fileName);

		m_modelsPath[objName] = objPath;
	}

	//loadModelInstances(data);
}

void World::loadModelInstances(json& data)
{
	for (auto& instance : data["ModelInstances"])
	{
		ModelInstance model;

		string modelName = instance["model"];
		model.m_modelName = modelName;

		if (model.m_modelName == "Rock_3")
		{
			model.m_color[0] = 0.0f;
			model.m_color[1] = 0.0f;
			model.m_color[2] = 0.0f;
		}

		model.m_row = instance["row"];
		model.m_column = instance["column"];
		model.m_scale = instance["scale"];

		vector<float> rotation = instance["rotation"];
		model.m_rotation.X = rotation[0];
		model.m_rotation.Y = rotation[1];
		model.m_rotation.Z = rotation[2];

		for (Hex& hex : m_hexes)
		{

			if (hex.m_row == model.m_row &&
				hex.m_column == model.m_column)
			{
				model.m_center = hex.m_center;

				MathHelper::Matrix4 modelMatrixRotation = MathHelper::RotAroundX(model.m_rotation.X);
									modelMatrixRotation = MathHelper::RotAroundY(model.m_rotation.Y);
									modelMatrixRotation = MathHelper::RotAroundZ(model.m_rotation.Z);

				MathHelper::Matrix4 modelMatrixTranslation = MathHelper::TranslationMatrix(model.m_center.X, model.m_center.Y, model.m_center.Z);
				MathHelper::Matrix4 modelMatrixScale = MathHelper::ScaleMatrix(model.m_scale, model.m_scale, model.m_scale);

				MathHelper::Matrix4 modelMatrixRotScale = MathHelper::Multiply(modelMatrixRotation, modelMatrixScale);

				MathHelper::Matrix4 modelMatrix = MathHelper::Multiply(modelMatrixRotScale, modelMatrixTranslation);

				model.m_modelMatrix = modelMatrix;
				m_modelInstances.push_back(model);
			}
		}
	}
}



void World::allocateGraphicMemory()
{
	allocateHex();

	for (auto& model : m_modelsPath)
	{
		allocateModels(model.first, model.second);

	}
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


void World::allocateModels(string model_name, string obj_file)
{
	C3DObj objReader;
	
	if (!objReader.readObjFile(obj_file.c_str()))
	{
		cout << "\a\nUnable to load 3D model\n";
	}
	else
	{
		if (objReader.hasUVs() && objReader.hasTextures())
		{
			unordered_map<string, string>* materialFiles = objReader.getMaterialFiles();

			// Switch shaders to textured object ones
			Model.shaderID = m_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

			for (auto& file : *materialFiles)
			{
				unsigned int newTextureID;

				// LOAD TEXTURE AND ALSO CREATE TEXTURE OBJECT
				if (CTextureLoader::loadTexture(file.second.c_str(), &newTextureID, m_OpenGLRenderer))
				{
					Model.textureIDMap[file.first] = newTextureID;
				}
				else
				{
					// Texture could not be loaded, default back to color shader
					Model.shaderID = m_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
				}
			}

		}
		else
		{
			// Load color shader
			Model.shaderID = m_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
		}

		unordered_map<string, MaterialFaces*>* facesPerMaterial = objReader.getFacesPerMaterial();

		for (auto& faces : *facesPerMaterial)
		{
			vector<unsigned short> verticesIdx;
			vector<unsigned short> normalsIdx;
			vector<unsigned short> texturesIdx;

			MaterialFaces* currentFaces = faces.second;

			verticesIdx = currentFaces->getFacesVerticesIdx();
			normalsIdx = currentFaces->getFacesNormalsIdx();
			texturesIdx = currentFaces->getFacesTexturesIdx();

			bool loadedToGraphicsCard;
			unsigned int geometryID;

			// Allocate graphics memory for object
			loadedToGraphicsCard = m_OpenGLRenderer->allocateGraphicsMemoryForObject(
				&Model.shaderID,
				&geometryID,
				objReader.getVertices()->data(),
				objReader.getVertices()->size() / 3,
				objReader.getNormals()->data(),
				objReader.getNormals()->size() / 3,
				objReader.getTextureCoords()->data(),
				objReader.getTextureCoords()->size() / 2,
				verticesIdx.data(),
				verticesIdx.size() / 3,
				normalsIdx.data(),
				normalsIdx.size() / 3,
				texturesIdx.data(),
				texturesIdx.size() / 3
			);

			if (!loadedToGraphicsCard)
			{
				cout << "\a\nUnable to allocate graphic memory!\n";
			}
			else
			{
				Model.geometryIDMap[faces.first] = geometryID;
				
				Model.numFacesInMtl[faces.first] = verticesIdx.size() / 3;
			}
		}
		
		m_modelsLoaded[model_name] = Model;
		
		Model.geometryIDMap.clear();
		Model.numFacesInMtl.clear();
	}
}


void World::render()
{
	renderHexGrid();
	renderModelInstances();
}


void World::renderHexGrid()
{
	float color[3] = { 0.0f, 2.0f, 0.0f };

	for (auto& hex : m_hexes)
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

void World::renderModelInstances()
{
	for (auto& model : m_modelInstances)
	{
		string modelName = model.m_modelName;

		for (auto& id : m_modelsLoaded[modelName].geometryIDMap)
		{
			unsigned int modelVAO = id.second;
			unsigned int modelTexture = m_modelsLoaded[modelName].textureIDMap[id.first];

			m_OpenGLRenderer->renderObject(
				&m_modelsLoaded[modelName].shaderID,
				&modelVAO,
				&modelTexture,
				m_modelsLoaded[modelName].numFacesInMtl[id.first],
				model.m_color,
				&model.m_modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}
	}
}