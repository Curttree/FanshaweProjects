#include "GLCommon.h"

#include "cVAOManager.h"

#include <extern/glm/vec3.hpp>
#include <extern/glm/vec4.hpp>

#include "../Graphics/AssimpModel.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <Graphics/MeshData.h>
#include <Conversion.h>
#include <glm/gtx/matrix_decompose.hpp>

bool LoadPLYModelFromFile(std::string fileName, sModelDrawInfo& drawInfo);

typedef struct sFloat4
{
	float x, y, z, w;
} sFloat4;

typedef struct sUint4
{
	unsigned int x, y, z, w;
} sUint4;

typedef struct sInt4
{
	int x, y, z, w;
} sInt4;

typedef struct sVertex_p4t4n4
{
	sFloat4 Pos;
	sFloat4 TexUVx2;
	sFloat4 Normal;
} sVertex_p4t4n4;

typedef struct sVertex_p4t4n4b4
{
	sFloat4 Pos;
	sFloat4 TexUVx2;
	sFloat4 Normal;
	sFloat4 BoneWeights;
} sVertex_p4t4n4b4;

typedef struct sVertex_p4t4n4b4i4
{
	sFloat4 Colour;
	sFloat4 Pos;
	sFloat4 Normal;
	sFloat4 TexUVx2;
	sFloat4 Tangent;
	sFloat4 BiNormal;
	sFloat4 BoneWeights;
	sUint4  BoneIDs;
} sVertex_p4t4n4b4i4;

void MapBoneHierarchy(aiNode* node, cBoneHierarchy& boneHierarchy, const std::map<aiNode*, unsigned int>& aiNodePtrToBoneIdx, int depth = 0)
{
	// Create a recursive Function for this
	cBone* bone = nullptr;
	if (node->mParent != nullptr)
	{
		std::map<aiNode*, unsigned int>::const_iterator nodeIt = aiNodePtrToBoneIdx.find(node);
		if (nodeIt != aiNodePtrToBoneIdx.end())
		{
			bone = boneHierarchy.bones[nodeIt->second];
			if (boneHierarchy.rootBone == nullptr)
				boneHierarchy.rootBone = bone;
		}
	}

	for (int nodeIdx = 0; nodeIdx < node->mNumChildren; nodeIdx++)
	{
		aiNode* child = node->mChildren[nodeIdx];

		if (node->mParent != nullptr && bone != nullptr)
		{
			std::map<aiNode*, unsigned int>::const_iterator childIt = aiNodePtrToBoneIdx.find(child);
			if (childIt != aiNodePtrToBoneIdx.end())
			{
				cBone* childBone = boneHierarchy.bones[childIt->second];
				childBone->SetParent(bone);
				bone->AddChild(childBone);
			}
		}
		MapBoneHierarchy(child, boneHierarchy, aiNodePtrToBoneIdx, depth + 1);
	}
}

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;
	this->pIndices = 0;

	glm::vec3 maxValues;
	glm::vec3 minValues;

	float maxExtent;

	return;
}


bool cVAOManager::LoadModelIntoVAO(
		std::string fileName, 
		sModelDrawInfo &drawInfo,
	    unsigned int shaderProgramID)

