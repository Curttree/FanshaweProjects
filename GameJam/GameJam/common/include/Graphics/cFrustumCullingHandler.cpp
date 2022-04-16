#include "cFrustumCullingHandler.h"
#include "../../../GameJam/globals.h"

cFrustumCullingHandler* cFrustumCullingHandler::_instance = 0;

cFrustumCullingHandler::cFrustumCullingHandler() {

}

cFrustumCullingHandler* cFrustumCullingHandler::Instance() {
	if (_instance == 0) {
		_instance = new cFrustumCullingHandler;
	}

	return _instance;
}

sFrustum cFrustumCullingHandler::createFromMatricies(glm::mat4 view, glm::mat4 perspective) {
    sFrustum result;
    //glm::mat4 mv = perspective * view;

    //glm::vec4 row1 = glm::vec4(mv[0][0], mv[0][1], mv[0][2], mv[0][3]);
    //glm::vec4 row2 = glm::vec4(mv[1][0], mv[1][1], mv[1][2], mv[1][3]);
    //glm::vec4 row3 = glm::vec4(mv[2][0], mv[2][1], mv[2][2], mv[2][3]);
    //glm::vec4 row4 = glm::vec4(mv[3][0], mv[3][1], mv[3][2], mv[3][3]);

    //glm::vec4 p1 = row4 + row1;
    //glm::vec4 p2 = row4 - row1;
    //glm::vec4 p3 = row4 + row2;
    //glm::vec4 p4 = row4 - row2;
    //glm::vec4 p5 = row4 + row3;
    //glm::vec4 p6 = row4 - row3;

    //result.leftFace = { p1.w, glm::normalize(glm::vec3(p1.x, p1.y, p1.z)) };
    //result.rightFace = { p2.w, glm::normalize(glm::vec3(p2.x, p2.y, p2.z)) };
    //result.bottomFace = { p3.w, glm::normalize(glm::vec3(p3.x, p3.y, p3.z)) };
    //result.topFace = { p4.w, glm::normalize(glm::vec3(p4.x, p4.y, p4.z)) };
    //result.nearFace = { p5.w, glm::normalize(glm::vec3(p5.x, p5.y, p5.z)) };
    //result.farFace = { p6.w, glm::normalize(glm::vec3(p6.x, p6.y, p6.z)) };

    return result;
}

sFrustum cFrustumCullingHandler::createFromCamera(cFlyCamera* camera) {
    //Adapted from: https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
	sFrustum result;
    const float aspectRatio = ::g_pFBO->width / (float)::g_pFBO->height;
    const float halfVSide = 1000.f * tanf(camera->FOV * .5f);
    const float halfHSide = halfVSide * aspectRatio;
    const glm::vec3 frontMultFar = 1000.f * camera->getAtDirection();
    const glm::vec3 cameraRight = glm::cross(camera->getAtDirection(), camera->getUp());

    result.nearFace = { camera->getEye() + camera->nearPlane * camera->getAtDirection(), camera->getAtDirection() };
    result.farFace = { camera->getEye() + frontMultFar, -camera->getAtDirection() };
    result.rightFace = { camera->getEye(), glm::cross(camera->getUp(), frontMultFar + cameraRight * halfHSide) };
    result.leftFace = { camera->getEye(), glm::cross(frontMultFar - cameraRight * halfHSide, camera->getUp()) };
    result.bottomFace = { camera->getEye(), glm::cross(cameraRight, frontMultFar - camera->getUp() * halfVSide) };
    result.topFace = { camera->getEye(), glm::cross(frontMultFar + camera->getUp() * halfVSide, cameraRight) };
	return result;
}

bool cFrustumCullingHandler::isWithinFrustum(const sFrustum cameraFrustum, const cMesh* mesh) {
    if (mesh->boundingRadius < 0) {
        //No valid radius has been set. Always draw.
        return true;
    }

    bool close = isWithinPlaneBounds(cameraFrustum.nearFace, mesh, true);
    bool away = isWithinPlaneBounds(cameraFrustum.farFace, mesh, true);
    return (close && away);

    //return (isWithinPlaneBounds(cameraFrustum.nearFace, mesh) &&
   //     isWithinPlaneBounds(cameraFrustum.farFace, mesh) &&
   //     isWithinPlaneBounds(cameraFrustum.rightFace, mesh) &&
   //     isWithinPlaneBounds(cameraFrustum.leftFace, mesh) &&
   //     isWithinPlaneBounds(cameraFrustum.topFace, mesh) &&
   //     isWithinPlaneBounds(cameraFrustum.bottomFace, mesh));
}

bool cFrustumCullingHandler::isWithinPlaneBounds(const sPlane plane, const cMesh * mesh, bool greaterThan) {
    //Check if we are either intersecting, or within the plane.
    //float test = glm::dot(plane.normal, mesh->positionXYZ) + plane.distance + mesh->boundingRadius;
    //return test > 0;
    float distance = glm::dot(plane.normal, mesh->positionXYZ) - plane.distance;
    if (greaterThan) {
       return distance >= mesh->boundingRadius;
    }
    return distance <= mesh->boundingRadius;
}