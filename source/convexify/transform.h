#pragma once
#include "convexify.h"

NefPolyhedron make_nef(Vertex* vertices, int numVertices, Polygon* polygons, int numFaces);


struct BlenderData
{
	bool valid;
	Array<Material> materials;
	Array<Mesh> meshes;
	Array<Object> objects;
	Array<Texture> textures;
};

BlenderData extractData(PyObject* blenderData);