{
	drawInfo.meshName = fileName;

    // Add the path 
    std::string fileNameWithPath = this->m_FilePath + fileName;

    LoadPLYModelFromFile(fileNameWithPath, drawInfo);

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays( 1, &(drawInfo.VAO_ID) );
	// "Bind" this buffer:
	// - aka "make this the 'current' VAO buffer
	glBindVertexArray(drawInfo.VAO_ID);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 

	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &(drawInfo.VertexBufferID) );

	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	// sVert vertices[3]
	glBufferData( GL_ARRAY_BUFFER, 
				  sizeof(sVertex_XYZW_RGBA_N_UV_T_B) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
				  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
				  GL_STATIC_DRAW );

	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers( 1, &(drawInfo.IndexBufferID) );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
	              sizeof( unsigned int ) * drawInfo.numberOfIndices, 
	              (GLvoid*) drawInfo.pIndices,
                  GL_STATIC_DRAW );

    // ****************************************************************
	// Set the vertex attributes.

	// Set the vertex attributes for this shader
	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	    // program
	glEnableVertexAttribArray(vpos_location);	    // vPosition
	glVertexAttribPointer( vpos_location, 4,		// vPosition
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,      
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, x));

	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// program;
	glEnableVertexAttribArray(vcol_location);	    // vColour
	glVertexAttribPointer( vcol_location, 4,		// vColour
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,   
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, r));

    // And all these, too:
    //in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
 	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
	glEnableVertexAttribArray(vNormal_location);	    // vNormal
	glVertexAttribPointer(vNormal_location, 4,		    // vNormal
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),    
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, nx));


    //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
 	GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
	glEnableVertexAttribArray(vUVx2_location);	        // vUVx2
	glVertexAttribPointer(vUVx2_location, 4,		    // vUVx2
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),    
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, u0));


    //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
 	GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	// program;
	glEnableVertexAttribArray(vTangent_location);	    // vTangent
	glVertexAttribPointer(vTangent_location, 4,		    // vTangent
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),    
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, tx));


    //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
 	GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	// program;
	glEnableVertexAttribArray(vBiNormal_location);	        // vBiNormal
	glVertexAttribPointer(vBiNormal_location, 4,		    // vBiNormal
						   GL_FLOAT, GL_FALSE,
                           sizeof(sVertex_XYZW_RGBA_N_UV_T_B),    
						   ( void* ) offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bx));

	//in vec4 vBoneWeights;				// X,Y,Z (W ignored)
	GLint vBoneWeight_location = glGetAttribLocation(shaderProgramID, "vBoneWeights");	// program;
	glEnableVertexAttribArray(vBoneWeight_location);	        // vBoneWeights
	glVertexAttribPointer(vBoneWeight_location, 4,		    // vBoneWeights
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
		(void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bWx));

	//in vec4 vBoneIDs;				// X,Y,Z (W ignored)
	GLint vBoneIDs_location = glGetAttribLocation(shaderProgramID, "vBoneIDs");	// program;
	glEnableVertexAttribArray(vBoneIDs_location);	        // vBoneIDs
	glVertexAttribPointer(vBoneIDs_location, 4,		    // vBoneIDs
		GL_INT, GL_FALSE,
		sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
		(void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bIx));


    // ****************************************************************

	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
   // And the newer ones:
    glDisableVertexAttribArray(vNormal_location);	    // vNormal
    glDisableVertexAttribArray(vUVx2_location);	        // vUVx2
    glDisableVertexAttribArray(vTangent_location);	    // vTangent
    glDisableVertexAttribArray(vBiNormal_location);	        // vBiNormal
	glDisableVertexAttribArray(vBoneWeight_location);	        // vBiNormal
	glDisableVertexAttribArray(vBoneIDs_location);	        // vBiNormal

	// Store the draw information into the map
	this->m_map_ModelName_to_VAOID[ drawInfo.meshName ] = drawInfo;

	return true;
}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo) 
{
	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}

bool cVAOManager::FindBonesByModelName(
	std::string filename,
	cBoneHierarchy*& boneHierarchy)
{
	std::map< std::string /*model name*/,
		cBoneHierarchy* /* info needed to draw*/ >::iterator
		bones = this->m_map_ModelName_to_Bones.find(filename);

	// Find it? 
	if (bones == this->m_map_ModelName_to_Bones.end())
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	boneHierarchy = bones->second;
	return true;
}

bool cVAOManager::FindAnimationByName(
	std::string filename,
	Animation*& animation)
{
	std::map< std::string /*model name*/,
		Animation* /* info needed to draw*/ >::iterator
		anim = this->m_map_ModelName_to_Animation.find(filename);

	// Find it? 
	if (anim == this->m_map_ModelName_to_Animation.end())
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	animation = anim->second;
	return true;
}

