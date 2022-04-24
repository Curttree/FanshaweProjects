# INFO 6023 - Gems - Game Jam
# INFO 6020 - Graphics 2 - Game Jam
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 with either Debug or Release configuration (both options should work).

## Running the Project
- Controls are:
    - No input : Idle Animation
    - W/A/S/D : Walk
    - Shift + W/A/S/D : Run
    - Q / Don't press anything for  a while : Waiting Animation
    - Right Click : Aim
    - Left Click (while Aiming) : Shoot

- This project was developed for educational purposes only. You are welcome to reference the code, however this project should not be referenced/otherwise utilized for commercial purposes.

- 4 Technical challenges being implemented are:
1) Underwater filter - Combined g-buffer depth info and an underwater normal texture that has it's UV location changed over time to reproduce that 'underwater effect' I had linked, but with some added visual style, such as a colour shift from red to blue as you get further from the camera.

2) Particles - There are various bubbles throughout the scene (coming from vents and as part the 'waiting' animation), as well as an impact effect that appears wherever you shoot. These are all quads rotated to face the camera.
I was going to sort these by transparency, but since they are so small, I decided to forgo this step.
Particles are also utilizing the level of detail code (more on that below) to make sure we don't draw particles that are too far in the distance.

3) Level of Detail - All models now support a high/medium/low detail mesh. If any of these meshes are missing, the object does not get drawn.
Outside of the particles, I am leveraging this to render the next city block. Rather than rendering the actual 3D models when you are far away, an imposter with an approximation of the next block is drawn. As you get closer to the end of the stage, the imposter is removed and we start rendering the next set of buildings.

4) Frustum culling - Implemented with an approximation of the frustum. Used for smaller objects within the scene such as props.

# Additional Info
Bubble Texture Vectors by Vecteezy - https://www.vecteezy.com/free-vector/bubble-texture
Old man dialogue sampled from The Simpsons episode 'Marge on the Lam' (Season 5 Episode 6). Used as a placeholder.