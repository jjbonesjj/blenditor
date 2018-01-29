#pragma once

#undef _DEBUG
#include <Python.h>
#define _DEBUG

static PyObject* hello_world(PyObject* self, PyObject* args);


PyMODINIT_FUNC PyInit_Convexify();