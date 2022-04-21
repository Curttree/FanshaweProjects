#pragma once

#include <map>
#include <vector>
#include <assert.h>

#include <Animation/cBoneHierarchy.h>
#include <Animation/Animation.h>
#include <string>

class Material;
class ObjModel;
class aiMesh;
class aiNode;

struct BoneData
{
	glm::mat4 BoneOffset;
	glm::mat4 LocalTransformation;
	glm::mat4 GlobalTransformation;
	glm::mat4 FinalTransformation;
};

class NodeData
{
public:
	NodeData(aiNode* assimpNode);
	virtual ~NodeData();
	std::string name;
	glm::mat4 transformation;
	std::vector<NodeData*> children;
};

struct VertexBoneData
{
	void AddBoneData(unsigned int boneId, float weight);

	unsigned int IDs[4];
	float weights[4];
};

struct MeshData
{
public:
	MeshData() {}

	void LoadBones(aiMesh* mesh);
	NodeData* LoadNodes(aiNode* node, const bool isRoot = false);

	// Single Animation Update
	void UpdateTransforms(float time, Animation* animation, std::vector<glm::mat4>& transforms,
		std::vector<glm::mat4>& globals, std::vector<glm::mat4>& offsets);
	void UpdateHierarchy(float time, Animation* animation, NodeData* node, glm::mat4& parentMatrix);

	// Blend Animation Update
	void UpdateTransforms(float time1, Animation* animation1, float time2,
		Animation* animation2, float factor, std::vector<glm::mat4>& transforms,
		std::vector<glm::mat4>& globals, std::vector<glm::mat4>& offsets);
	void UpdateHierarchy(float time1, Animation* animation1, float time2,
		Animation* animation2, float factor, NodeData* node, glm::mat4& parentMatrix);

	void CalculateScaling(glm::vec3& scaling, float time, AnimationNode* node);
	void CalculateRotation(glm::quat& rotation, float time, AnimationNode* node);
	void CalculateTranslation(glm::vec3& translation, float time, AnimationNode* node);
	AnimationNode* FindAnimationNode(Animation* animation, const std::string& name);
	int FindKeyFramePositionIndex(AnimationNode* animation, float time);
	int FindKeyFrameScaleIndex(AnimationNode* animation, float time);
	int FindKeyFrameRotationIndex(AnimationNode* animation, float time);

	std::string filename;
	unsigned int	meshId;
	unsigned int	GL_VBO_ID;
	unsigned int	GL_VertexBuffer_ID;
	unsigned int	GL_IndexBuffer_ID;
	unsigned int	numTriangles;
	ObjModel* objFile;
	Material* material;

	// Temporary
	cBoneHierarchy boneHierarchy;
	std::vector<Animation*> animations;

	std::vector<VertexBoneData> mVertexBoneData;
	glm::mat4 mGlobalInvereseTransformation;

	unsigned int mNumBones;
	std::vector<BoneData> mBoneData;
	std::map<std::string, unsigned int> mBoneMap;

	NodeData* mRootNode;
	std::vector<NodeData*> mNodeData;
	//std::map<std::string, unsigned int> mNodeMap;

	std::vector<MeshData*> mVecMeshData;

	unsigned int mBaseVertex;
	unsigned int mNumIndices;
	unsigned int mBaseIndex;
};

