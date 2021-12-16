#include "cParticleConstraints.h"

cParticleConstraint::cParticleConstraint(cParticle* _particle1, cParticle* _particle2) {
	particle1 = _particle1;
	particle2 = _particle2;
}

cParticleConstraint::~cParticleConstraint() {

}

cParticle* cParticleConstraint::GetParticle1() {
	return particle1;
}
cParticle* cParticleConstraint::GetParticle2() {
	return particle2;
}

cParticleRodConstraint::~cParticleRodConstraint() {

}

cParticleRodConstraint::cParticleRodConstraint(cParticle* _particle1, cParticle* _particle2) : cParticleConstraint(_particle1, _particle2) {
	length = glm::distance(_particle1->GetPosition(), _particle2->GetPosition());
}

size_t cParticleRodConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const {
	float current = glm::distance(particle1->GetPosition(), particle2->GetPosition());

	if (glm::abs(current - length) == 0.f) {
		return 0;
	}

	//We need to adjust the positions so we maintain our length.
	contact->particle0 = particle1;
	contact->particle1 = particle2;

	glm::vec3 normal = glm::normalize(particle1->GetPosition() - particle2->GetPosition());

	if (current < length) {
		// Too close, move apart.
		contact->contactNormal = normal;
		contact->penetrationDepth = current - length;
	}
	else {
		// Too far apart, move together;

		contact->contactNormal = -(normal);
		contact->penetrationDepth = length - current;
	}

	contact->restitutionCoefficient = 0.f;

	return 1;
}
cParticleCableConstraint::~cParticleCableConstraint() {

}

cParticleCableConstraint::cParticleCableConstraint(cParticle* _particle1, cParticle* _particle2) : cParticleConstraint(_particle1, _particle2) {
	length = glm::distance(_particle1->GetPosition(), _particle2->GetPosition());
}

size_t cParticleCableConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const {
	float current = glm::distance(particle1->GetPosition(), particle2->GetPosition());

	if (current <= length) {
		return 0;
	}

	//We need to adjust the positions so we maintain our length.
	contact->particle0 = particle1;
	contact->particle1 = particle2;

	glm::vec3 normal = glm::normalize(particle1->GetPosition() - particle2->GetPosition());

	contact->contactNormal = -normal;
	contact->penetrationDepth = length - current  ;

	contact->restitutionCoefficient = 0.f;

	return 1;
}
cParticleElasticConstraint::~cParticleElasticConstraint() {

}

cParticleElasticConstraint::cParticleElasticConstraint(cParticle* _particle1, cParticle* _particle2, float _springConstant) : cParticleConstraint(_particle1, _particle2) {
	length = glm::distance(_particle1->GetPosition(), _particle2->GetPosition());
	springConstant = _springConstant;
}

size_t cParticleElasticConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const {
	float current = glm::distance(particle1->GetPosition(), particle2->GetPosition());

	if (current <= length) {
		return 0;
	}

	//We need to adjust the positions so we maintain our length.
	contact->particle0 = particle1;
	contact->particle1 = particle2;

	current = springConstant * (length - current);
	glm::vec3 normal = glm::normalize(particle1->GetPosition() - particle2->GetPosition());

	contact->contactNormal = -normal;
	contact->penetrationDepth = current ;

	contact->restitutionCoefficient = 0.f;

	return 1;
}