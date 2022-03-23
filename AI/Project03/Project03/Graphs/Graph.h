#pragma once
#include "sNode.h"

class Graph
{
public:
	std::vector<Node*> nodes;  //all of the nodes inside my graph
	Graph();
	void CreateNode(char id, Vertex position, bool bHasGoal = false);
	void AddEdge(Node* parent, Node* child, float weight, bool bUndirected = true);
	void PrintGraph();
	void ResetGraph();
	void PrintParents(bool includeCost = false);
};
