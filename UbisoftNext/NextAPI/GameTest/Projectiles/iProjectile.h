#pragma once

class iProjectile {
public:
	iProjectile(){};
	virtual ~iProjectile() {};
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy() = 0;
	virtual void Draw() = 0;
	virtual bool ShouldDestroy() = 0;
};