bool LoadPLYModelFromFile(std::string fileName, sModelDrawInfo& drawInfo)
{
    // These structures match the PLY file format
    struct sVertex
    {
        float x, y, z;      // , c, i;
        float nx, ny, nz;
        float red, green, blue, alpha;  // (Note the file is in HTML style, so 0 to 255, but the shader wants 0.0 to 1.0)
        float u, v;
		float boneId;
		float boneWeight;
    };
    struct sTriangle
    {
        unsigned int vertIndex[3];
    };

    std::ifstream theFile(fileName.c_str());

    if (!theFile.is_open())          // theFile.is_open() == false
    {
        return false;
    }

    std::string nextToken;

    // Scan until we find the word "vertex"...
    while (theFile >> nextToken)
    {
         if (nextToken == "vertex")
        {
            break;  // Exits the while loop
        }
    }

    // Read the number of vertices
    theFile >> drawInfo.numberOfVertices;

    // Scan until we find the word "face"...
    while (theFile >> nextToken)
    {
        if (nextToken == "face")
        {
            break;  // Exits the while loop
        }
    }

    // Read the number of triangles (aka "faces")
    theFile >> drawInfo.numberOfTriangles;

    // Scan until we find the word "end_header"...
    while (theFile >> nextToken)
    {
        if (nextToken == "end_header")
        {
            break;  // Exits the while loop
        }
    }

    std::vector<sVertex> vecVertexArray;

    // Now we can read the vertices (in a for loop)
    for (unsigned int index = 0; index < drawInfo.numberOfVertices; index++)
    {
        sVertex tempVertex;

        theFile >> tempVertex.x;
        theFile >> tempVertex.y;
        theFile >> tempVertex.z;
        //        theFile >> tempVertex.c;
        //       theFile >> tempVertex.i;

        // Now load normals:
        theFile >> tempVertex.nx;
        theFile >> tempVertex.ny;
        theFile >> tempVertex.nz;

        // Now load RGBA colours:
        theFile >> tempVertex.red;
        theFile >> tempVertex.green;
        theFile >> tempVertex.blue;
        theFile >> tempVertex.alpha;       
        
        theFile >> tempVertex.u;
        theFile >> tempVertex.v;

        vecVertexArray.push_back(tempVertex);
    }

    std::vector<sTriangle> vecTriagleArray;

    // Now we can read the triangles (in a for loop)
    for (unsigned int index = 0; index < drawInfo.numberOfTriangles; index++)
    {
        sTriangle tempTri;
        int discardThis;

        theFile >> discardThis; // The "3" at the start
        theFile >> tempTri.vertIndex[0];
        theFile >> tempTri.vertIndex[1];
        theFile >> tempTri.vertIndex[2];

        vecTriagleArray.push_back(tempTri);
    }

    // Allocate the amount of space we need for the GPU side arrays
    drawInfo.pVertices = new sVertex_XYZW_RGBA_N_UV_T_B[drawInfo.numberOfVertices];

    // There are 3 indices per triangle...
    drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

    // Copy the vertices from the PLY format vector
    //  to the one we'll use to draw in the GPU
    for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
    {
        // Copy position...
        drawInfo.pVertices[index].x = vecVertexArray[index].x;
        drawInfo.pVertices[index].y = vecVertexArray[index].y;
        drawInfo.pVertices[index].z = vecVertexArray[index].z;
        drawInfo.pVertices[index].w = 1.0f;     // Not currently utilizing. Set to 1.

        // Copy colour...
        // And convert from 0-255 into 0-1
        drawInfo.pVertices[index].r = (vecVertexArray[index].red / 255.0f);
        drawInfo.pVertices[index].g = (vecVertexArray[index].green / 255.0f);
        drawInfo.pVertices[index].b = (vecVertexArray[index].blue / 255.0f);
        drawInfo.pVertices[index].a = (vecVertexArray[index].alpha / 255.0f);

        // And copy the other things, too:
//        float nx, ny, nz, nw;   // in vec4 vNormal;	Vertex normal X,Y,Z (W ignored)
        drawInfo.pVertices[index].nx = vecVertexArray[index].nx;
        drawInfo.pVertices[index].ny = vecVertexArray[index].ny;
        drawInfo.pVertices[index].nz = vecVertexArray[index].nz;
        drawInfo.pVertices[index].nw = 1.0f;

        drawInfo.pVertices[index].u0 = vecVertexArray[index].u;
        drawInfo.pVertices[index].v0 = vecVertexArray[index].v;
        drawInfo.pVertices[index].u1 = 0.0f;
        drawInfo.pVertices[index].v1 = 0.0f;

		drawInfo.pVertices[index].bIx = vecVertexArray[index].boneId;
		drawInfo.pVertices[index].bWx = vecVertexArray[index].boneWeight;
       }

    // Copy the triangle ("index") values to the index (element) array
    unsigned int elementIndex = 0;
    for (unsigned int triIndex = 0; triIndex < drawInfo.numberOfTriangles; 
         triIndex++, elementIndex += 3)
    {
        drawInfo.pIndices[elementIndex + 0] = vecTriagleArray[triIndex].vertIndex[0];
        drawInfo.pIndices[elementIndex + 1] = vecTriagleArray[triIndex].vertIndex[1];
        drawInfo.pIndices[elementIndex + 2] = vecTriagleArray[triIndex].vertIndex[2];
    }

    return true;
}

