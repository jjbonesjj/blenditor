#pragma once

#undef _DEBUG
#include <Python.h>
#define _DEBUG

// MUST MATCH PYTHON
struct Point
{
	float x;
	float y;
	float z;
};

#define EXPORT extern "C" __declspec(dllexport)