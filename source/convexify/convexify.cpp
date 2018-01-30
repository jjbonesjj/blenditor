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

struct Vertex
{
	float a[3];
};

extern "C" static int convexifyMeshRaw()
{
	return 12345;
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
