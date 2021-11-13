# INFO 6044 - Graphics - Midterm
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 with either Debug or Release configuration (both options should work).
## Running the Project
-INFO6044MidtermPt1.mkv covers Questions 1-6
- Questions 1-6 have been answered in the solution contained in the INFO6044Midterm_Q1-6 folder.
	- Within the contained solution, I tried to use filters to logically separate the different pieces of the project although some areas (ex. player/AI tank behaviour) overlaps.
	- Question 1: 
		-The .ply file generated is included in the projectDir/assets/models folder.
	- Question 2: 
		- cCombatMediator (in Questions 1,2,3,6 / Patterns filter) has the code to place the tanks and setup the game.
		- cPlayerTank (in Questions 1,2,3,6 / Tanks filter) has the code to move the tank
		- cMazeManager (in Questions 1,2,3,6 / Maze filter) handles collision checks for the mediator.
		- InputHandling (in Questions 1,2,3,6 / Input filter) handles keyboard input.
	- Question 3:
		- cAITank (in Questions 1,2,3,6 / Tanks filter)  has the "dumb" AI 
	- Question 4:
		- cBullet and cLaser (in Questions 4,5 / Projectiles filter) control the behavior for each projectile
		- cAITank decides when to fire based on a response from the mediator.
	- Question 5:
		- cPlayerTank fires the same projectiles based on a message from the mediator triggered in InputHandling.
	- Question 6:
		- cPlayerTank and cAITank track their own life totals and death states.
		
- The maze .txt file is included in the Project directory (hopefully that is acceptable).
- WASD controls your tank's movement (you can hold down the keys, you don't need to repeatedly press them). 1 fires bullets and 2 fires the laser. You can use either the num keys or the keypad.
- Space bar activates the AI tanks (if they aren't already awake)
- The camera can be zoomed out/in using Q/E keys. You can't zoom closer than the starting distance, but you can zoom out to see more of the board/observe AI behaviour.

-INFO6044MidtermPt2.mkv covers Questions 7
- Questions 7 have been answered in the solution contained in the INFO6044Midterm_Q7 folder.
- Question 7 has its own copy of the solution featuring the SuperTank (with an appropriate filter)
- The scene has been modified to more visibly show the super tank in action.
- The mediator has been updated with a few additional messages related to the SuperTank (though some of these areas have not been extensively tested).
- There are still a few rough patches around the cleanup of the old tank and it's projectiles upon promoting, but hopefully this captures the basic ideas of inheritance.
