#include "py_debug.h"

#include "convexify.h"
#include <CGAL/CORE_Expr.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Delaunay_triangulation_2.h>

static PyObject* helloWorld(PyObject* self, PyObject* args)
{
	printf("printf: hello convexified world!");

	return PyUnicode_FromString("return: hello convexified world!");
}



EXPORT bool convexifyMesh(Point* vertexPoints, int vertexPointsSize)
{
	
	if (vertexPoints)
	{
		for (int i = 0; i < vertexPointsSize; i++)
		{
			
		}

		return true;
	}

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
	/*{
		"convexifyMesh", convexifyMesh, METH_VARARGS,
		"attempts to conexify a mesh"
	},*/
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


typedef CORE::Expr Real;
typedef CGAL::Simple_cartesian<Real> K;
typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef K::Point_2 Point_2;

int main(int argc, const char** argv)
{
	DT dt;
	double two = 2;
	Point_2 p(0, 0), q(std::sqrt(two), 1), r(0, 1);

	dt.insert(p);
	dt.insert(q);
	dt.insert(r);

	std::cout << dt << std::endl;

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
