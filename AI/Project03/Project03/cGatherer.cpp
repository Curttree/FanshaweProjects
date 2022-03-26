#include "cGatherer.h"
#include "globals.h"
#include <iostream>

cGatherer::cGatherer(glm::vec3 _position, Node* node) : position(_position), currentNode(node) {
	currentState = GathererStates::Idle; 
	mesh = new cMesh;
	mesh->meshName = "animal.ply";
	mesh->textureNames[0] = "green.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Gatherer";
	mesh->positionXYZ = position;
	mesh->scale = 5.0f;

	g_vec_pMeshes.push_back(mesh);
}

void cGatherer::Update(float deltaTime) {
	if (isWaiting) {
		waitCounter += deltaTime;
	}
	UpdateState();
}

void cGatherer::UpdateState() {
	switch (currentState) {
	case GathererStates::Idle: {
		//Initial State, transition to Search.
		//TODO: Remove wait counter once we are actually searching. For now, fake it.
		Node* target = Dijkstra(Terrain::Resource);
		if (target == NULL) {
			//All resources have been collected.
			currentState = GathererStates::Done;
		}
		else {
			currentState = GathererStates::Search;
			std::cout << "State change to search" << std::endl;
			targetNode = target;
			waitMaxTime = 5.f;
			isWaiting = true;
		}
		break;
	}
	case GathererStates::Search: {
		//TODO: Use A* to find a path to the resource. For now, assume path has been found and use timer to simulate walking to it.
		if (isWaiting && waitCounter >= waitMaxTime) {
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
					waitCounter = 0.f;
					waitMaxTime = 5.f;
					isWaiting = true;
				}
			}
			else {
				isWaiting = false;
				currentState = GathererStates::Gather;
				targetNode->terrain = Terrain::Blank;	// Resource hasn't technically been cleared yet, but set terrain to blank so the next gatherer to arrive knows we are already gathering.
				targetNode = NULL;
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
			//TODO: Actually use A* to return. For now, fake it.
			std::cout << "State change to return" << std::endl;
			waitMaxTime = 5.f;
			isWaiting = true;
		}
		break;
	}
	case GathererStates::Return: {
		//TODO: Use A* to return. For now, use timer to simulate. Remember to account for 2 seconds wait time after returning resource.
		if (isWaiting && waitCounter >= waitMaxTime) {
			isWaiting = false;
			//TODO: Check to see if there is another resource. For now, assume there is.
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
				waitCounter = 0.f;
				waitMaxTime = 5.f;
				isWaiting = true;
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
	mapManager.currentGraph->ResetGraph();

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
	for (auto iter = openList.begin(); iter != openList.end(); ++iter)
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
	for (auto iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if (*iter == neighbour)
			return true;
	}
	return false;
}
