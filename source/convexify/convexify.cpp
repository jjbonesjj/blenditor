#include "py_debug.h"

#include "convexify.h"
#include "transform.h"
#include "io.h"
#include <CGAL/Surface_mesh.h>
#include <boost/property_map/property_map.hpp>
#include <CGAL/boost/graph/properties.h>
#include <list>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

using namespace Cy;

Array<Chunk> convexify(BlenderData blenderData)
{
	Array<Chunk> chunks = {};
	chunks.size = 1;
	chunks.data = (Chunk*)malloc(sizeof(Chunk));

	Chunk* chunk = chunks.data;
	chunk->meshes.size = blenderData.meshes.size;
	chunk->meshes.data = (Mesh*)malloc(chunk->meshes.size * sizeof(Mesh));

	FOR(blenderData.meshes, blenderMesh)
	{
		Array<Vertex> vertices = blenderMesh->vertices;
		Array<Polygon> faces = blenderMesh->polygons;

		for (int i = 0; i < faces.size; i++)
		{
#define lazy(num) faces[i].vertices[num]
			printf("polygon: %i %i %i %i\n", lazy(0), lazy(1), lazy(2), lazy(3));
		}

		for (int i = 0; i < vertices.size; i++)
		{
#define lazy2 vertices[i].coords
			printf("vertex: %.10f %.10f %.10f\n", lazy2.x, lazy2.y, lazy2.z);
		}


		C_Polyhedron poly = makePolyhedron(vertices, faces);

		auto nef = C_NefPolyhedron(poly);
		printf("hello14\n");
		Assert(nef.is_valid(VERBOSE_LOGGING));
		printf("hello15\n");


		printf("hello16\n");
		CGAL::convex_decomposition_3(nef);
		std::list<C_Polyhedron> convex_parts;
		printf("hello17\n");
		// the first volume is the outer volume, which is 
		// ignored in the decomposition
		typedef C_NefPolyhedron::Volume_const_iterator Volume_const_iterator;
		Volume_const_iterator ci = ++nef.volumes_begin();
		for (; ci != nef.volumes_end(); ++ci) {
			if (ci->mark()) {
				C_Polyhedron P;
				printf("hello21\n");
				nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);
				printf("hello22\n");
				convex_parts.push_back(P);
			}
		}
		printf("decomp into %zi convex parts\n", convex_parts.size());

		printf("END CONVEXIFY\n");

		// initialize the indices
		int vertexIndices = 0;
		int facetsIndices = 0;
		int edgeIndices = 0;

		for (auto it = convex_parts.begin(); it != convex_parts.end(); it++)
		{
			if (TRIANGULATE_MESHES)
			{
				printf("hello18\n");
				Assert(CGAL::Polygon_mesh_processing::triangulate_faces(*it));
			}
			for (C_Polyhedron::Vertex_iterator jt = it->vertices_begin(); jt != it->vertices_end(); jt++)
			{
				jt->id() = vertexIndices++;
			}

			for (C_Polyhedron::Facet_iterator jt = it->facets_begin(); jt != it->facets_end(); jt++)
			{
				jt->id() = facetsIndices++;
			}

			for (C_Polyhedron::Edge_iterator jt = it->edges_begin(); jt != it->edges_end(); jt++)
			{
				jt->id() = edgeIndices++;
			}
		}

		Mesh mesh = polyhedraListToMesh(convex_parts);

		chunk->meshes[blenderMeshIndex] = mesh;
	}

	buildCyLevel(chunks, "level1");

	return chunks;
}

