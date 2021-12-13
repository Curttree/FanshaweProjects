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

	//Place decorations
	PlaceCrystals();
	PlacePlants();
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

void cDungeonMaster::PlaceCrystals() {
	cMesh* crystal1 = factory->createDungeonPiece(7, glm::vec3(3000.f, 0.f, 1500.f));
	cMesh* crystal2 = factory->createDungeonPiece(8, glm::vec3(3840.f, 0.f, 2300.f));
	cMesh* crystal3 = factory->createDungeonPiece(9, glm::vec3(3150.f, 0.f, 2050.f));
	cMesh* crystal4 = factory->createDungeonPiece(10, glm::vec3(2560.f, 0.f, 550.f));
	cMesh* crystal5 = factory->createDungeonPiece(7, glm::vec3(4170.f, 0.f, 1050.f));

	dungeonModels.push_back(crystal1);
	dungeonModels.push_back(crystal2);
	dungeonModels.push_back(crystal3);
	dungeonModels.push_back(crystal4);
	dungeonModels.push_back(crystal5);
}

void cDungeonMaster::PlacePlants() {

	//Place trees to act as center of our cluster
	cMesh* tree1 = factory->createDungeonPiece(11, glm::vec3(8250.f, 0.f, 2500.f));
	cMesh* tree2 = factory->createDungeonPiece(12, glm::vec3(14250.f, 0.f, 5000.f));
	cMesh* tree3 = factory->createDungeonPiece(13, glm::vec3(4750.f, 0.f, 8500.f));

	//Place plants around the trees
	cMesh* plant1 = factory->createDungeonPiece(14, glm::vec3(8350.f, 0.f, 2660.f));
	cMesh* plant2 = factory->createDungeonPiece(15, glm::vec3(8150.f, 0.f, 2700.f));
	cMesh* plant3 = factory->createDungeonPiece(17, glm::vec3(8000.f, 0.f, 2400.f));

	cMesh* plant4 = factory->createDungeonPiece(18, glm::vec3(14150.f, 0.f, 5200.f));
	cMesh* plant5 = factory->createDungeonPiece(15, glm::vec3(14100.f, 0.f, 4900.f));
	cMesh* plant6 = factory->createDungeonPiece(14, glm::vec3(14350.f, 0.f, 5100.f));

	cMesh* plant7 = factory->createDungeonPiece(16, glm::vec3(4650.f, 0.f, 8380.f));
	cMesh* plant8 = factory->createDungeonPiece(18, glm::vec3(4550.f, 0.f, 8720.f));
	cMesh* plant9 = factory->createDungeonPiece(19, glm::vec3(4850.f, 0.f, 8600.f));


	dungeonModels.push_back(tree1);
	dungeonModels.push_back(tree2);
	dungeonModels.push_back(tree3);

	dungeonModels.push_back(plant1);
	dungeonModels.push_back(plant2);
	dungeonModels.push_back(plant3);
	dungeonModels.push_back(plant4);
	dungeonModels.push_back(plant5);
	dungeonModels.push_back(plant6);
	dungeonModels.push_back(plant7);
	dungeonModels.push_back(plant8);
	dungeonModels.push_back(plant9);
}