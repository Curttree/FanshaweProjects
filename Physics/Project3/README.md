# INFO 6019 - Physics & Simulation 1 - Project 3
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 or x86 with either Debug or Release configuration (all options should work).

## Running the Project
- Similar to project 2 and the midterm, I am utilizing a scene.json file in the Project2 folder to set the camera's focus and starting position. 
- Scene.json also contains the config definitions for the cannon and projectiles (the randomly generated particle is based on the 'Bullet' definition except with a randomized radius). These should be set at an appropriate level but are configurable should you want to test different interactions to help with marking.
- The randomly sized particles can be fired via the SPACE BAR. They will collide with other particles as well as the 5 planes I have defined (left, right, and back wall as well as the ceiling and floor). Mass is being calculated using the formula of a sphere: density * (4/3 * pi * radius^3).
	- Note that the attached video used a simplified version of this calculation, so different radius sizes will have more of an effect on the current version of the code compared to the video.
- Additional particle types can be fired via either set of 1,2,3,4 keys if your keyboard has a keypad (ex. most standalone keyboards).
- I did make a decision to remove particles that were located behind the cameras starting position, however any particles that are visible in the scene should not be destroyed (hopefully this is acceptable)