#include "cMapManager.h"
#include "BMPLoader/BMPImage.h"
#include "BMPLoader/ResourceManager.h"
#include "cMesh.h"
#include "globals.h"

ResourceManager gResourceManager;

char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0)		return 'r';
	if (r == 0 && g == 255 && b == 0)		return 'g';
	if (r == 0 && g == 0 && b == 255)	return 'b';
	if (r == 255 && g == 255 && b == 0)		return 'y';
	if (r == 255 && g == 255 && b == 255)	return 'w';
	if (r == 0 && g == 0 && b == 0)		return '_';
	return 'x';
}

Terrain GetTerrain(char colourCode) {
	if (colourCode == 'r') return Terrain::Resource;
	if (colourCode == 'g') return Terrain::Start;
	if (colourCode == 'b') return Terrain::Base;
	if (colourCode == 'y') return Terrain::Difficult;
	if (colourCode == 'w') return Terrain::Blank;
	if (colourCode == '_') return Terrain::Wall;
	return Terrain::Unknown;
}

std::string GetTexture(Terrain terrain) {
	if (terrain == Terrain::Resource) return "white.bmp";
	if (terrain == Terrain::Start) return "white.bmp";
	if (terrain == Terrain::Base) return "blue.bmp";
	if (terrain == Terrain::Difficult) return "yellow.bmp";
	if (terrain == Terrain::Blank) return "white.bmp";
	if (terrain == Terrain::Wall) return "black.bmp";
	return "";
}

void cMapManager::Init() {
	currentGraph = new Graph();
	BMPImage* bmp = new BMPImage("resourceMap.bmp");

	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();

	int index = 0;
	for (unsigned long x = 0; x < imageWidth; x++) {
		for (unsigned long y = 0; y < imageHeight; y++) {
			char value = GetColourCharacter(data[index++], data[index++], data[index++]);
			Terrain ter = GetTerrain(value);
			printf("%c", value);
			currentGraph->CreateNode(index/3, Vertex(x, y, 0), ter);
			cMesh* tile = new cMesh;
			tile->meshName = "tile.ply";
			tile->textureNames[0] = GetTexture(ter);
			tile->textureRatios[0] = 1.f;
			tile->friendlyName = "Tile_" + std::to_string(x) + "_" + std::to_string(y);
			tile->positionXYZ = glm::vec3(x * 1.0f + 0.5f, -0.2f, y * 1.0f + 0.5f);
			tile->scale = 1.0f;

			g_vec_pMeshes.push_back(tile);
		}
		printf("\n");
	}

	//We have our nodes, now loop through and add edges and weights.
	//Edges have to be directed since weights are different.
	//TODO: Refactor so code is less repetitive.
	Node* current;
	for (unsigned int index = 0; index < currentGraph->nodes.size(); index++) {
		current = currentGraph->nodes[index];
		if (current->terrain != Terrain::Wall && current->terrain != Terrain::Unknown) {
			if (current->position.x > 0) {
				//Look to the left.
				if (currentGraph->nodes[index - 1]->terrain != Terrain::Wall && currentGraph->nodes[index - 1]->terrain != Terrain::Unknown) {
					float weight = 10.f;
					if (currentGraph->nodes[index - 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index - 1], weight, false);
				}
			}			
			if (current->position.y > 0) {
				//Look to the bottom.
				if (currentGraph->nodes[index - imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index - imageWidth]->terrain != Terrain::Unknown) {
					float weight = 10.f;
					if (currentGraph->nodes[index - imageWidth]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index - imageWidth], weight, false);
				}
			}		
			if (current->position.x < imageWidth -1) {
				//Look to the right.
				if (currentGraph->nodes[index + 1]->terrain != Terrain::Wall && currentGraph->nodes[index + 1]->terrain != Terrain::Unknown) {
					float weight = 10.f;
					if (currentGraph->nodes[index + 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index + 1], weight, false);
				}
			}
			if (current->position.y < imageHeight - 1) {
				//Look to the top.
				if (currentGraph->nodes[index + imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index + imageWidth]->terrain != Terrain::Unknown) {
					float weight = 10.f;
					if (currentGraph->nodes[index + imageWidth]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index + imageWidth], weight, false);
				}
			}

			//Handle diagonals.
			if (current->position.x > 0 && current->position.y > 0) {
				//Look to the bottom left.
				if (currentGraph->nodes[index - 1]->terrain != Terrain::Wall && currentGraph->nodes[index - 1]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index - imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index - imageWidth]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index - imageWidth - 1]->terrain != Terrain::Wall && currentGraph->nodes[index - imageWidth - 1]->terrain != Terrain::Unknown) {
					float weight = 14.f;
					if (currentGraph->nodes[index - imageWidth - 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index - imageWidth - 1], weight, false);
				}
			}
			if (current->position.x < imageWidth - 1 && current->position.y > 0) {
				//Look to the bottom right.
				if (currentGraph->nodes[index + 1]->terrain != Terrain::Wall && currentGraph->nodes[index + 1]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index - imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index - imageWidth]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index - imageWidth + 1]->terrain != Terrain::Wall && currentGraph->nodes[index - imageWidth + 1]->terrain != Terrain::Unknown) {
					float weight = 14.f;
					if (currentGraph->nodes[index - imageWidth + 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index - imageWidth + 1], weight, false);
				}
			}
			if (current->position.x > 0 && current->position.y < imageHeight - 1) {
				//Look to the top left.
				if (currentGraph->nodes[index - 1]->terrain != Terrain::Wall && currentGraph->nodes[index - 1]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index + imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index + imageWidth]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index + imageWidth - 1]->terrain != Terrain::Wall && currentGraph->nodes[index + imageWidth - 1]->terrain != Terrain::Unknown) {
					float weight = 14.f;
					if (currentGraph->nodes[index + imageWidth - 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index + imageWidth - 1], weight, false);
				}
			}
			if (current->position.x < imageWidth - 1 && current->position.y < imageHeight - 1) {
				//Look to the top right.
				if (currentGraph->nodes[index + 1]->terrain != Terrain::Wall && currentGraph->nodes[index + 1]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index + imageWidth]->terrain != Terrain::Wall && currentGraph->nodes[index + imageWidth]->terrain != Terrain::Unknown &&
					currentGraph->nodes[index + imageWidth + 1]->terrain != Terrain::Wall && currentGraph->nodes[index + imageWidth + 1]->terrain != Terrain::Unknown) {
					float weight = 14.f;
					if (currentGraph->nodes[index + imageWidth + 1]->terrain == Terrain::Difficult) {
						weight *= 2.f;
					}
					currentGraph->AddEdge(current, currentGraph->nodes[index + imageWidth + 1], weight, false);
				}
			}
		}
	}
}