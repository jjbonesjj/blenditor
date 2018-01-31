#include "py_debug.h"

#include "convexify.h"

static PyObject* helloWorld(PyObject* self, PyObject* args)
{
	printf("printf: hello convexified world!");

	return PyUnicode_FromString("return: hello convexified world!");
}

static PyObject* convexifyMesh(PyObject* self, PyObject* list)
{
	// char* meshData;
	// int size;
	
	if (PyList_Check(list))
	{
		for (int i = 0; i < PyList_GET_SIZE(list); i++)
		{
			static char* keywordList[] = { "co", "x", nullptr };
			float co[4] = {};

			PyObject* vertexObj = PyList_GetItem(list, i);
			// if(!PyArg_ParseTupleAndKeywords(vertexObj, )
		}

		return PyUnicode_FromString("success");
	}
	

	return PyUnicode_FromString("failure");
}

struct Point
{
	float x;
	float y;
	float z;
};

extern "C" __declspec(dllexport) int paramless()
{
	return 12345;
}

extern "C" __declspec(dllexport) int piper(int piped)
{
	return piped;
}

extern "C" __declspec(dllexport) char* passArray(int* arr, int size)
{
	if (arr)
	{
		arr[size - 1] = 0;
		char* bleh = (char*)arr;
		return bleh;
	}
	return nullptr;
}

extern "C" __declspec(dllexport) Point sumPointArray(Point* arr, int size)
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
