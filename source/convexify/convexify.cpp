#include "py_debug.h"

#include "convexify.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_indexed_items.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Exact_integer.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <list>

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;

typedef CGAL::Nef_polyhedron_3<Kernel, CGAL::SNC_indexed_items> Nef_polyhedron;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
// typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

typedef Kernel::Point_3 Point_3;
typedef Nef_polyhedron::Volume_const_iterator Volume_const_iterator;

static PyObject* helloWorld(PyObject* self, PyObject* args)
{
	printf("printf: hello convexified world!");

	return PyUnicode_FromString("return: hello convexified world!");
}

static PyObject* helloArgs(PyObject* self, PyObject* args)
{
	PyObject* list = nullptr;
	if (PyArg_ParseTuple(args, "O", &list))
	{
		if (!PySequence_Check(list)) {
			PyErr_SetString(PyExc_TypeError, "expected sequence");
			return NULL;
		}

		int listSize = PyObject_Length(list);

		int* intArray = (int *)malloc(sizeof(int)*listSize);
		for (int index = 0; index < listSize; index++)
		{
			/* get the element from the list/tuple */
			PyObject* item = PySequence_GetItem(list, index);
			/* we should check that item != NULL here */
			/* make sure that it is a Python integer */
			if (!PyLong_Check(item)) 
			{
				free(intArray);  /* free up the memory before leaving */
				PyErr_SetString(PyExc_TypeError, "expected sequence of integers");
				return NULL;
			}
			/* assign to the C array */
			intArray[index] = PyLong_AsLong(item);
		}

		printf("arg success: printf: hello arg'ed world!");
		return PyUnicode_FromString("arg success: return: hello arg'd world!");
	}

	printf("argfail: printf: hello arg'ed world!");
	return PyUnicode_FromString("argfail: return: hello arg'd world!");
	
}

struct Vertex
{
	float x, y, z;
};

struct Face
{
	int indices[3];
};

template <class HDS>
class Build_Polyhedron : public CGAL::Modifier_base<HDS> {
public:

	Vertex * vertices;
	int numVertices;
	Face* faces;
	int numFaces;

	Build_Polyhedron(Vertex* vertices, int numVertices, Face* faces, int numFaces)
	{
		this->vertices = vertices;
		this->numVertices = numVertices;
		this->faces = faces;
		this->numFaces = numFaces;
	}
	void operator()(HDS& hds) {
		// Postcondition: hds is a valid polyhedral surface.
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
		typedef typename HDS::Vertex   Vertex;
		typedef typename Vertex::Point Point;

		B.begin_surface(this->numVertices, 0, this->numFaces);

		for (int i = 0; i < numVertices; i++)
		{
			B.add_vertex(Point(vertices[i].x, vertices[i].y, vertices[i].z));
		}

		for (int i = 0; i < numFaces; i++)
		{
			B.begin_facet();
			B.add_vertex_to_facet(this->faces[i].indices[0]);
			B.add_vertex_to_facet(this->faces[i].indices[1]);
			B.add_vertex_to_facet(this->faces[i].indices[2]);
			B.end_facet();
		}

		B.end_surface();
	}
};

Nef_polyhedron make_nef(Vertex* vertices, int numVertices, Face* faces, int numFaces)
{
	typedef CGAL::Simple_cartesian<double>     Kernel;
	typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
	typedef Polyhedron::HalfedgeDS             HalfedgeDS;
	
	Polyhedron poly;
	Build_Polyhedron<HalfedgeDS> build_poly(vertices, numVertices, faces, numFaces);
	poly.delegate(build_poly);
	CGAL_assertion(poly.is_closed());

	return Nef_polyhedron();
	
}

EXPORT bool convexifyMesh(Point* vertexPoints, int vertexPointsSize)
{

	Vertex verts[4] = 
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f }
	};

	Face faces[4] = 
	{
		{ 0, 1, 2 },
		{ 0, 1, 3 },
		{ 0, 2, 3 },
		{ 1, 2, 3 },
	};

	make_nef(verts, 4, faces, 4);

	printf("STARTING CONVEXIFY\n");
	printf("there are %i vertexes\n", vertexPointsSize);
	
	if (vertexPoints)
	{
		Point_3* points = new Point_3[vertexPointsSize];
		for (int i = 0; i < vertexPointsSize; i++)
		{
			points[i] = Point_3(vertexPoints[i].x,
								vertexPoints[i].y,
								vertexPoints[i].z);
			printf("{ %f, %f, %f }", vertexPoints[i].x, vertexPoints[i].y, vertexPoints[i].z);
			//printf("{ %f, %f, %f }\n", points[i].x, points[i].y, points[i].z);
		}
		printf("\n\n");

		Nef_polyhedron nef(points, points + vertexPointsSize, Nef_polyhedron::Points_tag());

		CGAL::convex_decomposition_3(nef);
		std::list<Polyhedron> convex_parts;

		// the first volume is the outer volume, which is 
		// ignored in the decomposition
		Volume_const_iterator ci = ++nef.volumes_begin();
		for (; ci != nef.volumes_end(); ++ci) {
			if (ci->mark()) {
				Polyhedron P;
				nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);
				convex_parts.push_back(P);
			}
		}
		std::cout << "decomposition into " << convex_parts.size() << " convex parts " << std::endl;
		
		printf("END CONVEXIFY\n");
		delete[] points;
		return true;
	}


	printf("ERROR CONVEXIFY\n");
	return false;
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
		"helloWorld", helloWorld, METH_NOARGS,
		"prints out \"Hello Convexified World\""
	},
	{
		"helloArgs", helloArgs, METH_VARARGS,
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
	Point points[8] = {
		{ 1.0f, 1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f }
	};
	convexifyMesh(points, 8);

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
