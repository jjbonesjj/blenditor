#pragma once

#include "convexify.h"
#include <stdint.h>
#include <iostream>
#include <fstream>

typedef uint32_t u32;
typedef u32 absOffset;
typedef u32 relOffset;

static const int MAGIC_LEN = 16;
static const int MAGIC_LEN_MINOR = 4;
static const char MAGIC[MAGIC_LEN] = "scott.checko";
static const char* CYL_EXTENSION = ".cyl";

#define MAKE_VERSION(major, minor, mini) (major << 24) | (major << 16) | (mini << 0) 
#define REL_PTR(ptr, base) decltype(ptr)((char*)ptr - (char*)base)
#define ABS_PTR(ptr, offset) (((char*)ptr) + offset)

#define BUILD_MAGIC(ptr, value) strncpy(ptr->magic, #value#value#value#value#value , MAGIC_LEN_MINOR)

struct Face
{
	char magic[MAGIC_LEN_MINOR];
	u32 indices[3];
	float normal[3];
};

struct SubMesh
{
	char magic[MAGIC_LEN_MINOR];
	Array<Face> faces;
};

struct Mesh
{
	char magic[MAGIC_LEN_MINOR];
	Array<SubMesh> subMeshes;

	// vertices for all submeshes
	// submeshes are expected to share many faces
	Array<Vertex> vertices;
};

struct Chunk
{
	char magic[MAGIC_LEN_MINOR];
	Array<Mesh> meshes;
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

	Array<Chunk> chunks;

};

void buildCyLevel(Array<Chunk> chunks, char* filePath);