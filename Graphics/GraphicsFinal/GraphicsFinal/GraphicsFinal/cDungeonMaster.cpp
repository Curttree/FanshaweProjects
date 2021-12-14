#include "cDungeonMaster.h"
#include "cDungeonFactory.h"
#include "globals.h"

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
	glm::vec4 downstairCoords = glm::vec4(-1.f);
	glm::vec4 upstairCoords = glm::vec4(-1.f);
	for (int x = 0; x < x_size; x++) {
		for (int z = 0; z < z_size; z++) {
			piece = dungeonReader->GetObjectAtLocation(x, z);
			if (piece == 1) {
				//Floor. Need to place adjacent walls.
				wallsToPlace = dungeonReader->DetermineWallOrientation(x, z);
				PlaceWalls(glm::vec3(x * 500.f - 250.f, 0, z * 500.f - 250.f), wallsToPlace);
			}
			if (!(piece >= 5 && piece <= 8)) {
				cMesh* model = factory->createDungeonPiece(piece, glm::vec3(x * 500.f, 0.f, z * 500.f));

				if (piece == 3 || piece == 4) {
					//model->positionXYZ += glm::vec3(-250.f, 0.f, -250.f);
					SetupDoors(model, x, z);
				}

				if (model) {
					dungeonModels.push_back(model);
				}
			}
			else {
				if (piece == 5) {
					downstairCoords.x = x;
					downstairCoords.y = z;
				}
				if (piece == 6) {
					downstairCoords.z = x;
					downstairCoords.w = z;
				}
				if (piece == 7) {
					upstairCoords.x = x;
					upstairCoords.y = z;
				}
				if (piece == 8) {
					upstairCoords.z = x;
					upstairCoords.w = z;
				}
				if (downstairCoords.x >= 0 &&
					downstairCoords.y >= 0 &&
					downstairCoords.z >= 0 &&
					downstairCoords.w >= 0) {
					// We have all four downstairs coordinates. Place the stairs.
					cMesh* model = factory->createDungeonPiece(21, glm::vec3(downstairCoords.x * 500.f, 0.f, downstairCoords.y * 500.f));
					if (downstairCoords.x > downstairCoords.z) {
						model->orientationXYZ = (glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
						model->positionXYZ += (glm::vec3(-500.f, -250.F, -500.f));
					}
					if (downstairCoords.x < downstairCoords.z) {
						model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
					}
					if (model) {
						dungeonModels.push_back(model);
					}
				}
				if (upstairCoords.x >= 0 &&
					upstairCoords.y >= 0 &&
					upstairCoords.z >= 0 &&
					upstairCoords.w >= 0) {
					// We have all four downstairs coordinates. Place the stairs.
					cMesh* model = factory->createDungeonPiece(21, glm::vec3(upstairCoords.x * 500.f, 0.f, upstairCoords.y * 500.f));
					if (upstairCoords.x > upstairCoords.z) {
						model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
						//model->positionXYZ += (glm::vec3(-500.f, -250.F, -500.f));
					}
					if (upstairCoords.x < upstairCoords.z) {
						model->orientationXYZ = (glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
					}
					if (model) {
						dungeonModels.push_back(model);
					}
				}
			}
		}
	}

	//Place decorations
	PlaceCrystals();
	PlacePlants();
	PlaceTorches(x_size, z_size);
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


	::g_pTheLights->theLights[1].position = glm::vec4(glm::vec3(3000.f, 250.f, 1500.f), 1.f);
	::g_pTheLights->theLights[1].diffuse = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[1].specular = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[1].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 400.f);
	::g_pTheLights->theLights[1].param1.x = 0.f;    // point light
	::g_pTheLights->TurnOnLight(1);

	::g_pTheLights->theLights[2].position = glm::vec4(glm::vec3(3840.f, 250.f, 2300.f), 1.f);
	::g_pTheLights->theLights[2].diffuse = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[2].specular = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[2].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 500.f);
	::g_pTheLights->theLights[2].param1.x = 0.f;    // point light
	::g_pTheLights->TurnOnLight(2);

	::g_pTheLights->theLights[3].position = glm::vec4(glm::vec3(3150.f, 250.f, 2050.f), 1.f);
	::g_pTheLights->theLights[3].diffuse = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[3].specular = glm::vec4(1.f, 0.f,1.f, 1.f);
	::g_pTheLights->theLights[3].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 500.f);
	::g_pTheLights->theLights[3].param1.x = 0.f;    // point light
	::g_pTheLights->TurnOnLight(3);

	::g_pTheLights->theLights[4].position = glm::vec4(glm::vec3(2560.f, 250.f, 550.f), 1.f);
	::g_pTheLights->theLights[4].diffuse = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[4].specular = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[4].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 500.f);
	::g_pTheLights->theLights[4].param1.x = 0.f;    // point light
	::g_pTheLights->TurnOnLight(4);

	::g_pTheLights->theLights[5].position = glm::vec4(glm::vec3(4170.f, 250.f, 1050.f), 1.f);
	::g_pTheLights->theLights[5].diffuse = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[5].specular = glm::vec4(1.f, 0.f, 1.f, 1.f);
	::g_pTheLights->theLights[5].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 500.f);
	::g_pTheLights->theLights[5].param1.x = 0.f;    // point light
	::g_pTheLights->TurnOnLight(5);




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

