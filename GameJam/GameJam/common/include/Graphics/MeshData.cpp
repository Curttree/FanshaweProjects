#include "MeshData.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "Conversion.h"

#include <glm/gtx/matrix_decompose.hpp>

NodeData::NodeData(aiNode* assimpNode)
{
	name = std::string(assimpNode->mName.data);
	transformation = Convert(assimpNode->mTransformation);
}

NodeData::~NodeData()
{}


void VertexBoneData::AddBoneData(unsigned int boneId, float weight)
{
	for (unsigned int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); ++i)
	{
		if (weights[i] == 0.0) {
			IDs[i] = boneId;
			weights[i] = weight;
			return;
		}
	}

	assert(0);
}

void MeshData::LoadBones(aiMesh* mesh)
{
	mVertexBoneData.resize(mesh->mNumVertices);
	mNumBones = 0;
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		unsigned int boneIdx = 0;
		std::string boneName(mesh->mBones[i]->mName.data);
		std::map<std::string, unsigned int>::const_iterator it = mBoneMap.find(boneName);
		if (it == mBoneMap.end())
		{
			boneIdx = mNumBones;
			mNumBones++;
			BoneData boneData;
			mBoneData.push_back(boneData);

			mBoneData[boneIdx].BoneOffset = Convert(mesh->mBones[i]->mOffsetMatrix);
			mBoneMap[boneName] = boneIdx;
		}
		else
		{
			boneIdx = mBoneMap[boneName];
		}

		for (unsigned int weightIdx = 0; weightIdx != mesh->mBones[i]->mNumWeights; ++weightIdx)
		{
			unsigned int vertexID = mesh->mBones[i]->mWeights[weightIdx].mVertexId;
			float weight = mesh->mBones[i]->mWeights[weightIdx].mWeight;
			mVertexBoneData[vertexID].AddBoneData(boneIdx, weight);
		}
	}
}

NodeData* MeshData::LoadNodes(aiNode* assimpNode, const bool isRoot)
{
	if (assimpNode == 0)
		return 0;

	NodeData* newNode = new NodeData(assimpNode);
	mNodeData.push_back(newNode);

	if (isRoot)
		mRootNode = newNode;

	unsigned int numChildren = assimpNode->mNumChildren;
	for (int i = 0; i < numChildren; ++i)
	{
		NodeData* child = LoadNodes(assimpNode->mChildren[i], false);
		newNode->children.push_back(child);
	}

	return newNode;
}

void MeshData::UpdateTransforms(float time,
	Animation* animation,
	std::vector<glm::mat4>& transforms,
	std::vector<glm::mat4>& globals,
	std::vector<glm::mat4>& offsets)
{
	// Update the node hierarchy, starting from the root node
	// Update node hierarchy..
	glm::mat4 identity(1.f);
	UpdateHierarchy(time, animation, mRootNode, identity);

	// Acquire all the matrix information
	// Set all transforms, globals, and offsets
	unsigned int numBones = mBoneData.size();
	transforms.resize(numBones);
	globals.resize(numBones);
	offsets.resize(numBones);

	for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
	{
		transforms[boneIdx] = mBoneData[boneIdx].LocalTransformation;
		globals[boneIdx] = mBoneData[boneIdx].GlobalTransformation;
		offsets[boneIdx] = mBoneData[boneIdx].BoneOffset;
	}
}

AnimationNode* MeshData::FindAnimationNode(Animation* animation, const std::string& name)
{
	for (int i = 0; i < animation->animationNodes.size(); ++i)
	{
		if (animation->animationNodes[i]->name.compare(name) == 0)
			return animation->animationNodes[i];
	}
	return 0;
}