bool cVAOManager::LoadMeshWithAssimp(const std::string& filename,
	unsigned int shaderProgramID)
{
	const aiScene* scene = m_AssimpImporter.ReadFile(m_FilePath + filename,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_PopulateArmatureData |
		aiProcess_FixInfacingNormals |
		aiProcess_LimitBoneWeights);


	if (scene == nullptr)
	{
		printf("MeshManager::LoadMeshWithAssimp: ERROR: Failed to load file %s\n", filename.c_str());
		return false;
	}

	if (!scene->HasMeshes())
	{
		printf("MeshManager::LoadMeshWithAssimp: ERROR: Model file does not contain any meshes %s\n", filename.c_str());
		return false;
	}

	MeshData* meshData = new MeshData();
	aiMesh* mesh = scene->mMeshes[0];
	meshData->mGlobalInvereseTransformation = Convert(scene->mRootNode->mTransformation);
	meshData->mGlobalInvereseTransformation = glm::inverse(meshData->mGlobalInvereseTransformation);

	meshData->LoadBones(mesh);
	meshData->LoadNodes(scene->mRootNode, true);

	aiNode* rootNode;
	{
		std::map<unsigned int, std::vector<std::pair<unsigned int, float>>> map_BoneWeights;
		//----------------------------------------------------
		//-- Iterate through all of the Bones
		unsigned int NumBones = mesh->mNumBones;
		for (unsigned int BoneIndex = 0; BoneIndex != NumBones; BoneIndex++)
		{
			aiBone* CurrentBone = mesh->mBones[BoneIndex];

			aiString BoneName = CurrentBone->mName;
			aiMatrix4x4 offsetMatrix = CurrentBone->mOffsetMatrix;

			unsigned int NumWeights = CurrentBone->mNumWeights;
			for (unsigned int WeightIndex = 0; WeightIndex != NumWeights; WeightIndex++)
			{
				aiVertexWeight VertexWeight = CurrentBone->mWeights[WeightIndex];

				unsigned int VertexID = VertexWeight.mVertexId;
				float Weight = VertexWeight.mWeight;

				map_BoneWeights[VertexID].push_back(std::pair<unsigned int, float>(BoneIndex, Weight));
				//printf("%d, %.2f\n", BoneIndex, Weight);
			}
		}

		for (unsigned int VertexIndex = 0; VertexIndex != mesh->mNumVertices; VertexIndex++)
		{
			for (unsigned int x = map_BoneWeights[VertexIndex].size(); x < 4; x++)
			{
				map_BoneWeights[VertexIndex].push_back(std::pair<unsigned int, float>(0, 0.0f));
			}
		}

		unsigned int numVerticesInVertArray = mesh->mNumVertices;
		unsigned int numIndicesInIndexArray = mesh->mNumFaces * 3;
		sVertex_p4t4n4b4i4* pTempVertArray = new sVertex_p4t4n4b4i4[numIndicesInIndexArray * 2];
		GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
		int count = 0;
		int index = 0;

		//printf("\n\nfaces:\n");
		unsigned int vertArrayIndex = 0;
		for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
		{
			aiFace face = mesh->mFaces[faceIdx];

			for (int idx = 0; idx != 3; idx++)
			{
				unsigned int index = face.mIndices[idx];
				//printf("%d ", index);

				aiVector3D position = mesh->mVertices[face.mIndices[idx]];
				pTempVertArray[vertArrayIndex].Pos.x = position.x;
				pTempVertArray[vertArrayIndex].Pos.y = position.y;
				pTempVertArray[vertArrayIndex].Pos.z = position.z;
				pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

				if (mesh->HasTextureCoords(0)) {
					aiVector3D textureCoord = mesh->mTextureCoords[0][face.mIndices[idx]];
					pTempVertArray[vertArrayIndex].TexUVx2.x = textureCoord.x;
					pTempVertArray[vertArrayIndex].TexUVx2.y = textureCoord.y;
					pTempVertArray[vertArrayIndex].TexUVx2.z = textureCoord.z;
					pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
				}
				else
				{
					pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
				}

				aiVector3D normal = mesh->mNormals[face.mIndices[idx]];
				pTempVertArray[vertArrayIndex].Normal.x = normal.x;
				pTempVertArray[vertArrayIndex].Normal.y = normal.y;
				pTempVertArray[vertArrayIndex].Normal.z = normal.z;
				pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

				pTempVertArray[vertArrayIndex].BoneIDs.x = meshData->mVertexBoneData[index].IDs[0];;
				pTempVertArray[vertArrayIndex].BoneIDs.y = meshData->mVertexBoneData[index].IDs[1];;
				pTempVertArray[vertArrayIndex].BoneIDs.z = meshData->mVertexBoneData[index].IDs[2];;
				pTempVertArray[vertArrayIndex].BoneIDs.w = meshData->mVertexBoneData[index].IDs[3];;

				// We will come back to this!!
				// Smoothing the bone transforms.
				pTempVertArray[vertArrayIndex].BoneWeights.x = meshData->mVertexBoneData[index].weights[0];;
				pTempVertArray[vertArrayIndex].BoneWeights.y = meshData->mVertexBoneData[index].weights[1];;
				pTempVertArray[vertArrayIndex].BoneWeights.z = meshData->mVertexBoneData[index].weights[2];;
				pTempVertArray[vertArrayIndex].BoneWeights.w = meshData->mVertexBoneData[index].weights[3];;

				//Garbage data.
				pTempVertArray[vertArrayIndex].Colour.x = 1.f;
				pTempVertArray[vertArrayIndex].Colour.y = 1.f;
				pTempVertArray[vertArrayIndex].Colour.z = 1.f;
				pTempVertArray[vertArrayIndex].Colour.w = 1.f;

				pTempVertArray[vertArrayIndex].Tangent.x = 0.f;
				pTempVertArray[vertArrayIndex].Tangent.y = 0.f;
				pTempVertArray[vertArrayIndex].Tangent.z = 0.f;
				pTempVertArray[vertArrayIndex].Tangent.w = 0.f;

				pTempVertArray[vertArrayIndex].BiNormal.x = 0.f;
				pTempVertArray[vertArrayIndex].BiNormal.y = 0.f;
				pTempVertArray[vertArrayIndex].BiNormal.z = 0.f;
				pTempVertArray[vertArrayIndex].BiNormal.w = 0.f;


				pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

				vertArrayIndex++;
			}
		}

		struct BoneInfo
		{
			glm::mat4 BoneOffset;
		};

		unsigned int numBones = 0;
		std::vector<int> Bones;
		std::vector<BoneInfo> boneInfo;
		std::map<std::string, unsigned int> boneMapping;
		for (unsigned int i = 0; i < mesh->mNumBones; ++i)
		{
			unsigned int boneIndex = 0;
			std::string boneName(mesh->mBones[i]->mName.data);

			if (boneMapping.find(boneName) == boneMapping.end())
			{
				boneIndex = numBones;
				numBones++;
				BoneInfo bi;
				boneInfo.push_back(bi);
			}
			else
			{
				boneIndex = boneMapping[boneName];
			}

			boneMapping[boneName] = boneIndex;
			Convert(mesh->mBones[i]->mOffsetMatrix, boneInfo[boneIndex].BoneOffset);

			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
			{
				unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;

			}
		}

		if (mesh->HasBones())
		{
			// Create bone hierarchy
			aiNode* rootNode = nullptr;
			std::map<aiNode*, unsigned int> aiNodePtrToBoneIdx;
			cBoneHierarchy* boneH = new cBoneHierarchy();
			meshData->boneHierarchy.currentTransforms.resize(mesh->mNumBones);

			for (int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
			{
				aiBone* assimpBone = mesh->mBones[boneIdx];
				aiNode* assimpNode = assimpBone->mNode;

				//printf("\n%d: ", boneIdx);
				for (int i = 0; i < assimpBone->mNumWeights; ++i)
				{
					//printf(" %d", assimpBone->mWeights[i].mVertexId);
					//if (pTempVertArray[assimpBone->mWeights[i].mVertexId].BoneIDs.x == 0)
					pTempVertArray[assimpBone->mWeights[i].mVertexId].BoneIDs.x = boneIdx;
					//pTempVertArray[assimpBone->mWeights[i].mVertexId].BoneWeights.x = assimpBone->mWeights[i].mWeight;
				}
				//printf("\n");

				// Convert aiMatrix to glm Matrix
				aiVector3D assimpScale;
				aiVector3D assimpRotation;
				aiVector3D assimpPosition;

				glm::vec3 glmScale;
				glm::quat glmOrientation;
				glm::vec3 glmTranslation;
				glm::vec3 glmSkew;
				glm::vec4 glmPerspective;

				glm::mat4 glmOffsetMatrix;
				glm::mat4 glmTransformMatrix;
				Convert(assimpBone->mOffsetMatrix, glmOffsetMatrix);
				Convert(assimpNode->mTransformation, glmTransformMatrix);

				glm::decompose(glmOffsetMatrix, glmScale, glmOrientation, glmTranslation, glmSkew, glmPerspective);

				cBone* bone = new cBone();
				bone->SetRotation(glmOrientation);
				bone->SetScale(glmScale);

				glm::decompose(glmTransformMatrix, glmScale, glmOrientation, glmTranslation, glmSkew, glmPerspective);

				bone->SetPosition(glmTranslation);

				glm::mat4 BoneTranslationMatrix = glm::translate(glm::mat4(1.0f), glmTranslation);
				glm::mat4 BoneRotationMatrix = glm::mat4_cast(glmOrientation);
				glm::mat4 BoneScaleMatrix = glm::scale(glm::mat4(1.0f), glmScale);
				glm::mat4 BoneModelMatrix = BoneTranslationMatrix * BoneRotationMatrix * BoneScaleMatrix;

				meshData->boneHierarchy.currentTransforms[boneIdx].value = BoneModelMatrix;

				bone->name = assimpBone->mName.C_Str();
				bone->SetModelMatrix(BoneModelMatrix);

				aiNodePtrToBoneIdx[assimpNode] = meshData->boneHierarchy.bones.size();
				meshData->boneHierarchy.bones.push_back(bone);
			}

			rootNode = mesh->mBones[0]->mNode;
			while (rootNode->mParent != nullptr)
				rootNode = rootNode->mParent;

			MapBoneHierarchy(rootNode, meshData->boneHierarchy, aiNodePtrToBoneIdx);
			(*boneH) = meshData->boneHierarchy;
			m_map_ModelName_to_Bones.insert(std::pair<std::string,cBoneHierarchy*>(filename, boneH));
		}

		if (scene->HasAnimations())
		{
			unsigned int numAnimations = scene->mNumAnimations;

			for (int animIdx = 0; animIdx < 1; ++animIdx)
			{
				aiAnimation* aiAnim = scene->mAnimations[animIdx];

				Animation* animation = new Animation();
				animation->ticksPerSecond = aiAnim->mTicksPerSecond;
				animation->numTicks = aiAnim->mDuration;
				animation->name = aiAnim->mName.C_Str();
				animation->duration = (float)animation->numTicks / (float)animation->ticksPerSecond;

				unsigned int numChannels = aiAnim->mNumChannels;

				for (int channelIdx = 0; channelIdx < numChannels; ++channelIdx)
				{
					aiNodeAnim* aiNode = aiAnim->mChannels[channelIdx];

					AnimationNode* node = new AnimationNode();
					node->name = aiNode->mNodeName.C_Str();

					animation->animationNodes.push_back(node);

					unsigned int numPositionKeys = aiNode->mNumPositionKeys;
					for (int positionKeyIdx = 0; positionKeyIdx < numPositionKeys; ++positionKeyIdx)
					{
						aiVectorKey aiKeyframe = aiNode->mPositionKeys[positionKeyIdx];
						KeyFramePosition keyframe;
						keyframe.time = aiKeyframe.mTime / animation->ticksPerSecond;
						Convert(aiKeyframe.mValue, keyframe.position);

						node->keyFramePositions.push_back(keyframe);
					}

					unsigned int numScaleKeys = aiNode->mNumScalingKeys;
					for (int scaleKeyIdx = 0; scaleKeyIdx < numScaleKeys; ++scaleKeyIdx)
					{
						aiVectorKey aiKeyframe = aiNode->mScalingKeys[scaleKeyIdx];

						KeyFrameScale keyframe;
						keyframe.time = aiKeyframe.mTime / animation->ticksPerSecond;
						Convert(aiKeyframe.mValue, keyframe.scale);

						node->keyFrameScales.push_back(keyframe);
					}

					unsigned int numRotationKeys = aiNode->mNumRotationKeys;
					for (int rotationKeyIdx = 0; rotationKeyIdx < numRotationKeys; ++rotationKeyIdx)
					{
						aiQuatKey aiKeyframe = aiNode->mRotationKeys[rotationKeyIdx];

						KeyFrameRotation keyframe;
						keyframe.time = aiKeyframe.mTime / animation->ticksPerSecond;
						Convert(aiKeyframe.mValue, keyframe.rotation);

						node->keyFrameRotations.push_back(keyframe);
					}
				}

				meshData->animations.push_back(animation);

				// Right now we are only storing one animation per file. This could have problems if attempting to load models with multiple animations.
				// May wish to refactor to store a vector of animations, or change the key.
				m_map_ModelName_to_Animation.insert(std::pair<std::string, Animation*>(filename, animation));
			}
		}

		meshData->filename = filename;
		//meshData->objFile = model;
		meshData->numTriangles = mesh->mNumFaces;

		glGenVertexArrays(1, &meshData->GL_VBO_ID);
		glBindVertexArray(meshData->GL_VBO_ID);
		//CheckGLError();

		GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	    // program
		GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// program;
		GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
		GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
		GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	// program;
		GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	// program;
		GLint vBoneWeight_location = glGetAttribLocation(shaderProgramID, "vBoneWeights");	// program;
		GLint vBoneIDs_location = glGetAttribLocation(shaderProgramID, "vBoneIDs");	// program;

		glEnableVertexAttribArray(vpos_location);
		glEnableVertexAttribArray(vcol_location);
		glEnableVertexAttribArray(vNormal_location);
		glEnableVertexAttribArray(vUVx2_location);
		glEnableVertexAttribArray(vTangent_location);
		glEnableVertexAttribArray(vBiNormal_location);
		glEnableVertexAttribArray(vBoneWeight_location);
		glEnableVertexAttribArray(vBoneIDs_location);
		//CheckGLError();

		glGenBuffers(1, &meshData->GL_VertexBuffer_ID);
		glGenBuffers(1, &meshData->GL_IndexBuffer_ID);
		//CheckGLError();

		glBindBuffer(GL_ARRAY_BUFFER, meshData->GL_VertexBuffer_ID);
		//CheckGLError();

		unsigned int totalVertBufferSizeBYTES = numVerticesInVertArray * sizeof(sVertex_p4t4n4b4i4); ;
		glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_DYNAMIC_DRAW);
		//CheckGLError();

		unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4b4i4);
		unsigned int byteOffsetToColour = offsetof(sVertex_p4t4n4b4i4, Colour);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4b4i4, Pos);
		unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4b4i4, Normal);
		unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4b4i4, TexUVx2);
		unsigned int byteOffsetToTangent = offsetof(sVertex_p4t4n4b4i4, Tangent);
		unsigned int byteOffsetToBiNormal = offsetof(sVertex_p4t4n4b4i4, BiNormal);
		unsigned int byteOffsetToBoneWeights = offsetof(sVertex_p4t4n4b4i4, BoneWeights);
		unsigned int byteOffsetToBoneIDs = offsetof(sVertex_p4t4n4b4i4, BoneIDs);

		glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToColour);
		glVertexAttribPointer(vNormal_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
		glVertexAttribPointer(vUVx2_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
		glVertexAttribPointer(vTangent_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToTangent);
		glVertexAttribPointer(vBiNormal_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBiNormal);
		glVertexAttribPointer(vBoneWeight_location, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);
		glVertexAttribPointer(vBoneIDs_location, 4, GL_INT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIDs);
		//CheckGLError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GL_IndexBuffer_ID);
		//CheckGLError();

		unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_DYNAMIC_DRAW);
		//CheckGLError();

		glBindVertexArray(0);
		//CheckGLError();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vpos_location);
		glDisableVertexAttribArray(vcol_location);
		glDisableVertexAttribArray(vNormal_location);	    // vNormal
		glDisableVertexAttribArray(vUVx2_location);	        // vUVx2
		glDisableVertexAttribArray(vTangent_location);	    // vTangent
		glDisableVertexAttribArray(vBiNormal_location);	        // vBiNormal
		glDisableVertexAttribArray(vBoneWeight_location);	        // vBiNormal
		glDisableVertexAttribArray(vBoneIDs_location);	        // vBiNormal

		delete[] pTempVertArray;
		delete[] pIndexArrayLocal;

		sModelDrawInfo drawInfo;
		drawInfo.numberOfIndices = mesh->mNumFaces * 3;
		drawInfo.VAO_ID = meshData->GL_VBO_ID;


		this->m_map_ModelName_to_VAOID[meshData->filename] = drawInfo;
		meshData->meshId = nextID;
		m_map_NameToId[filename] = nextID;
		m_map_pMeshes[nextID] = meshData;
		nextID++;
	}

	return false;
}


