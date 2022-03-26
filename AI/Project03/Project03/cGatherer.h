#pragma once
#include "GathererStates.h"
#include "cMesh.h"
#include "Graphs/sNode.h"
#include <extern/glm/vec3.hpp>
#include <list>

class cGatherer {
public:
	cGatherer(glm::vec3 _position, Node* node);
	void Update(float deltaTime);
private:
	void UpdateState();
	Node* Dijkstra(Terrain target);
	Node* GetLowestCostNode(std::list<Node*> openList);
	bool IsNodeInOpenList(std::list<Node*> openList, Node* neighbour);
	GathererStates currentState;
	cMesh* mesh;
	Node* currentNode;
	Node* targetNode;
	glm::vec3 position;
	bool isWaiting;
	float waitMaxTime = 4.f;
	float waitCounter = 0.f;
};