void MeshData::UpdateHierarchy(float time, Animation* animation, NodeData* node, glm::mat4& parentMatrix)
{
	if (node == 0)
		return;

	std::string nodeName = node->name;
	glm::mat4 transformation;

	AnimationNode* animNode = FindAnimationNode(animation, nodeName);
	transformation = node->transformation;
	if (animNode)
	{
		glm::vec3 scaling;
		CalculateScaling(scaling, time, animNode);
		glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaling);

		glm::quat rotation;
		CalculateRotation(rotation, time, animNode);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

		glm::vec3 translation;
		CalculateTranslation(translation, time, animNode);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), translation);

		transformation = translationMatrix * rotationMatrix * scalingMatrix;
	}

	glm::mat4 globalTransformation = parentMatrix * transformation;
	glm::mat4 finalTransformation;
	std::map<std::string, unsigned int>::const_iterator itBoneMap = mBoneMap.find(nodeName);
	if (itBoneMap != mBoneMap.end())
	{
		unsigned int boneIdx = itBoneMap->second;
		mBoneData[boneIdx].GlobalTransformation = globalTransformation;
		mBoneData[boneIdx].LocalTransformation = transformation;
		finalTransformation = mGlobalInvereseTransformation *
			globalTransformation * mBoneData[boneIdx].BoneOffset;
		mBoneData[boneIdx].FinalTransformation = finalTransformation;
	}

	for (unsigned int i = 0; i < node->children.size(); ++i)
	{
		UpdateHierarchy(time, animation, node->children[i], globalTransformation);
	}
}

void MeshData::UpdateTransforms(float time1, Animation* animation1, float time2,
	Animation* animation2, float factor, std::vector<glm::mat4>& transforms,
	std::vector<glm::mat4>& globals, std::vector<glm::mat4>& offsets)
{
	// Update the node hierarchy, starting from the root node
	// Update node hierarchy..
	glm::mat4 identity(1.f);
	UpdateHierarchy(time1, animation1, time2, animation2, factor, mRootNode, identity);

	// Acquire all the matrix information
	// Set all transforms, globals, and offsets
	unsigned int numBones = mBoneData.size();
	transforms.resize(numBones);
	globals.resize(numBones);
	offsets.resize(numBones);

	for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
	{
		transforms[boneIdx] = mBoneData[boneIdx].LocalTransformation;
		globals[boneIdx] = mBoneData[boneIdx].GlobalTransformation;
		offsets[boneIdx] = mBoneData[boneIdx].BoneOffset;
	}
}

void MeshData::UpdateHierarchy(float time1, Animation* animation1, float time2,
	Animation* animation2, float factor, NodeData* node, glm::mat4& parentMatrix)
{
	if (node == 0)
		return;

	float rFactor = 1.0f - factor;

	std::string nodeName = node->name;
	glm::mat4 transformation;

	AnimationNode* anim1Node = FindAnimationNode(animation1, nodeName);
	AnimationNode* anim2Node = FindAnimationNode(animation2, nodeName);

	transformation = node->transformation;
	if (anim1Node && anim2Node)
	{
		glm::vec3 scaling1;
		glm::vec3 scaling2;
		CalculateScaling(scaling1, time1, anim1Node);
		CalculateScaling(scaling2, time2, anim2Node);
		glm::vec3 scaling = scaling1 * rFactor + scaling2 * factor;
		glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaling);

		glm::quat rotation1;
		glm::quat rotation2;
		CalculateRotation(rotation1, time1, anim1Node);
		CalculateRotation(rotation2, time2, anim2Node);
		glm::quat rotation = glm::slerp(rotation1, rotation2, factor);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

		glm::vec3 translation1;
		glm::vec3 translation2;
		CalculateTranslation(translation1, time1, anim1Node);
		CalculateTranslation(translation2, time2, anim2Node);
		glm::vec3 translation = translation1 * rFactor + translation2 * factor;
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), translation);

		transformation = translationMatrix * rotationMatrix * scalingMatrix;
	}

	glm::mat4 globalTransformation = parentMatrix * transformation;
	glm::mat4 finalTransformation;
	std::map<std::string, unsigned int>::const_iterator itBoneMap = mBoneMap.find(nodeName);
	if (itBoneMap != mBoneMap.end())
	{
		unsigned int boneIdx = itBoneMap->second;
		mBoneData[boneIdx].GlobalTransformation = globalTransformation;
		mBoneData[boneIdx].LocalTransformation = transformation;
		finalTransformation = mGlobalInvereseTransformation *
			globalTransformation * mBoneData[boneIdx].BoneOffset;
		mBoneData[boneIdx].FinalTransformation = finalTransformation;
	}

	for (unsigned int i = 0; i < node->children.size(); ++i)
	{
		UpdateHierarchy(time1, animation1, time2, animation2, factor, node->children[i], globalTransformation);
	}
}

