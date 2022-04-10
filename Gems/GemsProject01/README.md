# INFO 6023 - Gems - Project 1
## By: Curtis Tremblay (Student Number: 1049697)


### Build Instructions
- In Visual Studio, build project in x64 with either Debug or Release configuration (both options should work).
## Running the Project / Notes

- There is a known issue regarding reallocating space for the maps. Guessing it is due to a misuse of pointers on my part, but I wasn't able to land on the exact issue. I've increased the default sizes of each map to account for this, but if your data set is very large, you may recieve errors. I've tested up to 1000 users though and it seems to work, though similar SIN numbers being genereated could cause reallocation sooner than I anticipated.

- My containers (A templated Array and Linked List, and 2 variations of a Hash Map) can be found in the 'Custom Containers' filter

- My modified version of your provided code is included in the 'Provided Files' filter

- Sorting is handled at time of insert. Could be further optimized with Red/Black Tree structure or handling insert into sorted list as async (unsorted lists/numerical sort are handled via maps, so this data should scale a lot better)

- Video is a little long, but last half is basically just walking through the code in detail (along with my sample run to verify it at least works with some data set) in case it does not run with your new solution. The first few minutes should at least point you in the direction of my containers/other relevant code.
