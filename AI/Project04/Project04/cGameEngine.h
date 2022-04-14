#pragma once

class cGameEngine {

public:
	cGameEngine();
	virtual ~cGameEngine();

	bool Initialize();
	void Destory();

	void Update(float deltaTime);
	void Render();

	//TODO: Abstract components into their appropriate managers rather than handling as part of the 'main' loop

private:
	void LoadScene();

	bool isRunning;
	bool isPaused;
	float gameSpeed = 1.f;

};