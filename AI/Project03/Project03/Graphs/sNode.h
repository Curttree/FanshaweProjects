#pragma once
#include <vector>
#include "Vertex.h"

struct Node
{
	char id;
	bool visited;
	bool hasGoal;
	float costSoFar;
	float hDistance;
	Vertex position; //position in our game world. Used for calculating distances
	struct Node* parent; //parent Node that we can use to follow back to the root node
	std::vector<std::pair<Node*, float>> edges; //Edges pointing to our neighbouring nodes <childNode, edgeWeight>
};