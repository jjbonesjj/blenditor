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
#define REL_PTR(base, ptr) decltype(ptr)((char*)base - (char*)ptr)
#define ABS_PTR(ptr, offset) (((char*)ptr) + offset)

struct Face
{
	u32 indices[3];
	float normal[3];
};

struct SubMesh
{
	u32 numFaces;
	Face* faces;
};

struct Mesh
{
	u32 numSubMeshes;
	SubMesh* subMeshes;

	// vertices for all submeshes
	// submeshes are expected to share many faces
	u32 numVertices;
	Vertex* vertices;
};

struct Chunk
{
	u32 numMeshes;
	Mesh* meshes;
};

enum NumberType
{
	FixedPoint,
	FloatingPoint
};

struct CylHeader
{
	char magic[MAGIC_LEN];
	u32 version;

	NumberType numberType;
	union
	{
		struct // fixed point specific
		{
			u32 fixedPointNumFractionalBits;
		};
		struct // floating point specific
		{

		};
	};


	u32 numChunks;
	Chunk* chunks;

};

void buildCyLevel(Array<Chunk> chunks, char* filePath);