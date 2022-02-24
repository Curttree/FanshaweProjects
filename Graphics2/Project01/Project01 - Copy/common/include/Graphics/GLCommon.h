#pragma once

// These have to be included in a certain order
//	or the build fails (it's a thing with GFLW)

#include <extern/glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <extern/GLFW/glfw3.h>
