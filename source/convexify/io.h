#pragma once

#include "convexify.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
#include "globals.h"

typedef uint32_t u32;
typedef u32 absOffset;
typedef u32 relOffset;

typedef uint64_t u64;

static const int MAGIC_LEN = 16;
static const int MAGIC_LEN_MINOR = 4;
static const char MAGIC[MAGIC_LEN] = "scott.checko";
static const char* CYL_EXTENSION = ".cyl";

#define MAKE_VERSION(major, minor, mini) (major << 24) | (major << 16) | (mini << 0) 
#define REL_PTR(ptr, base) decltype(ptr)((char*)ptr - (char*)base)
#define ABS_PTR(ptr, offset) (((char*)ptr) + offset)

#if DEBUG_WITH_MAGIC
#define BUILD_MAGIC(ptr, value) strncpy(ptr->magic, #value#value#value#value#value , MAGIC_LEN_MINOR)
#else
#define BUILD_MAGIC(ptr, value)
#endif

namespace Cy
{
	struct Point
	{
#if DEBUG_WITH_MAGIC
		char magic[MAGIC_LEN_MINOR];
#endif
		union
		{
			int posFixed[3];
			float posFloating[3];
		};
	};

	struct Face
	{
#if DEBUG_WITH_MAGIC
		char magic[MAGIC_LEN_MINOR];
#endif
		u32 indices[3];
		float normal[3];
	};

	struct SubMesh
	{
#if DEBUG_WITH_MAGIC
		char magic[MAGIC_LEN_MINOR];
#endif
		Array<Face> faces;
	};

	struct Mesh
	{
#if DEBUG_WITH_MAGIC
		char magic[MAGIC_LEN_MINOR];
#endif
		Array<SubMesh> subMeshes;

		// vertices for all submeshes
		// submeshes are expected to share many faces
		Array<Point> vertices;
	};

	struct Chunk
	{
#if DEBUG_WITH_MAGIC
		char magic[MAGIC_LEN_MINOR];
#endif
		Array<Mesh> meshes;

		u64 faceCount;
		u64 subMeshCount;
	};

	enum NumberType : bool
	{
		FixedPoint,
		FloatingPoint
	};

	struct CylHeader
	{
		char magic[MAGIC_LEN];
		u32 version;
		bool containsMagic;
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
}