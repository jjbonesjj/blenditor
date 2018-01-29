#include "py_debug.h"

#include "convexify.h"

static PyObject* helloWorld(PyObject* self, PyObject* args)
{
	const char *command;
	int sts;

	// if (!PyArg_ParseTuple(args, "s", &command))
		// return NULL;
	// sts = system(command);
	printf("hello convexified world!");

	return PyLong_FromLong(1);
}

static PyMethodDef convexifyMethods[] = {
	{
		"helloWorld", helloWorld, METH_NOARGS,
		"prints out \"Hello Convexified World\""
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
	printf("importing");
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
		"D:/src/blenditor/source/convexify/",
		"test",
		"test",
	};
	return import_call_execute(numArguments, arguments);
#else
	return import_call_execute(argc, argv);
#endif

}
