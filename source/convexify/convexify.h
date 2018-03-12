#pragma once

#undef _DEBUG
#include <Python.h>
#define _DEBUG

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/convex_decomposition_3.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;

typedef CGAL::Nef_polyhedron_3<Kernel>		NefPolyhedron;
typedef CGAL::Polyhedron_3<Kernel>			Polyhedron;

typedef Polyhedron::HalfedgeDS				Halfedge;
typedef NefPolyhedron::Halfedge				NefHalfedge;

// MUST MATCH PYTHON
struct Point
{
	float x;
	float y;
	float z;
};

template <class T>
struct Array
{
	int size;
	T* data;
};

struct Polygon
{
	int numVertices;
	int vertices[4];
};

struct Vector
{
	float x, y, z;
};

struct Vertex
{
	Vector coords;
	Vector normal;
	int index;
};

struct Material
{

};

struct Mesh
{
	Array<Polygon> polygons;
	Array<Vertex> vertices;
};

struct Object
{

};

struct Texture
{

};

#define EXPORT extern "C" __declspec(dllexport)