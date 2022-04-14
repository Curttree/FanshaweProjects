# INFO 6017 - Artificial Intelligence - Project 4
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- Map can be changed in the cMapManager.cpp file within the Init() method (ln. 86), or renaming the bmp files within the Project03 folder. Right now it is defaulted to resourceMap.bmp.
- New code is in Project04Entities filter. cConductor handles collision detection, cSensor uses the detected collisions to provide input for the fuzzy logic, cVehicle determines speed and steering behaviours based on input from the sensors.
- Steering logic is biased to left hand turns, so objects may start to cluster towards the bottom left of the map if the simulation is run for long enough at low speeds. This could be offset by randomly deciding whether to check the left or right sensors first instead of always checking for objects on the left before ones on the right. Increasing the speed can also offset this behaviour as collisions are less likely to occur in one given portion of the map (see video).