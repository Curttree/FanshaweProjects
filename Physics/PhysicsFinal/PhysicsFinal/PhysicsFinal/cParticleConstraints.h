#pragma once
#include <Physics/iParticleContactGenerator.h>
class cParticleConstraint : public iParticleContactGenerator {
public:
	cParticleConstraint() = delete;
	cParticleConstraint(cParticle* _particle1, cParticle* _particle2);
	virtual ~cParticleConstraint();

	cParticle* GetParticle1();
	cParticle* GetParticle2();

protected:
	cParticle* particle1;
	cParticle* particle2;
};

class cParticleRodConstraint : public cParticleConstraint {
public:
	cParticleRodConstraint() = delete;
	cParticleRodConstraint(cParticle* _particle1, cParticle* _particle2);
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	virtual ~cParticleRodConstraint();

protected:
	float length;
};

class cParticleCableConstraint : public cParticleConstraint {
public:
	cParticleCableConstraint() = delete;
	cParticleCableConstraint(cParticle* _particle1, cParticle* _particle2);
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	virtual ~cParticleCableConstraint();

protected:
	float length;
};

class cParticleElasticConstraint : public cParticleConstraint {
public:
	cParticleElasticConstraint() = delete;
	cParticleElasticConstraint(cParticle* _particle1, cParticle* _particle2, float _springConstant);
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	virtual ~cParticleElasticConstraint();

protected:
	float length = 0.f;
	float springConstant = 0.f;
};