void cDungeonMaster::PlaceTorches(int x_size,int z_size) {
	int x_step = 4;
	int z_step = 4;
	glm::vec4 adjacentWalls = glm::vec4(0.f);
	// Automatic torch placement.
	for (int x = 1; x < x_size; x += x_step) {
		for (int z = 1; z < z_size; z += z_step) {
			if (dungeonReader->GetObjectAtLocation(x, z) == 1) {
				adjacentWalls = dungeonReader->DetermineWallOrientation(x, z);
				if (adjacentWalls.x > 0.f) {
					cMesh* model = factory->createDungeonPiece(20, glm::vec3(x*500.f-250.f, 0.f, z*500.f-250.f));
					if (model) {
						model->orientationXYZ = (glm::vec3(0.f, -glm::pi<float>() / 2, 0.f));
						model->positionXYZ += glm::vec3(-215.f, 0.f, 0.f);
						dungeonModels.push_back(model);
						CreateFlame(model->positionXYZ + glm::vec3(20.f, 10.f, 0.f));
					}
				}
				if (adjacentWalls.y > 0.f) {
					cMesh* model = factory->createDungeonPiece(20, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
					if (model) {
						model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>() / 2, 0.f));
						model->positionXYZ += glm::vec3(215.f, 0.f, 0.f);
						dungeonModels.push_back(model);
						CreateFlame(model->positionXYZ + glm::vec3(-20.f, 10.f, 0.f));
					}
				}
				if (adjacentWalls.z > 0.f) {
					cMesh* model = factory->createDungeonPiece(20, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
					if (model) {
						model->orientationXYZ = (glm::vec3(0.f, glm::pi<float>(), 0.f));
						model->positionXYZ += glm::vec3(0.f, 0.f, -215.f);
						dungeonModels.push_back(model);
						CreateFlame(model->positionXYZ + glm::vec3(0.f, 10.f, 20.f));
					}
				}
				if (adjacentWalls.w > 0.f) {
					cMesh* model = factory->createDungeonPiece(20, glm::vec3(x * 500.f - 250.f, 0.f, z * 500.f - 250.f));
					if (model) {
						model->orientationXYZ = (glm::vec3(0.f, 0.f, 0.f));
						model->positionXYZ += glm::vec3(0.f, 0.f, 215.f);
						dungeonModels.push_back(model);
						CreateFlame(model->positionXYZ + glm::vec3(0.f,10.f,-20.f));
					}
				}
			}
		}
	}
}

void cDungeonMaster::CreateFlame(glm::vec3 position) {
	candlesPlaced++;
	if (candlesPlaced < ::g_pTheLights->NUMBER_OF_LIGHTS) {
		::g_pTheLights->theLights[candlesPlaced].position = glm::vec4(position, 1.f);
		::g_pTheLights->theLights[candlesPlaced].diffuse = glm::vec4(1.f, 0.4f, 0.f, 1.f);
		::g_pTheLights->theLights[candlesPlaced].specular = glm::vec4(1.f, 0.4f, 0.f, 1.f);
		::g_pTheLights->theLights[candlesPlaced].atten = glm::vec4(0.003f, 0.0001f, 0.00027f, 400.f);
		::g_pTheLights->theLights[candlesPlaced].param1.x = 0.f;    // point light
		::g_pTheLights->TurnOnLight(candlesPlaced);
	}
	std::cout << candlesPlaced++ << std::endl;
}