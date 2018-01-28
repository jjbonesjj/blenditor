#include "py_debug.h"

#include "convexify.h"

static PyObject* hello_world(PyObject* self, PyObject* args)
{
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = system(command);
	return PyLong_FromLong(sts);
}

static PyMethodDef convexifyMethods[] = {
	{
		"hello_world", hello_world, METH_VARARGS,
		"Print 'hello world' from a method defined in a C extension."
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

int main(int argc, const char* argv[])
{
    /*Py_SetProgramName(argv[0]);

	Py_Initialize();

	PyInit_convexify();*/
	return import_call_execute(argc, argv);
}
