#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn
#include <string>
#include <map>
#include "sVertex_types.h"
#include <assimp/Importer.hpp>
#include "MeshData.h"

struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex_XYZW_RGBA_N_UV_T_B* pVertices;	//  = 0;
	unsigned int* pIndices;

};

class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	bool LoadMeshWithAssimp(const std::string& filename,
		unsigned int shaderProgramID);

	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	bool FindBonesByModelName(std::string filename,
		cBoneHierarchy*& boneHierarchy);

	bool FindAnimationByName(std::string name,
		Animation*& animation);

	std::string getLastError(bool bAndClear = true);

	void setFilePath(std::string filePath);
	std::string getFilePath(void);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	std::map< std::string /*model name*/,
		cBoneHierarchy* /* info needed to draw*/ >
		m_map_ModelName_to_Bones;

	std::map< std::string /*model name*/,
		Animation* /* info needed to draw*/ >
		m_map_ModelName_to_Animation;

	// This will be added to the path when a file is loaded
	std::string m_FilePath;

	std::string m_lastError;
	Assimp::Importer m_AssimpImporter;
	unsigned int nextID = 1;

	typedef std::map<unsigned int, MeshData*>::iterator id_mesh_iterator;
	typedef std::map<std::string, unsigned int>::iterator name_id_iterator;
	std::map<unsigned int, MeshData*> m_map_pMeshes;
	std::map<std::string/*name*/, unsigned int/*id*/> m_map_NameToId;

	void convertMeshData(MeshData* mesh, sModelDrawInfo& drawInfo);
};

#endif	// _cVAOManager_HG_
