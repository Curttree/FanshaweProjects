#include "cMesh.h"
#include "../../GameJam/globals.h"

cMesh::cMesh(std::string _meshName) : meshName(_meshName), lowDetailMeshName(_meshName), midDetailMeshName(_meshName), highDetailMeshName(_meshName)
{
	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = glm::vec3(1.0f);

	this->bIsWireframe = false;
	this->bDisableDepthBufferCheck = false;

	this->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// White
	this->bUseObjectDebugColour = false;
	this->bDontLight = false;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	this->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// The "colour" of the object
	//
	// Specular HIGHLIGHT colour (usually the same as the light, or white)
	this->wholeObjectSpecularRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	// Specular highlight POWER (or shininess). Starts at 1.0 and goes to ? (like 100, 1000, 10000 is OK)
	this->wholeObjectShininess_SpecPower = 1.0f;	// 1.0 to ??

	// Default to 1.0f (all solid)
	this->alphaTransparency = 1.0f;

	this->bUseDiscardTransparency = false;

	this->bUseBones = false;

	::g_pVAOManager->GetMeshDataByName(meshName, &this->meshData);

	std::vector<glm::mat4> transforms;
	std::vector<glm::mat4> globals;
	std::vector<glm::mat4> offsets;
	Animation* temp;
	if (::g_pVAOManager->FindAnimationByName(_meshName, temp)) {
		meshData->UpdateTransforms(0.f, temp, transforms, globals, offsets);
	}
	// Assign a unique ID
	this->m_UniqueID = cMesh::m_NextID;
	cMesh::m_NextID++;
}


unsigned int cMesh::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static 
unsigned int cMesh::m_NextID = cMesh::INITIAL_UNIQUE_ID;

void cMesh::setAllMeshNames(std::string name) {
	meshName = name;
	lowDetailMeshName = name;
	midDetailMeshName = name;
	highDetailMeshName = name;
}

void cMesh::UpdateTransforms(float time1, Animation* animation1, float time2,
	Animation* animation2, float factor, std::vector<glm::mat4>& transforms,
	std::vector<glm::mat4>& globals, std::vector<glm::mat4>& offsets)
{
	//if (gDebugPrint)
	//	printf("\n===============================================\n\n");

	//// Update the node hierarchy, starting from the root node
	//// Update node hierarchy..
	//glm::mat4 identity(1.f);
	////AnimationNode* rootNode = animation->animationNodes[it->second];
	//UpdateHierarchy(time1, animation1, time2, animation2, factor, mRootNode, identity);

	//// Acquire all the matrix information
	//// Set all transforms, globals, and offsets
	//unsigned int numBones = mBoneData.size();
	//transforms.resize(numBones);
	//globals.resize(numBones);
	//offsets.resize(numBones);

	//for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
	//{
	//	transforms[boneIdx] = mBoneData[boneIdx].LocalTransformation;
	//	globals[boneIdx] = mBoneData[boneIdx].GlobalTransformation;
	//	offsets[boneIdx] = mBoneData[boneIdx].BoneOffset;
	//}
}