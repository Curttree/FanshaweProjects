#include "cShieldPixel.h"

cShieldPixel::cShieldPixel(glm::vec3 position) {
	mesh = new cMesh();
	mesh->bDontLight = true;
	mesh->bUseWholeObjectDiffuseColour = true;
	mesh->wholeObjectSpecularRGB = glm::vec3(0.f, 1.f, 0.f);
	mesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
	mesh->friendlyName = "Shield";
	mesh->scale = 2.f;
	mesh->meshName = "Invaders/Invader_Single_Cube.ply";
	mesh->positionXYZ = position;

	particleScale = 20.f;
	particle = new cParticle(mesh->positionXYZ, 1.f, particleScale);
	particle->SetDamping(0.9f);
	particle->type = mesh->friendlyName;
	particle->owner = this;
	worldSpace->_world->AddParticle(particle);
	debugMesh = new cMesh();
	debugMesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	debugMesh->bIsWireframe = true;
	debugMesh->bUseObjectDebugColour = true;
	debugMesh->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	debugMesh->bDontLight = true;
	debugMesh->scale = particleScale;
	debugMesh->positionXYZ = mesh->positionXYZ;

}

bool cShieldPixel::RecieveMessage(sMessage theMessage) {
	if (theMessage.command == "Destroy") {
		worldSpace->_world->RemoveParticle(particle);
		mesh->scale = 0.f;
	}
	return true;
}
bool cShieldPixel::RecieveMessage(sMessage theMessage, sMessage& theResponse) {
	return true;
}
bool cShieldPixel::SetReciever(iMediator* pTheReciever) {
	return true;
}