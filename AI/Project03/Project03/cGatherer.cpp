#include "cGatherer.h"
#include "globals.h"
#include <iostream>

cGatherer::cGatherer(glm::vec3 _position) : position(_position){
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
		currentState = GathererStates::Search;
		//TODO: Remove wait counter once we are actually searching. For now, fake it.
		std::cout << "State change to search" << std::endl;
		waitMaxTime = 5.f;
		isWaiting = true;
		break;
	}
	case GathererStates::Search: {
		//TODO: Use Dijktstra's algorithm to find a path to the resource. For now, assume path has been found and use timer to simulate walking to it.
		if (isWaiting && waitCounter >= waitMaxTime) {
			isWaiting = false;
			currentState = GathererStates::Gather;
			std::cout << "State change to gather" << std::endl;
			isWaiting = true;
			waitCounter = 0.f;
			waitMaxTime = 4.f;
		}
		break;
	}
	case GathererStates::Gather: {
		if (isWaiting && waitCounter >= waitMaxTime) {
			//TODO: Check to see if resource is still there. Otherwise, revert to search with next nearest resource.
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
			std::cout << "State change to search" << std::endl;
			currentState = GathererStates::Search;
			waitCounter = 0.f;
			waitMaxTime = 5.f;
			isWaiting = true;
		}
		break;
	}
	default:
		//Should not enter this state unless a new state is added that we have not coded behavior for, or they are done with their task.
		break;
	}
}