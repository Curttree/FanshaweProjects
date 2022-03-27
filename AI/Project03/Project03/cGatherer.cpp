#include "cGatherer.h"
#include "globals.h"
#include <iostream>

cGatherer::cGatherer(glm::vec3 _position, Node* node) : position(_position) {
	currentState = GathererStates::Idle; 
	mesh = new cMesh;
	mesh->meshName = "animal.ply";
	mesh->textureNames[0] = "green.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Gatherer";
	mesh->positionXYZ = position;
	mesh->scale = 5.0f;

	g_vec_pMeshes.push_back(mesh);
	// Make a deep copy of the current graph.
	localGraph = new Graph();
	::mapManager.CopyGraph(localGraph);
	for (unsigned int x = 0; x < localGraph->nodes.size(); x++) {
		if (node->id == localGraph->nodes[x]->id) {
			currentNode = localGraph->nodes[x];
		}
	}
}

cGatherer::~cGatherer() {
	//if (localGraph) {
	//	delete localGraph;
	//	localGraph = NULL;
	//}
}

glm::vec3 cGatherer::GetPosition() {
	return position;
}

void cGatherer::Update(float deltaTime) {
	if (isWaiting) {
		waitCounter += deltaTime;
	}
	UpdateState();
	if (!isWaiting && (currentState == GathererStates::Search || currentState == GathererStates::Return) && targetNode != NULL) {
		//TODO: Check position against next node before calling GetNextNode
		if (glm::distance(position, targetPosition) < 0.05f) {
			currentNode = nextNode;
			nextNode = GetNextNode();
			RecalculateVelocity();
		}
		else {
			position += velocity;
			mesh->positionXYZ = position;
		}
	}
}

void cGatherer::UpdateLocalTerrain(unsigned int index, Terrain newTerrain) {
	localGraph->nodes[index]->terrain = newTerrain;
}

void cGatherer::UpdateState() {
	switch (currentState) {
	case GathererStates::Idle: {
		//Initial State, transition to Search.
		Node* target = Dijkstra(Terrain::Resource);
		if (target == NULL) {
			//All resources have been collected.
			currentState = GathererStates::Done;
		}
		else {
			currentState = GathererStates::Search;
			std::cout << "State change to search" << std::endl;
			targetNode = target;
			// Find the path to get to the resource.
			BuildPath(Terrain::Resource);
			RecalculateVelocity();
		}
		break;
	}
	case GathererStates::Search: {
		if (currentNode == targetNode) {
			// Once we have arrived, check to see if the resource is still here.
			if (targetNode->terrain != Terrain::Resource) {
				std::cout << "We have been beaten to the resource." << std::endl;
				Node* target = Dijkstra(Terrain::Resource);
				if (target == NULL) {
					//All resources have been collected.
					std::cout << "All resources have been collected" << std::endl;
					currentState = GathererStates::Done;
				}
				else {
					currentState = GathererStates::Search;
					std::cout << "Searching for another resource." << std::endl;
					targetNode = target;
					BuildPath(Terrain::Resource);
					RecalculateVelocity();
				}
			}
			else {
				currentState = GathererStates::Gather;
				::mapManager.SetTerrainOnLocalGraphs(targetNode->id, Terrain::Blank);	// Resource hasn't technically been cleared yet, but set terrain to blank so the next gatherer to arrive knows we are already gathering.
				std::cout << "State change to gather" << std::endl;
				isWaiting = true;
				waitCounter = 0.f;
				waitMaxTime = 4.f;
			}
		}
		break;
	}
	case GathererStates::Gather: {
		if (isWaiting && waitCounter >= waitMaxTime) {
			isWaiting = false;
			currentState = GathererStates::Return;
			waitCounter = 0.f;
			for (unsigned int index = 0; index < ::mapManager.resources.size(); index++) {
				if (glm::distance(position, ::mapManager.resources[index].GetPosition()) < 0.25f) {
					::mapManager.resources[index].Collect(this);
				}
			}
			Node* target = Dijkstra(Terrain::Base);
			if (target == NULL) {
				//No base. Stop collecting.
				std::cout << "No base is present. Stop gathering." << std::endl;
				currentState = GathererStates::Done;
			}
			else {
				targetNode = target;
			}
			BuildPath(Terrain::Base);
			RecalculateVelocity();
		}
		break;
	}
	case GathererStates::Return: {
		if (!isWaiting && currentNode == targetNode) {
			isWaiting = true;
			waitCounter = 0.f;
			waitMaxTime = 2.f;
		}
		if (isWaiting && waitCounter >= waitMaxTime) {
			isWaiting = false; 
			for (unsigned int index = 0; index < ::mapManager.resources.size(); index++) {
				if (::mapManager.resources[index].collectedBy == this) {
					::mapManager.resources[index].Deliver();
				}
			}
			Node* target = Dijkstra(Terrain::Resource);
			if (target == NULL) {
				//All resources have been collected.
				std::cout << "All resources have been collected" << std::endl;
				currentState = GathererStates::Done;
			}
			else {
				std::cout << "State change to search" << std::endl;
				targetNode = target;
				currentState = GathererStates::Search; 
				BuildPath(Terrain::Resource);
				RecalculateVelocity();
			}
		}
		break;
	}
	default:
		//Should not enter this state unless a new state is added that we have not coded behavior for, or they are done with their task.
		break;
	}
}

