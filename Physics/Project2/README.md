# INFO 6019 - Physics & Simulation 1 - Project 2
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 or x86 with either Debug or Release configuration (all options should work).

## Running the Project
- I am utilizing a scene.json file in the Project2 folder to set the camera's focus and starting position. You can move the camera with arrow keys/WASD as well as the scroll wheel, however focus will remain locked on a given point to ensure you will
  approximately be looking at the fireworks.
- I set up a few filters in the solution to start to organize my code. Main.cpp is still a bit messy from our Graphics course (my starting point for the assignment), but most of the new code relevant to this assignment can be found in the Fireworks filter.
- Fireworks can be triggered via either set of 1,2,3 keys if your keyboard has a keypad (ex. most standalone keyboards).
- The three firework designs are assembled by the cFireworkBuilder class. I am independently building a firework object and a fuse that determines when it should progress to the next stage. This allowed me to rapidly prototype different firework options.
- Explosion patterns and colour are determined via the individual FireworkObject classes. The builder determines the velocity and direction the firework will be launched at. 
- The position that the firework is generated at is determined by whatever calls the builder (ex. main.cpp in response to keyboard output, or a firework object when it is ready for the next stage).
- Firework #1 is fired in a tight angle upwards at a relatively large velocity and explodes after a set amount of time. It generates a large spherical explosion.
- Firework #2 is fired in a large range of angles at a small velocity and explodes once it starts falling back to the ground. It generates a circular explosion.
- Firework #3 is my 3 stage firework. It is fired a bit slower than Firework #1 at a larger range of angles and splits into two other fireworks after a few seconds that each explode on their own. These stage 2 fireworks generate a tight sphere of particles.