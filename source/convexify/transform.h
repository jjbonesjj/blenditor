#pragma once
#include "convexify.h"

C_NefPolyhedron make_nef(Array<Vertex> vertices, Array<Polygon> polygons);


struct BlenderData
{
	bool valid;
	Array<Material> materials;
	Array<Mesh> meshes;
	Array<Object> objects;
	Array<Texture> textures;
};

BlenderData extractData(PyObject* blenderData);
