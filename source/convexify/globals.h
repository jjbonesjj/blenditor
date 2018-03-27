#pragma once


const bool VERBOSE_LOGGING = false;

// **********************************
// WARNING: if triangulation is off, 
// you better make sure your faces 
// are perfectly planar. (THERE IS NO 
// LEEWAY IN CGAL NEF POLYHEDRON 
// CREATION) 
//***********************************
const bool TRIANGULATE_MESHES = true;

#define DEBUG_WITH_MAGIC true