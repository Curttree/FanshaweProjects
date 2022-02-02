# INFO 6045 - Animation - Project 1
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 with either Debug or Release configuration (both options should work).

## Running the Project
- Controls:
    - Right Arrow Key: Next Sequence
    - Left Arrow Key: Previous Sequence
    - 1-5 Animation Speed (1x - 5x)
    - Space: Pause/Play active sequence
    - C: Toggle color coding
    - R: Reverse Play
    - M: Mute/un-mute audio

- All 12 keyframe updates variations should be present. Since they can pass by quickly, I've included a brief summary of the animation with annotations for each occurence (color coding/referencing cGameEngine::LoadAnimationAssignmentOneScene will confirm these occurences):
    1 - Position - No Easing
    2 - Position - Ease In
    3 - Position - Ease Out
    4 - Position - Ease In/Out
    5 - Scaling - No Easing
    6 - Scaling - Ease In
    7 - Scaling - Ease Out
    8 - Scaling - Ease In/Out
    9 - Rotation - No Easing
    10 - Rotation - Ease In
    11 - Rotation - Ease Out
    12 - Rotation - Ease In/Out

    Skeleton walking down path (3), when it passes bushes they rustle(9). 

    Skeleton scales down slightly to show they are suspicious (8), turns (12) and walks towards bush (4).

    When they are close enough, the bush rustles again (10).

    Excalamation mark imposter is shown (7). Skeleton scales up and then down to show they are scared (5).

    Skeleton attempts to back away slowly (2), turns (11) and attempts to run away. Blood splatter effect moves towards camera(1) and scales up (6).
