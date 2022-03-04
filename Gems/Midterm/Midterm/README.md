# INFO 6023 - Gems - Midterm
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- Camera controls should be similar to default fly cam (ex. WASD and Q/E to move the camera, mouse to rotate)
- All code specific to this midterm should either be in the 'Midterm' filter, or referenced from main.cpp.
- Container used in place of STL's vector is called 'cCurtArray'. The only area where a STL container is used is in the existing code tied to rendering or cMazeManager. 
	- I tried copying the maze manager nested vector to my container, but I ran into some issues getting it working. Hopefully this isn't too bad of a deduction.
- Carnivores are yellow cubes when awake, light blue when asleep
	- Added a cool-down to the sleep timer, so once a carnivore wakes, it should take a bit of time before it sleeps again.
- Plants are green when growing, red when mature.
- Herbivores are pink when wandering/running from carnivores, off-white when hungry
	- Assumption: Rather than path-finding through the grass to get to the fruit, I am allowing herbavores to cross the sand when pursuing fruit. Once they are no longer hungry, they cannot cross the sand.
		- Not sure if this was the correct approach but it allows for herbivores to consistently eat without needing to navigate the maze. Felt like they would die off too quickly otherwise.
- 'Poop' timer for herbivores was shortened to 20 seconds rather than every few minutes to help the herbivore population from growing extinct as quickly.