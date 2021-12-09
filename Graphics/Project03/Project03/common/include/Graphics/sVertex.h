#pragma once

// This structure matches the PLY file format
struct sVertex
{
    float x, y, z;      // , c, i;
    float nx, ny, nz;
    float red, green, blue, alpha;  // (Note the file is in HTML style, so 0 to 255, but the shader wants 0.0 to 1.0)
    float u, v;
};