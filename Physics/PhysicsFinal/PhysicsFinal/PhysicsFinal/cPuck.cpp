#include "cPuck.h"
#include "globals.h"

cPuck::cPuck(glm::vec3 position, glm::vec3 velocity) :cEntity(new cMesh(),2.f,2.f){
	mesh->meshName = "puck_normalized.ply";
	mesh->scale = 2.5f;
	mesh->textureNames[0] = "puck.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->wholeObjectShininess_SpecPower = 50.f;
	mesh->positionXYZ = position;
	::g_vec_pMeshes.push_back(mesh);
	::g_vec_Entities.push_back(this);
	particle->SetPosition(position);
	particle->SetVelocity(velocity);
	particle->SetDamping(0.80f);
	worldSpace->_world->GetForceRegistry()->Register(particle, worldSpace->_puckDragGenerator);
	worldSpace->_world->GetForceRegistry()->Register(particle, worldSpace->_gravityGenerator);
	debugMesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}