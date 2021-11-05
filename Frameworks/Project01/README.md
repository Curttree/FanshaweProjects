# INFO 6044 - Game Engine Frameworks & Patterns - Project 1
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in either x64 or x86 with either Debug or Release configuration (all options should work).

## Running the Project/ Locating Implementations
- The base of the project was our second Physics assignment (which was in itself built on top of our first Graphics assignment).
- Premise is a simple Fireworks show. Press 1,2,3 (or 4 in the 'After' version of the project) to trigger fireworks.
- Examples of each pattern implementation can be located as follows:
    - Abstract Factory = cFireworkFactory, cFuseFactory (Both located in Fireworks/Factories filter)
    - Builder = cFireworkBuilder (Located in the Fireworks/Builder filter)
    - Mediator = cFireworkMediator, iMediator, iMessage (cFireworkMediator is in Fireworks filter, 
        Interfaces are in the Header filter)
    - Singleton = cWorldSpace, cMathHelper, etc. (A few implementations. These ones are in the Header Files/Source Files filters)
    - PIMPL = cFireworkObject / cFirework_Implementation (Both in Fireworks/Objects filter)
- Descriptions of how/why each pattern is being utilized will be discussed in the attached video.
- After recording the video, I moved the code within the 'MANUAL EXPLOSION' conditional statement to its own method within cFireworkMediator to better comply with the project description. Since this was basically just a copy/paste and doesn't change the actual details of the mediator implementation, I didn't re-record it in the video (hopefully this is still easy enough to follow where the change was made).
- Also noticed that my video glosses over the new 'Manual' Fuse object I created that is referenced towards the end of the video. It also follows the factory pattern (similar to the firework itself), so it seemed redundant to include an explanation of that pattern twice.