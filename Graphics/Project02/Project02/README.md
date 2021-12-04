# INFO 6028 - Graphics - Project 2
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug (if testing the "alternative" debug rendering) or Release configuration (both options should work).
## Running the Project / Notes
- Camera navigation should be close to the default fly camera (minor changes were made but it should be pretty similar functionally).
- Goal lights (Point lights) can be turned on by pressing and holding the 1 or 2 key.
	- In the final 'game' these would be triggered on a puck entering the net. For now they can be enabled at will.
- Transparency is shown via goal lights, watter bottles, and glass (highlighted in the video). If using Debug configuration, you can right click on the window and jump to camera locations that highlight the sorting of these objects.
- Discard transparency is present via the 'decals' on the boards near the center of the scene.
- Texture combinations are used in the following places:
	- The ice uses a trivial combination of the painted lines and an ice texture to make the surface look like it had been skated on.
	- The skaters use a base texture (with shared textures such as skate blades and their hockey stick) and a combination of texture masks to allow their uniform and skin colour to be easily changed
	- The concrete around the arena uses a mask texture and the snow texture from the skybox to make it look like there is some snow on the ground. The idea was to make the skybox seem more connected to the scene from camera angles that will be available in the final 'game'
- 9 lights are present in the scene (6 spot lights, 2 point lights which can be turned on/off, and a directional light). These are configurable via the scene.json file.
- If the 'Debug' configuration is used, you should be able to use the Right Click menu to toggle the collision details (labelled as hitboxes though I realize that is likely not the correct terminology in this case). This will show spheres/planes for the particle physics simulation around the models.
	- I am using "#if defined _DEBUG" to check for the current Configuration and control whether to show debug info. Not sure if this is the best/most scalable approach. If there is a better approach that I should be using instead, please let me know!