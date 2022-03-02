# INFO 6022 - Physics & Simulation 2 - Project 1
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 with either Debug or Release configuration (both options should work).

## Running the Project
- Controls are the same as Project 1 except now movement is applied as an angular velocity rather than linear. Friction values and movement speeds have been set for planes so that they feel relatively realistic but also allow for some interesting collisions.
- Note the active ball now uses a '15' texture rather than the plain white cue ball to help illustrate the rotation of the ball.

## Project 1 Details are listed below.
- Controls should match the assignment description (WASD to move the active ball, 1-5 keys to switch ball, arrow keys to control camera)
    - Each ball is labelled with a number that will let you change to it. The 'active' ball is plain white.
- All physics code in the Assignment1 project should be going through the provided interfaces (beyond the initial definitions for Factory, etc). All physics calculations are handled within the appropriate project and are abstracted from the Assignment 1 project.
- BONUS: A 3D sound effect will be played when two balls collide. For ease of listening, if a sound is already playing, no new sound will be played.
