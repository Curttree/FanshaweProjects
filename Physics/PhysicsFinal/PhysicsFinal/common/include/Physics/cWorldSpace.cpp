#include "cWorldSpace.h"
#include <iostream>
#include "../../../PhysicsFinal/cParticleFactory.h"
#include "../../../PhysicsFinal/cParticleConstraints.h"

cWorldSpace::cWorldSpace()
{
}

// Particle on Particle collisions
cParticleParticleContactGenerator particleCollisionGenerator;


cWorldSpace* cWorldSpace::_instance = 0;
glm::mat3  cWorldSpace::axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
cMathHelper* cWorldSpace::_mathHelper = cMathHelper::Instance();
cParticleFactory* factory = cParticleFactory::Instance();

cWorldSpace* cWorldSpace::Instance() {
	if (_instance == 0) {
		_instance = new cWorldSpace;

		//Initialize physics
		_instance->_gravityGenerator = new cGravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
		_instance->_puckDragGenerator = new cApproxPuckDragGenerator(glm::vec3(0.0f, -65.f, 0.0f));
		_instance->_world = new cParticleWorld(5000);
		_instance->_world->AddConstraintGenerator(&particleCollisionGenerator);
	}

	return _instance;
}

// MATH/Starter code from first physics assignment

glm::vec3 cWorldSpace::getRandomXVector()
{
	return glm::vec3(_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

glm::vec3 cWorldSpace::getRandomZVector()
{
	return glm::vec3(-_mathHelper->getRandom(0.1f, 1.f), 0.f, _mathHelper->getRandom(0.1f, 1.f));
}

// If the user decides to override the alpha value, use the provided value rather than randomly assigning.
glm::vec3 cWorldSpace::getRandomVector3(float magnitude, bool overrideAlpha, float alpha)
{
	if (!overrideAlpha){
		alpha = _mathHelper->getRandom(glm::pi<float>());
	}
	float sinAlpha = glm::sin(alpha);
	float cosAlpha = glm::cos(alpha);

	float delta = _mathHelper->getRandom(-glm::pi<float>(), glm::pi<float>());
	float sinDelta = glm::sin(delta);
	float cosDelta = glm::cos(delta);

	return glm::vec3(magnitude * sinAlpha * cosDelta, magnitude * sinAlpha * sinDelta, magnitude * cosAlpha);
}

// Generate an orthonormal basis, given two linearly independent vectors.
// The input vectors are unaffected.
glm::mat3 cWorldSpace::orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
	// If randomized orthonormal basis complicates simulation in the future, replace with standard axis.
	glm::vec3 _xVec = glm::vec3(xVec);
	glm::vec3 _yVec = glm::cross(zVec, _xVec);
	glm::vec3 _zVec = glm::cross(_xVec, _yVec);

	// We are orthogonal, now normalize!
	_xVec = glm::normalize(_xVec);
	_yVec = glm::normalize(_yVec);
	_zVec = glm::normalize(_zVec);
	return glm::mat3(_xVec, _yVec, _zVec);
}

glm::vec3 cWorldSpace::getPositionInWorldSpace(const glm::vec3 orientationXYZ, const glm::vec3 startPositionXYZ) {
	glm::mat4 identity = glm::mat4(1.f);
	glm::mat4 result = identity;

	glm::mat4 rotateZ = glm::rotate(identity, orientationXYZ.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotateY = glm::rotate(identity, orientationXYZ.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateX = glm::rotate(identity, orientationXYZ.x, glm::vec3(1.0f, 0.0f, 0.0f));

	result *= rotateZ;
	result *= rotateY;
	result *= rotateX;

	return result * glm::vec4(startPositionXYZ.x, startPositionXYZ.y, startPositionXYZ.z, 1.f);
}

void cWorldSpace::SetWorldBounds(glm::vec3 positiveBounds, glm::vec3 negativeBounds, float cornerRadius, int samplePoints) {
	// Planes
	cPlaneParticleContactGenerator* groundGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 1.f, 0.f), negativeBounds.y);
	cPlaneParticleContactGenerator* ceilingGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, -1.f, 0.f), -positiveBounds.y);
	cPlaneParticleContactGenerator* wallGenerator = new cPlaneParticleContactGenerator(glm::vec3(1.f, 0.f, 0.f), negativeBounds.x);
	cPlaneParticleContactGenerator* wall2Generator = new cPlaneParticleContactGenerator(glm::vec3(-1.f, 0.f, 0.f), -positiveBounds.x);
	cPlaneParticleContactGenerator* backWallGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 0.f, -1.f), -positiveBounds.z);
	cPlaneParticleContactGenerator* frontWallGenerator = new cPlaneParticleContactGenerator(glm::vec3(0.f, 0.f, 1.f), negativeBounds.z);
	_instance->_world->AddConstraintGenerator(groundGenerator);
	_instance->_world->AddConstraintGenerator(ceilingGenerator);
	_instance->_world->AddConstraintGenerator(wallGenerator);
	_instance->_world->AddConstraintGenerator(wall2Generator);
	_instance->_world->AddConstraintGenerator(backWallGenerator);
	_instance->_world->AddConstraintGenerator(frontWallGenerator);
	std::cout << "World bounds set" << std::endl;

	if (cornerRadius > 0.f && samplePoints > 0) {
		// Find the four center points.
		glm::vec3 cornerOneCenter = glm::vec3(-(negativeBounds.x) - cornerRadius, 0.f, (-negativeBounds.z) - cornerRadius);
		glm::vec3 cornerTwoCenter = glm::vec3(-(positiveBounds.x) + cornerRadius, 0.f, -(negativeBounds.z) - cornerRadius);

		for (int x = 0; x < samplePoints; x++) {
			//Find the displacement.
			float step = 90.f * (x + 1.f) / (samplePoints+1.f);	// Add one to the denominator, as our last point would be on the boundary.
			glm::vec3 point = glm::rotateY(glm::vec3(-cornerRadius, 0.f, 0.f), glm::radians(step));
			glm::vec3 normal = glm::rotateY(glm::vec3(1.0f, 0.f, 0.f), glm::radians(step));
			point += cornerOneCenter; 
			float displacement = glm::length(glm::vec3(point.x - cornerOneCenter.x, point.y, point.z));
			displacement *= -1.f; 
			_instance->_world->AddConstraintGenerator(new cPlaneParticleContactGenerator(normal, displacement));
		}

		for (int x = 0; x < samplePoints; x++) {
				//Find the displacement.
				float step = -90.f * (x + 1.f) / (samplePoints+1.f);	// Add one to the denominator, as our last point would be on the boundary.
				glm::vec3 point = glm::rotateY(glm::vec3(cornerRadius, 0.f, 0.f), glm::radians(step));
				glm::vec3 normal = glm::rotateY(glm::vec3(-1.0f, 0.f, 0.f), glm::radians(step));
				point += cornerTwoCenter;
				float displacement = glm::length(glm::vec3(point.x - cornerTwoCenter.x, point.y, point.z));
				displacement *= -1.f; 
				_instance->_world->AddConstraintGenerator(new cPlaneParticleContactGenerator(normal, displacement));
		}
	}
}

