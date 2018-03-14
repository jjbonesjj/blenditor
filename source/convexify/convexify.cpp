#include "py_debug.h"

#include "convexify.h"
#include "transform.h"

#include <list>

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

	Vertex verts[8] = 
	{
		{ 1.0f,		1.0f,	-1.0f	},
		{ 1.0f,		-1.0f,	-1.0f	},
		{ -1.0f,	-1.0f,	-1.0f	},
		{ -1.0f,	1.0f,	-1.0f	},
		{ 1.0f,		1.0f,	1.0f	},
		{ 1.0f,		-1.0f,	1.0f	},
		{ -1.0f,	-1.0f,	1.0f	},
		{ -1.0f,	1.0f,	1.0f	},
	};

	Array<Polygon> faces = { 6, polys };
	Array<Vertex> vertices = { 8, verts };

	C_NefPolyhedron nef = make_nef(vertices, faces);

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

	printf("data is %p\n", data.meshes.data);

	for (int i = 0; i < data.meshes[0].polygons.size; i++)
	{
#define lazy(num) data.meshes[0].polygons[i].vertices[num]
		printf("polygon: %i %i %i %i\n", lazy(0), lazy(1), lazy(2), lazy(3));
	}

	for (int i = 0; i < data.meshes[0].vertices.size; i++)
	{
#define lazy2 data.meshes[0].vertices[i].coords
		printf("vertex: %f %f %f\n", lazy2.x, lazy2.y, lazy2.z);
	}

	C_NefPolyhedron nef = make_nef(data.meshes[0].vertices, data.meshes[0].polygons);

	//CGAL::convex_decomposition_3(nef);
	std::list<C_Polyhedron> convex_parts;

	// the first volume is the outer volume, which is 
	// ignored in the decomposition
	/*typedef NefPolyhedron::Volume_const_iterator Volume_const_iterator;
	Volume_const_iterator ci = ++nef.volumes_begin();
	for (; ci != nef.volumes_end(); ++ci) {
		if (ci->mark()) {
			Polyhedron P;
			nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);
			convex_parts.push_back(P);
		}
	}
	std::cout << "decomposition into " << convex_parts.size() << " convex parts " << std::endl;
		*/
	printf("END CONVEXIFY\n");

	//if (nef.is_convex())
	{
		printf("SUCCESS CONVEXIFY\n");
	}
	//else
	{
		printf("ERROR CONVEXIFY\n");
	}

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

EXPORT Point sumPointArray(Point* arr, int size)
{
	Point sum = {};
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
	"convexify", /* name of module */
	"",          /* module documentation, may be NULL */
	-1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
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