void MeshData::CalculateScaling(glm::vec3& scaling, float time, AnimationNode* node)
{
	if (node->keyFrameScales.size() == 1)
	{
		scaling = node->keyFrameScales[0].scale;
		return;
	}

	unsigned int scalingIndex = FindKeyFrameScaleIndex(node, time);
	unsigned int nextScalingIndex = scalingIndex + 1;
	float dt = node->keyFrameScales[nextScalingIndex].time - node->keyFrameScales[scalingIndex].time;
	float factor = (time - (float)node->keyFrameScales[scalingIndex].time) / dt;
	const glm::vec3 startScale = node->keyFrameScales[scalingIndex].scale;
	const glm::vec3 endScale = node->keyFrameScales[nextScalingIndex].scale;
	scaling = startScale + (endScale - startScale) * factor;
}

void MeshData::CalculateRotation(glm::quat& rotation, float time, AnimationNode* node)
{
	if (node->keyFrameRotations.size() == 1)
	{
		rotation = node->keyFrameRotations[0].rotation;
		return;
	}

	unsigned int rotationIndex = FindKeyFrameRotationIndex(node, time);
	unsigned int nextRotationIndex = rotationIndex + 1;
	float dt = node->keyFrameRotations[nextRotationIndex].time - node->keyFrameRotations[rotationIndex].time;
	float factor = (time - (float)node->keyFrameRotations[rotationIndex].time) / dt;
	const glm::quat startRotation = node->keyFrameRotations[rotationIndex].rotation;
	const glm::quat endRotation = node->keyFrameRotations[nextRotationIndex].rotation;
	rotation = glm::slerp(startRotation, endRotation, factor);
	glm::normalize(rotation);
}

void MeshData::CalculateTranslation(glm::vec3& translation, float time, AnimationNode* node)
{
	if (node->keyFramePositions.size() == 1)
	{
		translation = node->keyFramePositions[0].position;
		return;
	}

	unsigned int positionIndex = FindKeyFramePositionIndex(node, time);
	unsigned int nextPositionIndex = positionIndex + 1;
	float dt = node->keyFramePositions[nextPositionIndex].time - node->keyFramePositions[positionIndex].time;
	float factor = (time - (float)node->keyFramePositions[positionIndex].time) / dt;
	const glm::vec3 startPosition = node->keyFramePositions[positionIndex].position;
	const glm::vec3 endPosition = node->keyFramePositions[nextPositionIndex].position;
	translation = startPosition + (endPosition - startPosition) * factor;
}


int MeshData::FindKeyFramePositionIndex(AnimationNode* node, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < node->keyFramePositions.size(); ++keyFrameIndex)
	{
		if (node->keyFramePositions[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return node->keyFramePositions.size() - 2;
}

int MeshData::FindKeyFrameScaleIndex(AnimationNode* node, float time)
{
	for (int keyFrameIndex = 1, size = node->keyFrameScales.size(); keyFrameIndex < size; ++keyFrameIndex)
	{
		if (node->keyFrameScales[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return node->keyFrameScales.size() - 2;
}

int MeshData::FindKeyFrameRotationIndex(AnimationNode* node, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < node->keyFrameRotations.size(); ++keyFrameIndex)
	{
		if (node->keyFrameRotations[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return node->keyFrameRotations.size() - 2;
}