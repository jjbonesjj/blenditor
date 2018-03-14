#include "transform.h"
#include "convexify.h"

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include "globals.h"

#include <vector>
#include <CGAL/IO/OFF_reader.h>
C_NefPolyhedron make_nef(Array<Vertex> vertices, Array<Polygon> faces)
{

	std::vector<C_Point> points;
	std::vector<std::vector<size_t>> polygons;

	points.resize(vertices.size);
	polygons.resize(faces.size);

	printf("hello1\n");
	for (int i = 0; i < vertices.size; i++)
	{
		points[i] = C_Point(vertices[i].coords.x, vertices[i].coords.y, vertices[i].coords.z);
	}
	printf("hello2\n");
	for (int i = 0; i < faces.size; i++)
	{
		polygons[i].resize(faces[i].numVertices);
		for (int j = 0; j < faces[i].numVertices; j++)
		{
			polygons[i][j] = faces[i].vertices[j];
		}
	}

	printf("hello3\n");
	Assert(CGAL::Polygon_mesh_processing::is_polygon_soup_a_polygon_mesh(polygons));
	bool result = CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);

	if (result)
	{
		printf("successfully oriented polygon soup\n");
	}
	else
	{
		printf("some points were duped when orienting polygon soup\n");
	}

	C_Polyhedron mesh = {};
	CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);
	Assert(is_valid(mesh, VERBOSE_LOGGING));
	Assert(is_closed(mesh));
	

	// ensure that the mesh orientation goes outward
	if (!CGAL::Polygon_mesh_processing::is_outward_oriented(mesh))
	{
		CGAL::Polygon_mesh_processing::reverse_face_orientations(mesh);
	}

	if (TRIANGULATE_MESHES)
	{
		Assert(CGAL::Polygon_mesh_processing::triangulate_faces(mesh));
	}

	Assert(is_valid(mesh, VERBOSE_LOGGING));
	Assert(is_closed(mesh));

	auto nef = C_NefPolyhedron(mesh);

	Assert(nef.is_valid(VERBOSE_LOGGING));

	return nef;
}


#include "Python.h"

#define GET_ATTR(object, name) PyObject* name = nullptr; if(PyObject_HasAttrString(object, #name)) { name = PyObject_GetAttrString(object, #name); } else { printf("failed to extract "#name"\n"); exit(0); }
#define PRINT_PY(object) { PyObject* objectPath = PyObject_Repr(object); const char* s = PyUnicode_AsUTF8(objectPath); const char* type = object->ob_type->tp_name; printf("%s : %s\n", type,  s); }


Array<Vertex> extractVertexArray(PyObject* vertexList)
{
	Array<Vertex> result = {};
	result.size = PyObject_Length(vertexList);

	result.data = (Vertex*)malloc(sizeof(Vertex)*result.size);
	for (int i = 0; i < result.size; i++)
	{

		PyObject* item = PySequence_GetItem(vertexList, i);

		Vertex vertex = {};

		GET_ATTR(item, co);
		GET_ATTR(item, normal);
		GET_ATTR(item, index);

		// coords
		PyObject* coordX = PySequence_GetItem(co, 0);
		vertex.coords.x = PyFloat_AsDouble(coordX);
		PyObject* coordY = PySequence_GetItem(co, 1);
		vertex.coords.y = PyFloat_AsDouble(coordY);
		PyObject* coordZ = PySequence_GetItem(co, 2);
		vertex.coords.z = PyFloat_AsDouble(coordZ);

		// normals
		PyObject* normalX = PySequence_GetItem(normal, 0);
		vertex.normal.x = PyFloat_AsDouble(normalX);
		PyObject* normalY = PySequence_GetItem(normal, 1);
		vertex.normal.y = PyFloat_AsDouble(normalY);
		PyObject* normalZ = PySequence_GetItem(normal, 2);
		vertex.normal.z = PyFloat_AsDouble(normalZ);

		//index
		vertex.index = PyLong_AsLong(index);

		result.data[i] = vertex;
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

		polygon.numVertices = PyObject_Length(vertices);

		for (int j = 0; j < polygon.numVertices; j++)
		{
			PyObject* vertexIndex = PySequence_GetItem(vertices, j);
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
		mesh.vertices = extractVertexArray(vertices);
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