static PyObject* test(PyObject* self, PyObject* args)
{
	Polygon polys[6] = 
	{	// NOTE: HACKY: first column is num indices
		{ 4, 0, 1, 2, 3 },
		{ 4, 4, 7, 6, 5 },
		{ 4, 0, 4, 5, 1 },
		{ 4, 1, 5, 6, 2 },
		{ 4, 2, 6, 7, 3 },
		{ 4, 4, 0, 3, 7 }
	};
	const float half = 0.5f;
#if 0
	Vertex verts[8] = 
	{
		{ half,		half,	-half	},
		{ half,		-half,	-half	},
		{ -half,	-half,	-half	},
		{ -half,	half,	-half	},
		{ half,		half,	half	},
		{ half,		-half,	half	},
		{ -half,	-half,	half	},
		{ -half,	half,	half	},
	};
#else
	Vertex verts[8] =
	{
		{	1.0000000000, 0.9999999404, - 1.0000000000	},
		{	1.0000000000, - 1.0000000000, - 1.0000000000 },
		{	-1.0000001192, - 0.9999998212, - 1.0000000000 },
		{	-0.9999996424, 1.0000003576, - 1.0000000000 },
		{	1.0000004768, 0.9999994636, 1.0000000000 },
		{	0.9999993443, - 1.0000005960, 1.0000000000 },
		{	-1.0000003576, - 0.9999996424, 1.0000000000 },
		{	-0.9999999404, 1.0000000000, 1.0000000000 }
	};
#endif
	Array<Polygon> faces = { polys, 6 };
	Array<Vertex> vertices = { verts , 8 };

	Mesh mesh = convexify(vertices, faces);

	return PyUnicode_FromString("done");	
}

static PyObject* convexifyMesh(PyObject* self, PyObject* args)
{
	printf("STARTING CONVEXIFY\n");
	
	BlenderData data = extractData(args);

	if (data.valid)
	{
		printf("SUCCESS DATA\n");
	}
	else
	{
		printf("ERROR DATA\n");
	}
	
	Mesh mesh = convexify(data.meshes[0].vertices, data.meshes[0].polygons);

	return PyUnicode_FromString("argsuccess: return: hello arg'd world!");
}

static PyObject* levelify(PyObject* self, PyObject* args)
{
	printf("STARTING CONVEXIFY\n");

	BlenderData data = extractData(args);

	if (data.valid)
	{
		printf("SUCCESS DATA\n");
	}
	else
	{
		printf("ERROR DATA\n");
	}

	Mesh mesh = convexify(data.meshes[0].vertices, data.meshes[0].polygons);
	
	Chunk chunk = {};
	chunk.meshes.data = &mesh;
	chunk.meshes.size = 1;

	Array<Chunk> chunks = {};
	chunks.size = 1;
	chunks.data = &chunk;

	printf("hello20\n");
	buildCyLevel(chunks, "level1");

	return PyUnicode_FromString("argsuccess: return: hello arg'd world!");
}

EXPORT int paramless()
{
	return 12345;
}

EXPORT int piper(int piped)
{
	return piped;
}

EXPORT char* passArray(int* arr, int size)
{
	if (arr)
	{
		arr[size - 1] = 0;
		char* bleh = (char*)arr;
		return bleh;
	}
	return nullptr;
}

EXPORT BlenderPoint sumPointArray(BlenderPoint* arr, int size)
{
	BlenderPoint sum = {};
	if (arr)
	{
		for (int i = 0; i < size; i++)
		{
			sum.x += arr[i].x;
			sum.y += arr[i].y;
			sum.z += arr[i].z;
		}
	}
	return sum;
}

static PyMethodDef convexifyMethods[] = {
	{
		"test", test, METH_NOARGS,
		"prints out \"Hello Convexified World\""
	},
	{
		"convexifyMesh", convexifyMesh, METH_VARARGS,
		"attempts to conexify a mesh"
	},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef moduleDefinition =
{
	PyModuleDef_HEAD_INIT,
	"convexify", // name of module 
	"",          // module documentation, may be NULL 
	-1,          // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
	convexifyMethods
};

PyMODINIT_FUNC PyInit_convexify()
{
	return PyModule_Create(&moduleDefinition);
}

int main(int argc, const char** argv)
{
	// setup some default arguments, since cmake does not support them.
#ifdef _DEBUG
	int numArguments = 4;
	const char* arguments[] =
	{
		argv[0],
		"./built/",
		"__init__",
		"test",
	};
	return import_call_execute(numArguments, arguments);
#else
	return import_call_execute(argc, argv);
#endif

}
