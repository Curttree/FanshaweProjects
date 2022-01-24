# INFO 6019/6025 - Physics 1 and Configuration - Final Exam Project
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug (if testing that no Deployment package is created) or Release configuration (both options should work, though a deployment folder is only created in the ProjectDir when building as Release).
## Running the Project / Notes

- Multiple config files are available, though only 'physics.json' should be relevant to this project.
- Controls are:
	- Arrow Keys aim your shot, space bar fires a puck
	- 1, 2, 3 keys change positions (as defined in config file). You can also right click on the window to select a position.
	- Right clicking the window also lets you toggle visualization for the particles used for physics calculations.
- Explanation of config options can be found in the attached video. All angles are given in degrees.
- Multiple planes are used to approximate the corners of the rink. The amount and location of these planes is configurable, though 		I tried to set relatively accurate default values.
- Sphere/sphere collision is present between pucks as well as the puck and the net.
- A simple force generator was used to add 'drag' to the puck so it's tragectory is more accurate to a rubber puck rather than a round particle with a similarly small mass.
- Mass values/dampening was set for the puck first, and then other values were adjusted to provide a better gameplay experience. Some values such as the netting have a higher mass/lower dampening to allow for the net to appear more taut than it might otherwise be.
- Rod constraints are present to lock the post particles in place. Elastic constraints are used to approximate a mesh for the netting.
- The deployment folder is created within the PhysicsFinal project folder. A .Zip file is also created (not sure whether that was a requirement this time).
- The installer script will be automatically copied to the deployment folder on creation. Building an installer using the script and running it will install a folder containing the .exe and all other required files (along with an uninstaller) on the desktop.