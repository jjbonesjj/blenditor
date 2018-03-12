#include "py_debug.h"

#include "convexify.h"
#include "transform.h"

#include <list>

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

		size_t listSize = PyObject_Length(list);

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

static PyObject* convexifyMesh(PyObject* self, PyObject* args)
{


	printf("STARTING CONVEXIFY\n");
	
	BlenderData data = extractData(args);

	make_nef(data.meshes[0].vertices, data.meshes[0].polygons);

	/*printf("there are %i vertexes\n", vertexPointsSize);
	
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

		NefPolyhedron nef(points, points + vertexPointsSize, NefPolyhedron::Points_tag());

		CGAL::convex_decomposition_3(nef);
		std::list<Polyhedron> convex_parts;

		// the first volume is the outer volume, which is 
		// ignored in the decomposition
		typedef NefPolyhedron::Volume_const_iterator Volume_const_iterator;
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
	}*/

	if (data.valid)
	{
		printf("SUCCESS CONVEXIFY\n");
		return PyUnicode_FromString("argsuccess: return: hello arg'd world!");
	}
	else
	{
		printf("ERROR CONVEXIFY\n");
		return PyUnicode_FromString("argfail: return: hello arg'd world!");
	}



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
