#pragma once

#undef _DEBUG
#include <Python.h>
#define _DEBUG

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel C_Kernel;
typedef C_Kernel::Point_3 C_Point3;

typedef CGAL::Nef_polyhedron_3<C_Kernel>		C_NefPolyhedron;
typedef CGAL::Polyhedron_3<C_Kernel>			C_Polyhedron;
typedef CGAL::Surface_mesh<C_Point3>			C_Mesh;

typedef C_Polyhedron::HalfedgeDS				C_Halfedge;
typedef C_NefPolyhedron::Halfedge				C_NefHalfedge;
typedef typename C_Halfedge::Vertex				C_Vertex;
typedef typename C_Vertex::Point				C_Point;

typedef C_Mesh::Vertex_index C_VertexIndex;
typedef C_Mesh::Face_index C_FaceIndex;

struct Point
{
	double x, y, z;
};

template <class T>
struct Array
{
	int size;
	T* data;

	T operator[](size_t index)
	{
		return this->data[index];
	}
};

struct Polygon
{
	int numVertices;
	int vertices[4];
};

struct Vector
{
	double x, y, z;
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

// FOR NOW, this tool will only be used by me, so FOR NOW this is sufficient.
#define Assert(test, message, ...) if(!test) { *(int*)0 = 0; }