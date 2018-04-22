#pragma once
#include "convexify.h"


namespace Cy
{
	C_Polyhedron makePolyhedron(Array<Vertex> vertices, Array<Polygon> polygons);
	Mesh polyhedraListToMesh(std::list<C_Polyhedron> polyhedra);

	struct BlenderData
	{
		bool valid;
		Array<Material> materials;
		Array<BlenderMesh> meshes;
		Array<Object> objects;
		Array<Texture> textures;
	};

	BlenderData extractData(PyObject* blenderData);
}
