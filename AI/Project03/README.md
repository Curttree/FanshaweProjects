# INFO 6017 - Artificial Intelligence - Project 3
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- Map can be changed in the cMapManager.cpp file within the Init() method (ln. 86), or renaming the bmp files within the Project03 folder. Right now it is defaulted to resourceMap.bmp.
- Maps 1 and 2 work correctly, though I ran into issues getting map 3 to load properly (see video for demonstration). I'm guessing I am not handling the width/height correctly, though there could be other issues.
- I added a resourceMap4.bmp to demonstrate behaviour of multiple gatherers since map 3 is not loading properly.
- I know at the very least my current edge creation code would be faulty if width and height aren't equal, however since I was not even able to get the code to properly load a map of different proportions, this has been left 'as is' so it will at least work properly for maps 1 and 2. Once image loading is corrected, other areas of the code such as camera placement would also need to be updated.