Node* cGatherer::Dijkstra(Terrain target)
{
	localGraph->ResetGraph();

	currentNode->costSoFar = 0;
	std::list<Node*> openList;
	std::list<Node*> closeList;
	openList.push_back(currentNode);

	while (!openList.empty())
	{
		Node* currNode = GetLowestCostNode(openList);
		openList.remove(currNode);
		closeList.push_back(currNode);

		currNode->visited = true;
		if (currNode->terrain == target)
		{
			return currNode;
		}

		//Go through every neighbouring node of the current node
		for (std::pair<Node*, float> neighbour : currNode->edges)
		{
			if (neighbour.first->visited == false)
			{
				float weightSoFar = currNode->costSoFar + neighbour.second;
				if (weightSoFar < neighbour.first->costSoFar)
				{
					neighbour.first->costSoFar = weightSoFar;
					neighbour.first->parent = currNode;
					if (!IsNodeInOpenList(openList, neighbour.first))
					{
						openList.push_back(neighbour.first);
					}
				}
			}
		}
	}
	return NULL;
}

Node* cGatherer::GetLowestCostNode(std::list<Node*> openList)
{
	float minCost = FLT_MAX;
	Node* lowestCostNode = NULL;
	//find the node with the lowest cost from the root node
	for (std::list<Node*>::iterator iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter)->costSoFar < minCost)
		{
			minCost = (*iter)->costSoFar;
			lowestCostNode = *iter;
		}
	}
	return lowestCostNode;
}

bool cGatherer::IsNodeInOpenList(std::list<Node*> openList, Node* neighbour)
{
	for (std::list<Node*>::iterator iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if (*iter == neighbour)
			return true;
	}
	return false;
}

float cGatherer::CalculateHeuristicDistances(Node* node, Node* goal)
{
	float D = 1;
	float dx = abs(node->position.x - goal->position.x);
	float dy = abs(node->position.y - goal->position.y);
	return D * (dx + dy);
}

Node* cGatherer::GetLowestFCostNode(std::list<Node*> openList)
{
	float minCost = FLT_MAX;
	Node* lowestCostNode = NULL;
	//find the node with the lowest f cost
	for (std::list<Node*>::iterator iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter)->costSoFar + (*iter)->hDistance < minCost)
		{
			minCost = (*iter)->costSoFar + (*iter)->hDistance;
			lowestCostNode = *iter;
		}
	}
	return lowestCostNode;
}

Node* cGatherer::AStar(Terrain target)
{
	currentNode->costSoFar = 0;
	currentNode->hDistance = CalculateHeuristicDistances(currentNode, targetNode);

	std::list<Node*> openList;
	std::list<Node*> closeList;
	openList.push_back(currentNode);

	while (!openList.empty())
	{
		//remove the current node from the open list with the lowest f cost
		Node* currNode = GetLowestFCostNode(openList);
		openList.remove(currNode);
		closeList.push_back(currNode);

		currNode->visited = true;
		if (currNode->terrain == target)
		{
			//Could possibly find another resource along the way if they are moving, 
			return currNode;
		}

		//Go through each neighbouring node of current node
		for (std::pair <Node*, float> neighbour : currNode->edges)
		{
			if (neighbour.first->visited == false)
			{
				float weightSoFar = currNode->costSoFar + neighbour.second;
				if (weightSoFar < neighbour.first->costSoFar)
				{
					neighbour.first->costSoFar = weightSoFar;
					neighbour.first->parent = currNode;
					if (!IsNodeInOpenList(openList, neighbour.first))
					{
						neighbour.first->hDistance = CalculateHeuristicDistances(neighbour.first, targetNode);
						openList.push_back(neighbour.first);
					}
				}
			}
		}
	}
	return NULL;
}

Node* cGatherer::GetNextNode() {
	nodesTraversed++;
	if (nodesTraversed >= pathLength) {
		return NULL;
	}
	Node* result = targetNode;
	for (int x = 1; x < pathLength - nodesTraversed; x++) {
		result = result->parent;

		if (result == NULL) {
			std::cout << "Math is off." << std::endl;
			return NULL;
		}
	}
	targetPosition = glm::vec3(result->position.x, 0.f, result->position.y);
	return result;
}

void cGatherer::BuildPath(Terrain t) {
	AStar(t);
	Node* checkingNode = targetNode;
	pathLength = 0;
	nodesTraversed = 0;
	do
	{
		pathLength++;
		checkingNode = checkingNode->parent;
	} while (checkingNode->parent != NULL);
	nextNode = GetNextNode();
}

void cGatherer::RecalculateVelocity() {
	velocity = glm::normalize(targetPosition - position) * 0.1f;
}