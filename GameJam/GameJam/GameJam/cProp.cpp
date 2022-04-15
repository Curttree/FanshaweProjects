#include "cProp.h"
#include "globals.h"

cProp::cProp(std::string name, std::string texture, glm::vec3 position, glm::vec3 scale) {
    cMesh* prop_mesh = new cMesh();
    prop_mesh->setAllMeshNames(name);
    prop_mesh->scale = scale;
    prop_mesh->positionXYZ = position;
    prop_mesh->textureNames[0] = texture;
    prop_mesh->textureRatios[0] = 1.f;
    mesh = prop_mesh;
    position = prop_mesh->positionXYZ;
    scale = prop_mesh->scale;
    rotation = prop_mesh->orientationXYZ;
}