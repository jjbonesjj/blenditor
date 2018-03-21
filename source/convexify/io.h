#pragma once

#include "convexify.h"
#include <stdint.h>
#include <iostream>
#include <fstream>

typedef uint32_t u32;
typedef u32 absOffset;
typedef u32 relOffset;

static const int MAGIC_LEN = 16;
static const char MAGIC[MAGIC_LEN] = "scott.checko";
static const char* CYL_EXTENSION = ".cyl";

#define MAKE_VERSION(major, minor, mini) (major << 24) | (major << 16) | (mini << 0) 
#define OFFSET(base, ptr) (((char*)ptr) + base)

struct SubMesh
{
	u32 numVertices;
	Vertex* vertices;
};

struct GroundMesh
{
	u32 numSubMeshes;
	SubMesh* subMeshes;
};

struct Chunk
{
	u32 numGroundMeshes;
	GroundMesh* meshes;
};

struct CylHeader
{
	char magic[MAGIC_LEN];
	u32 version;

	u32 numChunks;
	Chunk* chunks;

};

void buildCyLevel(C_NefPolyhedron nef, char* filePath);