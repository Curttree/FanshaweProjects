# INFO 6017 - Artifical Intelligence - Project 2
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Unity, open the project and press play.

## Running the Project
- Controls (All number keys should support keypad equivalents):
    - 1 = Circle Formation
    - 2 = V Formation
    - 3 = Square Formation
    - 4 = Line Formation
    - 5 = Two Rows Formation
    - 6 = Enable Flocking
    - 7 = Disable Flocking and return to previous formation (if one has been set)
    - 8 = Enable path following (note: a formation must be set before this is supported, otherwise objects will start path following once a formation is set)
    - 9 = Reverse path direction (objects will still continue towards current waypoint, but then reverse direction afterwards)
    - 0 = Stop path following and return to previous formations default orientation
    - + = Enable flocking + path following (requires some formation to have been previously set in order to path follow)
    - - = Disable flocking + path following

- Flocking controls (only supported when flocking has been enabled)
    - Z = Increase Cohesion
    - X = Decrease Cohesion
    - C = Increase Separation
    - V = Decrease Separation
    - B = Increase Alignment
    - N = Decrease Alignment