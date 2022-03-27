#pragma once
#include "Graphs/Graph.h"
#include "cGatherer.h"
#include "cResource.h"
#include <vector>

class cMapManager {
public:
	void Init();
	void CopyGraph(Graph* newGraph);
	void SetTerrainOnLocalGraphs(char id, Terrain newTerrain);
	Graph* currentGraph;
	std::vector<cGatherer> gatherers;
	std::vector<cResource> resources;
};