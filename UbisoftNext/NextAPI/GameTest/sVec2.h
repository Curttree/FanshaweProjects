#pragma once

//NOTE: I could've used an existing math library here, but since the use case was relatively simple, I wanted to include a minimal amount of external code. 
// If we wanted to use more complex calculations, we could replace this with an existing vector class.

struct Vec2 {
	float x;
	float y;
};