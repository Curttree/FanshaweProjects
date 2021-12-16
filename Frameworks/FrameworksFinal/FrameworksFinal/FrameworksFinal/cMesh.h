#pragma once

// This represents a single mesh object we are drawing

#include <string>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp>
#include <vector>

class cMesh
{
public:
	cMesh();
	std::string meshName;		// The 3D Mesh model we are drawing

	// Use these values to search for this specific instance of the object
	// We would set these ourselves...
	std::string friendlyName;
	unsigned int friendlyID;

	glm::vec3 positionXYZ;
	glm::vec3 orientationXYZ;		// "angle"
	float scale;

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
	glm::vec3 discardColour= glm::vec3(0.f);
	bool bUseSpecularMap;

	std::string specularMapTexture;

	// All the "child" mesh objects
	std::vector< cMesh* > vec_pChildMeshes;

	// This is used for any scene graph. 
	// When saving these to a file, you can save the 16 values of the matrix,
	//	or some other way, like storing translation, rotation, scale, etc.
	glm::mat4 matModelBaseOffset;

	// This is the final world matrix that was set at the last draw call
	// This is useful for finding where articulated (scene graph) models 
	//	ended up being 
	glm::mat4 matLastDrawCallWorldMatrix;

private:
	unsigned int m_UniqueID;
	static unsigned int m_NextID;
	// All our unique objects start at 9999
	const static unsigned int INITIAL_UNIQUE_ID = 9999;
};
