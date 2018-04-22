#pragma once
#include "convexify.h"
#include <list>
#include "io.h"

namespace Cy
{
	C_Polyhedron makePolyhedron(Array<BVertex> vertices, Array<BFace> polygons);
	Mesh polyhedraListToMesh(std::list<C_Polyhedron> polyhedra);

	struct BlenderData
	{
		bool valid;
		Array<BMaterial> materials;
		Array<BMesh> meshes;
		Array<BObject> objects;
		Array<BTexture> textures;
	};

	BlenderData extractData(PyObject* blenderData);
}
