#include "Graph.h"
#include <iostream>

Graph::Graph() {}

void Graph::CreateNode(char id, Vertex position, bool bHasGoal)
{
	Node* node = new Node;
	node->id = id;
	node->visited = false;
	node->costSoFar = FLT_MAX;
	node->hDistance = 0;
	node->position = position;
	node->parent = NULL;
	node->hasGoal = bHasGoal;

	this->nodes.push_back(node);
}

void Graph::AddEdge(Node* parent, Node* child, float weight, bool bUndirected)
{
	std::pair<Node*, float> edge;
	edge.first = child;
	edge.second = weight;
	parent->edges.push_back(edge);

	if (bUndirected)
	{
		std::pair<Node*, float> reverseEdge;
		reverseEdge.first = parent;
		reverseEdge.second = weight;
		child->edges.push_back(reverseEdge);
	}
}

void Graph::PrintGraph()
{
	for (Node*& currNode : this->nodes)
	{
		std::cout << "Node " << currNode->id << " -> ";
		for (std::pair<Node*, float>& edge : currNode->edges)
		{
			std::cout << " (" << edge.first->id << ", " << edge.second << ") ";
		}
		std::cout << std::endl;
	}
}

void Graph::ResetGraph()
{
	for (Node*& currNode : this->nodes)
	{
		currNode->visited = false;
		currNode->parent = NULL;
		currNode->costSoFar = FLT_MAX;
	}
}

void Graph::PrintParents(bool includeCost)
{
	for (Node*& currNode : this->nodes)
	{
		std::cout << "Node: " << currNode->id << " -> ";
		if (currNode->parent != NULL)
		{
			std::cout << currNode->parent->id;
		}
		else
		{
			std::cout << "NULL";
		}
		if (includeCost)
		{
			std::cout << " cost so far: " << currNode->costSoFar << " hDist= " << currNode->hDistance << " f cost = " << currNode->costSoFar + currNode->hDistance;
		}
		std::cout << std::endl;
	}
}