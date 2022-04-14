#pragma once
#include "GathererStates.h"
#include "cMesh.h"
#include "Graphs/Graph.h"
#include <extern/glm/vec3.hpp>
#include <list>

class cGatherer {
public:
	cGatherer(glm::vec3 _position, Node* node);
	virtual ~cGatherer();
	void Update(float deltaTime);
	glm::vec3 GetPosition();
	void UpdateLocalTerrain(unsigned int index, Terrain newTerrain);
private:
	void UpdateState();
	Node* Dijkstra(Terrain target);
	Node* GetLowestCostNode(std::list<Node*> openList);
	bool IsNodeInOpenList(std::list<Node*> openList, Node* neighbour);
	Node* AStar(Terrain target);
	float CalculateHeuristicDistances(Node* node, Node* goal);
	Node* GetLowestFCostNode(std::list<Node*> openList);
	Node* GetNextNode();
	void BuildPath(Terrain t);
	void RecalculateVelocity();
	GathererStates currentState;
	cMesh* mesh;
	Node* currentNode;
	Node* nextNode;
	Node* targetNode;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 targetPosition;
	bool isWaiting;
	float waitMaxTime = 4.f;
	float waitCounter = 0.f;
	int pathLength = 0;
	int nodesTraversed = 0;
	Graph* localGraph;
};