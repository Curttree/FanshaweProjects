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

	// You could write this is you didn't want the sModelDrawInfo to return by ref
	bool LoadModelIntoVAO(std::string fileName, 
						  unsigned int shaderProgramID);

	bool LoadMeshWithAssimp(const std::string& filename);

	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	void setFilePath(std::string filePath);
	std::string getFilePath(void);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// This will be added to the path when a file is loaded
	std::string m_FilePath;

	std::string m_lastError;
	Assimp::Importer m_AssimpImporter;
	unsigned int nextID = 1;

	typedef std::map<unsigned int, MeshData*>::iterator id_mesh_iterator;
	typedef std::map<std::string, unsigned int>::iterator name_id_iterator;
	std::map<unsigned int, MeshData*> m_map_pMeshes;
	std::map<std::string/*name*/, unsigned int/*id*/> m_map_NameToId;
};

#endif	// _cVAOManager_HG_
