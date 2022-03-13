# INFO 6020 - Graphics 2 - Midterm
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- Camera controls should be similar to default fly cam (ex. WASD and Q/E to move the camera, mouse to rotate)
	- Provided full camera control to assist with marking, but would likely restrict the amount you could rotate camera/move if this were an actual scene.
- I added comments in the form of QUESTION_1 , QUESTION_2 , etc. in my code to highlight areas that were relevant to each question.
	- QUESTION_1 and QUESTION_2 are mainly being answered by the two config files objects.json and scene.json as well as the configManager.cpp file I have been using for my other Graphics assignments.
	- QUESTION_3 and QUESTION_4 comments are in the cEffectsManager.cpp. This is where I am tracking all of the timers for changing images/adding effects, and then the data gets passed to my frag shader from the DrawObject_function.cpp file.
		- The actual camera angles for Question 3 are configurable via the scene.json file. Any camera after the first one in the list is treated as a potential view to be shown on the monitors.
		- The exterior view of the ship is located behind the default camera view if you needed to verify anything with that model.