#include "cPostParticle.h"
#include "globals.h"

cPostParticle::cPostParticle(glm::vec3 position) :cEntity(new cMesh(), 0.9f, 2.f) {
	//mesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	mesh->scale = 0.8f;
	mesh->positionXYZ = position;
	::g_vec_pMeshes.push_back(mesh);
	::g_vec_Entities.push_back(this);
	particle->SetPosition(position);
	particle->SetDensity(25.f);
	particle->SetDamping(0.f);
}