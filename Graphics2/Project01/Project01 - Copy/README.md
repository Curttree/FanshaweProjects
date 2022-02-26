# INFO 6020 - Graphics 2 - Project 1
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- Camera controls should be similar to default fly cam (ex. WASD and Q/E to move the camera, mouse to rotate)
- The two off-screen textures are:
	1) Full screen 2nd pass rendering
	2) Used as an object in the scene (TV screen on the right side of the rink)
- The TV screen that utilizes an off-screen texture combines the rendered image with a randomized static effect.
- Full screen effects are:
	1) Trivial colour change to make the scene appear later in the evening/blend colours better.
	2) Blur effect to provide a closer match between the 3D models and the skybox.
- Reflection and refraction are both being utilized for the glass surrounding the rink (see video for example) 
	- Refraction demonstrated when the camera is outside of the rink
	- Reflection demonstrated when the camera is inside of the rink
- Tougher to spot, but the second TV on the left side of the starting camera orientation and glass bulbs behind either net are also reflective.