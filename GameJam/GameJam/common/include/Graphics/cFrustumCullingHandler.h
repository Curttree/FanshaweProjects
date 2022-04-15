#pragma once
#include "../FlyCamera/cFlyCamera.h"
#include "sFrustum.h"
#include <cMesh.h>

// Down the road, update so uses iCamera instead of specifically the fly cam.
class cFrustumCullingHandler {
public:
	static cFrustumCullingHandler* Instance();
	sFrustum createFromCamera(cFlyCamera* camera);
	sFrustum createFromMatricies(glm::mat4 view, glm::mat4 perspective);
	bool isWithinFrustum(const sFrustum cameraFrustum, const cMesh* mesh);
private:
	cFrustumCullingHandler();
	static cFrustumCullingHandler* _instance;
	bool isWithinPlaneBounds(const sPlane plane, const cMesh* mesh);
};