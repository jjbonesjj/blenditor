#include "transform.h"
#include "convexify.h"

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>

template <class HDS>
class Build_Polyhedron : public CGAL::Modifier_base<HDS> {
public:

	Vertex * vertices;
	int numVertices;
	Polygon* polygons;
	int numFaces;

	Build_Polyhedron(Vertex* vertices, int numVertices, Polygon* polygons, int numFaces)
	{
		this->vertices = vertices;
		this->numVertices = numVertices;
		this->polygons = polygons;
		this->numFaces = numFaces;
	}
	void operator()(HDS& hds) {
		// Postcondition: hds is a valid polyhedral surface.
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
		typedef typename HDS::Vertex   Vertex;
		typedef typename Vertex::Point Point;

		B.begin_surface(this->numVertices, 0, this->numFaces);

		for (int i = 0; i < numVertices; i++)
		{
			B.add_vertex(Point(vertices[i].x, vertices[i].y, vertices[i].z));
		}

		for (int i = 0; i < numFaces; i++)
		{
			B.begin_facet();
			B.add_vertex_to_facet(this->polygons[i].vertices[0]);
			B.add_vertex_to_facet(this->polygons[i].vertices[1]);
			B.add_vertex_to_facet(this->polygons[i].vertices[2]);
			B.end_facet();
		}

		B.end_surface();
	}
};

NefPolyhedron make_nef(Vertex* vertices, int numVertices, Polygon* faces, int numFaces)
{

	Polyhedron poly;
	Build_Polyhedron<Halfedge> buildPoly(vertices, numVertices, faces, numFaces);
	poly.delegate(buildPoly);
	CGAL_assertion(poly.is_closed());

	return NefPolyhedron();
}


#include "Python.h"

#define GET_ATTR(object, name) PyObject* name = nullptr; if(PyObject_HasAttrString(object, #name)) { name = PyObject_GetAttrString(object, #name); } else { printf("failed to extract "#name"\n"); exit(0); }
#define PRINT_PY(object) { PyObject* objectPath = PyObject_Repr(object); const char* s = PyUnicode_AsUTF8(objectPath); const char* type = object->ob_type->tp_name; printf("%s : %s\n", type,  s); }


Array<Vertex> extractVertexArray(PyObject* vertexList)
{
	Array<Vertex> result = {};
	result.size = PyObject_Length(vertexList);

	result.data = (Vertex*)malloc(sizeof(Vertex)*result.size);
	for (int index = 0; index < result.size; index++)
	{

		PyObject* item = PySequence_GetItem(vertexList, index);

		Vertex vertex = {};

		GET_ATTR(item, vertices);

		//PyArg_ParseTuple(vertices, "fff", &vertex.x, &vertex.y, &vertex.z);


		//PyArg_ParseTuple(vertices, "O", &);
		result.data[index] = vertex;

		printf("vertex: %f %f %f\n", vertex.x, vertex.y, vertex.z);

	}
	return result;
}

Array<Polygon> extractPolygonArray(PyObject* facesList)
{
	Array<Polygon> result = {};
	result.size = PyObject_Length(facesList);

	result.data = (Polygon*)malloc(sizeof(Vertex)*result.size);
	for (int index = 0; index < result.size; index++)
	{
		PyObject* item = PySequence_GetItem(facesList, index);

		Polygon polygon = {};
		GET_ATTR(item, vertices);

		// PRINT_PY(vertices);

		polygon.numVertices = PyObject_Length(vertices);

		for (int j = 0; j < polygon.numVertices; j++)
		{
			PyObject* vertexIndex = PySequence_GetItem(vertices, j);
			PRINT_PY(vertexIndex);
			polygon.vertices[j] = PyLong_AsLong(vertexIndex);
		}

		result.data[index] = polygon;

	}
	return result;
}



Array<Mesh> extractMeshArray(PyObject* meshList)
{
	Array<Mesh> result = {};
	result.size = PyObject_Length(meshList);

	result.data = (Mesh *)malloc(sizeof(Vertex)*result.size);
	for (int index = 0; index < result.size; index++)
	{
			
		PyObject* item = PySequence_GetItem(meshList, index);

		Mesh mesh = {};

		GET_ATTR(item, polygons);
		GET_ATTR(item, vertices);
	
		printf("extract mesh array %i\n", index);
		mesh.polygons = extractPolygonArray(polygons);
		//mesh.vertices = extractVertexArray(vertices);
		printf("extract mesh array2 %i\n", index);
		result.data[index] = mesh;
	}
	return result;
}

BlenderData extractData(PyObject* args)
{

	BlenderData internal = {};


	PyObject* blenderData = nullptr;
	PyObject* blenderContext = nullptr;

	if (PyArg_ParseTuple(args, "OO", &blenderData, &blenderContext))
	{
		printf("successfully resolved args\n");
		internal.valid = true;

		GET_ATTR(blenderData, meshes);

		internal.meshes = extractMeshArray(meshes);
	}

	printf("argfail: printf: hello arg'ed world!\n");

	return internal;

}