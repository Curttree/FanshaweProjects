#pragma once

// This represents a single mesh object we are drawing

#include <string>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp>
#include <vector>
#include "Animation/cBoneHierarchy.h"
#include <Animation/Animation.h>
#include <Graphics/MeshData.h>

class cMesh
{
public:
	cMesh(std::string _name);
	std::string meshName;		// The 3D Mesh model we are drawing. Should be made private if I think it will be abused. Really should use settter instead of calling directly.
	std::string midDetailMeshName;
	std::string lowDetailMeshName;
	void setAllMeshNames(std::string name);

	// Use these values to search for this specific instance of the object
	// We would set these ourselves...
	std::string friendlyName;
	unsigned int friendlyID;

	glm::vec3 positionXYZ;
	glm::vec3 orientationXYZ;		// "angle"
	glm::vec3 scale;

	// Sets the overall colour of the object
	//	by overwriting the colour variable
	// HACK: We'll get rid of this once we have textures
	glm::vec4 objectDebugColourRGBA;
	bool bUseObjectDebugColour;
	bool bDontLight;
	// Changes polygon mode to LINES (instead of solid)
	bool bIsWireframe;
	// Turns of the depth buffer check when drawn
	bool bDisableDepthBufferCheck;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	glm::vec4 wholeObjectDiffuseRGBA;		// The "colour" of the object
	bool bUseWholeObjectDiffuseColour;		// If true, then wholeObject colour (not model vertex) is used

	glm::vec3 wholeObjectSpecularRGB;		// Specular HIGHLIGHT colour (usually the same as the light, or white)
	float wholeObjectShininess_SpecPower;	// 1.0 to ??

	// Alpha Transparency (0.0 to 1.0f)
	float alphaTransparency;

	static const unsigned int MAX_TEXTURES = 9;
	std::string textureNames[MAX_TEXTURES];
	float textureRatios[MAX_TEXTURES];

	unsigned int getUniqueID(void);

	bool bUseDiscardTransparency;

	// All the "child" mesh objects
	std::vector< cMesh* > vec_pChildMeshes;

	bool bUseBones;

	cBoneHierarchy* bones;

	float boundingRadius = -1.f;

	MeshData* meshData;

	void UpdateTransforms(float time1, Animation* animation1, float time2,
		Animation* animation2, float factor, std::vector<glm::mat4>& transforms,
		std::vector<glm::mat4>& globals, std::vector<glm::mat4>& offsets);

private:
	unsigned int m_UniqueID;
	static unsigned int m_NextID;
	// All our unique objects start at 9999
	const static unsigned int INITIAL_UNIQUE_ID = 9999;
};
