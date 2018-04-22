#pragma once

#undef _DEBUG
#include <Python.h>
#define _DEBUG

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_items_with_id_3.h> 
#include <CGAL/assertions_behaviour.h>

#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt				C_Kernel;

typedef CGAL::Nef_polyhedron_3<C_Kernel>										C_NefPolyhedron;
typedef CGAL::Polyhedron_3<C_Kernel, CGAL::Polyhedron_items_with_id_3>			C_Polyhedron;
typedef C_Kernel::Point_3														CK_Point;
typedef CGAL::Surface_mesh<CK_Point>											C_Mesh;
typedef C_Kernel::Weighted_point_3												C_Weight;

typedef C_Polyhedron::HalfedgeDS												C_Halfedge;
typedef C_Polyhedron::Halfedge													C_HalfedgeNorm;
typedef C_Polyhedron::Facet														C_Facet;

typedef C_NefPolyhedron::Halfedge												C_NefHalfedge;
typedef typename C_Halfedge::Vertex												C_Vertex;
typedef typename C_Vertex														CH_Point;
typedef C_Kernel::Vector_3														C_Vector;

typedef C_Mesh::Vertex_index													C_VertexIndex;
typedef C_Mesh::Face_index														C_FaceIndex;

typedef boost::graph_traits<C_Polyhedron>::vertex_descriptor					C_VertexDescriptor;
typedef boost::graph_traits<C_Polyhedron>::face_descriptor						C_FaceDescriptor;

typedef CGAL::Surface_mesh<CK_Point> Surface_mesh;

namespace Cy
{

	struct BlenderPoint
	{
		double x, y, z;
	};

	template <class T>
	struct Array
	{
		// NOTE: DO NOT CHANGE THE ORDER OF THIS STRUCT
		T* data;
		u64 size;


		T operator[](size_t index)
		{
			return this->data[index];
		}

		T* operator()(size_t index)
		{
			return &this->data[index];
		}
	};

#define FOR(arr, it) u64 it##Index = 0; for(decltype(arr.data) it = arr.data; it < arr.data + arr.size; it++, it##Index++)

	struct BFace
	{
		int numVertices;
		int vertices[4];
	};

	struct Vector
	{
		double x, y, z;
	};

	struct BVertex
	{
		Vector coords;
		Vector normal;
		int index;
	};

	struct BMaterial
	{

	};

	struct BMesh
	{
		Array<BFace> faces;
		Array<BVertex> vertices;
	};

	struct BObject
	{

	};

	struct BTexture
	{

	};

	struct Indices
	{
		int vertexIndices = 0;
		int facetsIndices = 0;
		int edgeIndices = 0;
	};

	Indices buildIndices(std::list<C_Polyhedron>& polyhedra);
}

#define EXPORT extern "C" __declspec(dllexport)

// FOR NOW, this tool will only be used by me, so FOR NOW this is sufficient.
#define Assert(test, message, ...) if(!(test)) { *(int*)0 = 0; }

