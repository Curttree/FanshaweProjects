# INFO 6019 - Physics & Simulation 1 - Midterm
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 or x86 with either Debug or Release configuration (all options should work).
- If you will be utilizing the generated .zip file or its contents, please either build as Release or run on computer with necessary dlls for C++ development.

## Running the Project
- Similar to project 2, I am utilizing a scene.json file in the Project2 folder to set the camera's focus and starting position. 
	This time though, the scroll wheel will also raise/lower the camera focus for a better look at projectiles.
- Scene.json also contains the config definitions for the cannon and projectiles.
- Projectiles can be triggered via either set of 1,2,3,4 keys if your keyboard has a keypad (ex. most standalone keyboards).
- Physics code has been re-written (thanks again for being understanding about this for project 2). The overall structure is largely the same, though there are a few small differences here and there.
- Projectile configuration uses default values for anything that is ommitted, so it should safely handle missing keys (ex. EnergyBall not explicitly declaring a mass, rgb values missing).
- After build, files are copied to a folder with the path of <SolutionDir>\Midterm\DeploymentFolder. This includes the .exe file as well as required .json config files, and relevant models/shaders.
	- I stated this above, but I am not copying C++ .dll files during a Debug build, so when testing this, make sure you build as release or test the .exe on a computer with the relevant .dlls.