void cWorldSpace::SetupNet(glm::vec3 position) {
	// Setup anchors
	glm::vec3 anchor1_offset = glm::vec3(12.f, -5.f, 0.f);
	cEntity* anchor1 = factory->createParticle(1, position + anchor1_offset);
	anchor1->particle->SetDensity(-1.f);

	glm::vec3 anchor2_offset = glm::vec3(-12.f, -5.f, 0.f);
	cEntity* anchor2 = factory->createParticle(1, position + anchor2_offset);
	anchor2->particle->SetDensity(-1.f);

	std::vector<cEntity*> post1positions;
	std::vector<cEntity*> post2positions;
	std::vector<cEntity*> post1;
	std::vector<cEntity*> post2;

	std::vector<cEntity*> crossbarpositions;
	std::vector<cEntity*> crossbar;

	std::vector<cEntity*> basebarpositions;
	std::vector<cEntity*> basebar;

	std::vector<cEntity*> leg1positions;
	std::vector<cEntity*> leg1;
	std::vector<cEntity*> leg2positions;
	std::vector<cEntity*> leg2;
	//Start building the goal posts.
	{
		cEntity* postStart = factory->createParticle(1, anchor1->particle->GetPosition() + glm::vec3(0.f, 3.f, 0.f));
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(postStart->particle, anchor1->particle)); 
		postStart->particle->SetDensity(-1.f);
		post1positions.push_back(postStart);

		//Serve as position locks for the particles.
		for (int x = 1; x < 9; x++) {
			cEntity* current = factory->createParticle(1, anchor1->particle->GetPosition() + glm::vec3(0.f, ((float)x * 2.f) + 3.f, 0.f));
			current->particle->SetDensity(-1.f);
			post1positions.push_back(current);
		}
		for (int x = 0; x < 9; x++) {
			cEntity* current = factory->createParticle(1, anchor1->particle->GetPosition() + glm::vec3(0.f, ((float)x * 2.f) + 3.f, 0.f));
			_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(post1positions[x]->particle, current->particle));
			post1.push_back(current);
		}
	}
	{
		cEntity* postStart = factory->createParticle(1, anchor1->particle->GetPosition() + glm::vec3(0.f, 3.f, 0.f));
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(postStart->particle, anchor2->particle));
		postStart->particle->SetDensity(-1.f);
		post2positions.push_back(postStart);

		//Serve as position locks for the particles.
		for (int x = 1; x < 9; x++) {
			cEntity* current = factory->createParticle(1, anchor2->particle->GetPosition() + glm::vec3(0.f, ((float)x * 2.f) + 3.f, 0.f));
			current->particle->SetDensity(-1.f);
			post2positions.push_back(current);
		}
		for (int x = 0; x < 9; x++) {
			cEntity* current = factory->createParticle(1, anchor2->particle->GetPosition() + glm::vec3(0.f, ((float)x * 2.f) + 3.f, 0.f));
			_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(post2positions[x]->particle, current->particle));
			post2.push_back(current);
		}
	}

	//Build the crossbar
	{
		cEntity* barStart = factory->createParticle(1, post2[8]->particle->GetPosition() + glm::vec3(2.f, 0.f, 0.f));
		barStart->particle->SetDensity(-1.f);
		crossbarpositions.push_back(barStart);
	}
	for (int x = 1; x < 11; x++) {
		cEntity* current = factory->createParticle(1, crossbarpositions[x - 1]->particle->GetPosition() + glm::vec3(2.f, 0.f, 0.f));
		current->particle->SetDensity(-1.f);
		crossbarpositions.push_back(current);
	}
	for (int x = 0; x < 11; x++) {
		cEntity* current = factory->createParticle(1, crossbarpositions[x]->particle->GetPosition());
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(crossbarpositions[x]->particle, current->particle));
		crossbar.push_back(current);
	}

	//Build the 1st leg (post that goes backwards.
	{
		cEntity* barStart = factory->createParticle(1, post1[0]->particle->GetPosition() + glm::vec3(0.f, 1.5f, 2.f));
		barStart->particle->SetDensity(-1.f);
		leg1positions.push_back(barStart);
	}
	for (int x = 1; x < 5; x++) {
		cEntity* current = factory->createParticle(1, leg1positions[x - 1]->particle->GetPosition() + glm::vec3(0.f, 0.f, 2.f));
		current->particle->SetDensity(-1.f);
		leg1positions.push_back(current);
	}
	for (int x = 0; x < 5; x++) {
		cEntity* current = factory->createParticle(1, leg1positions[x]->particle->GetPosition());
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(leg1positions[x]->particle, current->particle));
		leg1.push_back(current);
	}

	//Build the 2nd leg (post that goes backwards.
	{
		cEntity* barStart = factory->createParticle(1, post2[0]->particle->GetPosition() + glm::vec3(0.f, 1.5f, 2.f));
		barStart->particle->SetDensity(-1.f);
		leg2positions.push_back(barStart);
	}
	for (int x = 1; x < 5; x++) {
		cEntity* current = factory->createParticle(1, leg2positions[x - 1]->particle->GetPosition() + glm::vec3(0.f, 0.f, 2.f));
		current->particle->SetDensity(-1.f);
		leg2positions.push_back(current);
	}
	for (int x = 0; x < 5; x++) {
		cEntity* current = factory->createParticle(1, leg2positions[x]->particle->GetPosition());
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(leg2positions[x]->particle, current->particle));
		leg2.push_back(current);
	}

	//Build the basebar
	{
		cEntity* barStart = factory->createParticle(1, leg2[4]->particle->GetPosition() + glm::vec3(2.f, 0.f, 0.f));
		barStart->particle->SetDensity(-1.f);
		basebarpositions.push_back(barStart);
	}
	for (int x = 1; x < 11; x++) {
		cEntity* current = factory->createParticle(1, basebarpositions[x - 1]->particle->GetPosition() + glm::vec3(2.f, 0.f, 0.f));
		current->particle->SetDensity(-1.f);
		basebarpositions.push_back(current);
	}
	for (int x = 0; x < 11; x++) {
		cEntity* current = factory->createParticle(1, basebarpositions[x]->particle->GetPosition());
		_instance->_world->AddConstraintGenerator(new cParticleRodConstraint(basebarpositions[x]->particle, current->particle));
		basebar.push_back(current);
	}

	// Set up mesh
	// Start with points that will be joined via cables
	std::vector<std::vector<cEntity*>> attached;
	std::vector<std::vector<cEntity*>> attached2;
	std::vector<std::vector<cEntity*>> attachedBack;
	std::vector<std::vector<cEntity*>> attachedTop;
	float constant = 2.5f;

	for (int y = 0; y < 5; y++) {
		std::vector<cEntity*> row;
		attached.push_back(row);
		cEntity* current = factory->createParticle(2, glm::vec3(leg1[0]->particle->GetPosition().x, post1[2+y]->particle->GetPosition().y + 0.1f, leg1[0]->particle->GetPosition().z));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(post1[2+y]->particle, current->particle, constant));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(leg1[0]->particle, current->particle, constant));
		attached[y].push_back(current);

		for (int x = 1; x < 5; x++) {

			current = factory->createParticle(2, glm::vec3(leg1[x]->particle->GetPosition().x, attached[y][x - 1]->particle->GetPosition().y + 0.1f, leg1[x]->particle->GetPosition().z));
			_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached[y][x - 1]->particle, current->particle, constant));
			if (y > 0) {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached[y-1][x]->particle, current->particle, constant));
			}
			else {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(leg1[x]->particle, current->particle, constant));
			}
			attached[y].push_back(current);
		}

	}


	for (int y = 0; y < 6; y++) {
		std::vector<cEntity*> row;
		attached2.push_back(row);
		cEntity* current = factory->createParticle(2, glm::vec3(leg2[0]->particle->GetPosition().x, post2[2 + y]->particle->GetPosition().y + 0.1f, leg2[0]->particle->GetPosition().z));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(post2[2 + y]->particle, current->particle, constant));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(leg2[0]->particle, current->particle, constant));
		attached2[y].push_back(current);

		for (int x = 1; x < 5; x++) {

			current = factory->createParticle(2, glm::vec3(leg2[x]->particle->GetPosition().x, attached2[y][x - 1]->particle->GetPosition().y + 0.1f, leg2[x]->particle->GetPosition().z));
			_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached2[y][x - 1]->particle, current->particle, constant));
			if (y > 0) {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached2[y - 1][x]->particle, current->particle, constant));
			}
			else {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(leg2[x]->particle, current->particle, constant));
			}
			attached2[y].push_back(current);
		}

	}

	// Back of the net
	for (int y = 0; y < 6; y++) {
		std::vector<cEntity*> row;
		attachedBack.push_back(row);
		cEntity* current = factory->createParticle(2, glm::vec3(basebar[0]->particle->GetPosition().x, post2[1 + y]->particle->GetPosition().y + 0.1f, basebar[0]->particle->GetPosition().z));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached2[y][4]->particle, current->particle, constant));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(basebar[0]->particle, current->particle, constant));
		attachedBack[y].push_back(current);

		for (int x = 1; x < 11; x++) {

			current = factory->createParticle(2, glm::vec3(basebar[x]->particle->GetPosition().x, attachedBack[y][x - 1]->particle->GetPosition().y + 0.1f, basebar[x]->particle->GetPosition().z));
			_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedBack[y][x - 1]->particle, current->particle, constant));
			if (y > 0) {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedBack[y - 1][x]->particle, current->particle, constant));
			}
			else {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(basebar[x]->particle, current->particle, constant));
			}
			attachedBack[y].push_back(current);
		}
	}

	// Top of the net
	for (int z = 0; z < 5; z++) {
		std::vector<cEntity*> row;
		attachedTop.push_back(row);
		cEntity* current = factory->createParticle(2, glm::vec3(crossbar[0]->particle->GetPosition().x, crossbar[0]->particle->GetPosition().y + 0.1f, attached[4][z]->particle->GetPosition().z));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached[4][z]->particle, current->particle, constant));
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(crossbar[0]->particle, current->particle, constant));
		attachedTop[z].push_back(current);

		for (int x = 1; x < 11; x++) {

			current = factory->createParticle(2, glm::vec3(crossbar[x]->particle->GetPosition().x, crossbar[x]->particle->GetPosition().y + 0.1f, attached[4][z]->particle->GetPosition().z));
			_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedTop[z][x - 1]->particle, current->particle, constant));
			if (z > 0) {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedTop[z - 1][x]->particle, current->particle, constant));
			}
			else {
				_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(crossbar[x]->particle, current->particle, constant));
			}
			attachedTop[z].push_back(current);
		}
	}

	//Make the final constraint attachments.
	for (int x = 0; x < 11; x++) {
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedBack[5][x]->particle, attachedTop[4][x]->particle, constant));
	}

	for (int y = 0; y < 5; y++) {
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attachedBack[y][10]->particle, attached[y][4]->particle, constant));
	}

	for (int z = 0; z < 5; z++) {
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached[4][z]->particle, attachedTop[z][0]->particle, constant));
	}

	for (int z = 0; z < 5; z++) {
		_instance->_world->AddConstraintGenerator(new cParticleElasticConstraint(attached2[4][z]->particle, attachedTop[z][10]->particle, constant));
	}



}
