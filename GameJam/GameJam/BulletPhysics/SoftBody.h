#pragma once

#include <Fanshawe2022Physics/interfaces/iSoftBody.h>
#include <Fanshawe2022Physics/interfaces/cSoftBodyDesc.h>

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBody.h>

namespace gdp2022Physics
{
	class SoftBody : public iSoftBody
	{
	public:
		SoftBody(const cSoftBodyDesc& desc);
		~SoftBody();

		virtual unsigned int GetNumNodes();
		virtual void GetNodePosition(unsigned int nodeIndex, glm::vec3& nodePositionOut);

		btSoftBody* GetBulletBody() { return mBulletBody; }
	private:
		btSoftBody* mBulletBody;
	};
}