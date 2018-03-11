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
	Face* faces;
	int numFaces;

	Build_Polyhedron(Vertex* vertices, int numVertices, Face* faces, int numFaces)
	{
		this->vertices = vertices;
		this->numVertices = numVertices;
		this->faces = faces;
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
			B.add_vertex_to_facet(this->faces[i].indices[0]);
			B.add_vertex_to_facet(this->faces[i].indices[1]);
			B.add_vertex_to_facet(this->faces[i].indices[2]);
			B.end_facet();
		}

		B.end_surface();
	}
};

NefPolyhedron make_nef(Vertex* vertices, int numVertices, Face* faces, int numFaces)
{

	Polyhedron poly;
	Build_Polyhedron<Halfedge> buildPoly(vertices, numVertices, faces, numFaces);
	poly.delegate(buildPoly);
	CGAL_assertion(poly.is_closed());

	return NefPolyhedron();
}


#include "Python.h"

Array<Mesh> extractMeshArray(PyObject* vertexList)
{
	Array<Mesh> result = {};
	result.size = PyObject_Length(vertexList);

	result.data = (Mesh *)malloc(sizeof(Vertex)*result.size);
	for (int index = 0; index < result.size; index++)
	{
			
		PyObject* item = PySequence_GetItem(vertexList, index);

		Mesh mesh = {};
		/*vert.x = PyObject_GetAttrString(item, )
			
		if (!PyLong_Check(item))
		{
			free(result.data);
			PyErr_SetString(PyExc_TypeError, "expected sequence of integers");
			// return NULL;
		}*/
		/* assign to the C array */
		//result.data[index] = PyLong_AsLong(item);
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
		if (PyObject_HasAttrString(blenderData, "meshes"))
		{
			printf("data has meshes\n");

			PyObject* meshes = PyObject_GetAttrString(blenderData, "meshes");
			if (PyObject_Length(meshes) > 0)
			{
				printf("meshes is a sequence");
			}
			
		}

		if (PyObject_HasAttrString(blenderData, "objects"))
		{
			printf("data has objects\n");
		}
	}

	printf("argfail: printf: hello arg'ed world!\n");
	//return PyUnicode_FromString("argfail: return: hello arg'd world!");

	return internal;

}