std::string cVAOManager::getLastError(bool bAndClear /*=true*/)
{
    std::string error = this->m_lastError;
    if (bAndClear)
    {
        this->m_lastError = "";
    }
    return error;
}

void cVAOManager::setFilePath(std::string filePath)
{
    this->m_FilePath = filePath;
}

std::string cVAOManager::getFilePath(void)
{
    return this->m_FilePath;
}

void cVAOManager::convertMeshData(MeshData* mesh, sModelDrawInfo& drawInfo) {

}
unsigned int cVAOManager::GetMeshIdByName(const std::string& name)
{
	name_id_iterator it = this->m_map_NameToId.find(name);

	if (it == this->m_map_NameToId.end())
	{
		return 0;
	}

	return it->second;
}

bool cVAOManager::GetMeshDataByName(const std::string& name, MeshData** mesh)
{
	unsigned int meshID = this->GetMeshIdByName(name);
	if (meshID == 0)	// not set
	{
		return false;
	}

	id_mesh_iterator it = m_map_pMeshes.find(meshID);

	if (it == m_map_pMeshes.end())
	{
		return false;
	}

	*mesh = it->second;

	return true;
}

bool cVAOManager::GetMeshDataById(unsigned int id, MeshData** mesh)
{
	id_mesh_iterator it = m_map_pMeshes.find(id);

	if (it == m_map_pMeshes.end())
	{
		return false;
	}

	*mesh = it->second;

	return true;
}