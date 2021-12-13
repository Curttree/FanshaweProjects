#include "cDungeonMaster.h"
#include "cDungeonFactory.h"

cDungeonFactory* factory = cDungeonFactory::Instance();

cDungeonMaster::cDungeonMaster() {
	dungeonReader = new cDungeonReader();

}

cDungeonMaster::~cDungeonMaster() {
	if (dungeonReader) {
		delete dungeonReader;
		dungeonReader = NULL;
	}
}

void cDungeonMaster::BuildDungeonFromTSV(std::string fileName) {
	if (!dungeonReader) {
		std::cout << "Dungeon reader could not be found. Ensure the reader has been properly setup" << std::endl;
		return;
	}

	if (!dungeonReader->ReadDungeonFromTSV(fileName)) {
		return;
	}

	int x_size, z_size;
	dungeonReader->GetDungeonSize(x_size, z_size);

	std::cout << x_size << "," << z_size << std::endl;

	int piece; 
	glm::vec4 wallsToPlace;
	for (int x = 0; x < x_size; x++) {
		for (int z = 0; z < z_size; z++) {
			piece = dungeonReader->GetObjectAtLocation(x, z);
			if (piece == 1) {
				//Floor. Need to place adjacent walls.
				wallsToPlace = dungeonReader->DetermineWallOrientation(x, z);
				PlaceWalls(glm::vec3(x * 500.f - 250.f, 0, z * 500.f - 250.f), wallsToPlace);
			}
			cMesh* model = factory->createDungeonPiece(piece, glm::vec3(x*500.f, 0.f, z*500.f));
			if (piece == 3 || piece == 4) {
				//model->positionXYZ += glm::vec3(-250.f, 0.f, -250.f);
				SetupDoors(model, x, z);
			}
			if (model) {
				dungeonModels.push_back(model);
			}
		}
	}

}

void cDungeonMaster::PlaceWalls(glm::vec3 position, glm::vec4 wallsToPlace) {
	if (wallsToPlace.x > 0.f) {
		cMesh* model = factory->createDungeonPiece(2, glm::vec3(position.x, 0.f, position.z));
		if (model) {
			model->orientationXYZ = (glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
			model->positionXYZ += glm::vec3(-250.f, 0.f, 250.f);
			dungeonModels.push_back(model);
		}
	}
	if (wallsToPlace.y > 0.f) {
		cMesh* model = factory->createDungeonPiece(2, glm::vec3(position.x, 0.f, position.z));
		if (model) {
			model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
			model->positionXYZ += glm::vec3(250.f, 0.f, -250.f);
			dungeonModels.push_back(model);
		}
	}
	if (wallsToPlace.z > 0.f) {
		cMesh* model = factory->createDungeonPiece(2, glm::vec3(position.x, 0.f, position.z));
		if (model) {
			model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>(), 0.f));
			model->positionXYZ += glm::vec3(-250.f, 0.f, -250.f);
			dungeonModels.push_back(model);
		}
	}
	if (wallsToPlace.w > 0.f) {
		cMesh* model = factory->createDungeonPiece(2, glm::vec3(position.x, 0.f, position.z));
		if (model) {
			model->orientationXYZ = (glm::vec3(0.f, 0.f, 0.f));
			model->positionXYZ += glm::vec3(250.f, 0.f, 250.f);
			dungeonModels.push_back(model);
		}
	}
}

void cDungeonMaster::SetupDoors(cMesh* door, int x, int z) {
	// ASSUMPTION: No doors on outside. If practical case, we may need to null check here.
	// Also fills in area around the door.
	cMesh* actualDoor;
	int orientation = dungeonReader->DetermineDoorOrientation(x, z);
	if (door->friendlyName == "PortcullisFrame") {
		actualDoor = factory->createDungeonPiece(6, glm::vec3(x * 500.f, 0.f, z * 500.f));
	}
	else {
		actualDoor = factory->createDungeonPiece(5, glm::vec3(x * 500.f, 0.f, z * 500.f));
	}
	switch (orientation) {
	case 1:{
		door->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
		door->positionXYZ += (glm::vec3(-250.f, 0.f, 0.f));
		actualDoor->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
		if (door->friendlyName == "PortcullisFrame") {
			actualDoor->positionXYZ += (glm::vec3(-250.f, 0.f, 0.f));
		}
		else {
			actualDoor->positionXYZ += (glm::vec3(-250.f, 0.f, -160.f));
		}
		cMesh* wall1 = factory->createDungeonPiece(2, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
		wall1->orientationXYZ = (glm::vec3(0.f, glm::pi<float>(), 0.f));
		wall1->positionXYZ += glm::vec3(-250.f, 0.f, -250.f);
		dungeonModels.push_back(wall1);
		cMesh* wall2 = factory->createDungeonPiece(2, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
		wall2->orientationXYZ = (glm::vec3(0.f,0.f, 0.f));
		wall2->positionXYZ += glm::vec3(250.f, 0.f, 250.f);
		dungeonModels.push_back(wall2);
		cMesh* floor = factory->createDungeonPiece(1, glm::vec3(x * 500.f, 0.f, z * 500.f));
		dungeonModels.push_back(floor);
		break; }
	case 2: {
		door->positionXYZ += (glm::vec3(-500.f, 0.f, -250.f));

		if (door->friendlyName == "PortcullisFrame") {
			actualDoor->positionXYZ += (glm::vec3(-500.f, 0.f, -250.f));
		}
		else {
			actualDoor->positionXYZ += (glm::vec3(-340.f, 0.f, -250.f));
		}

		cMesh* wall1 = factory->createDungeonPiece(2, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
		wall1->orientationXYZ = (glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
		wall1->positionXYZ += glm::vec3(-250.f, 0.f, 250.f);
		dungeonModels.push_back(wall1);
		cMesh* wall2 = factory->createDungeonPiece(2, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
		wall2->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
		wall2->positionXYZ += glm::vec3(250.f, 0.f, -250.f);
		dungeonModels.push_back(wall2);
		cMesh* floor = factory->createDungeonPiece(1, glm::vec3(x * 500.f, 0.f, z * 500.f));
		dungeonModels.push_back(floor);
		break;
	}
	default:
		//Invalid placement. Leave as is.
		break;
	}

	dungeonModels.push_back(actualDoor);
}