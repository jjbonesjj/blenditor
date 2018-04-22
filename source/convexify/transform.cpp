#include "transform.h"
#include "convexify.h"

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include "globals.h"
#include "io.h"

#include <vector>
#include <CGAL/IO/OFF_reader.h>

namespace Cy
{
	C_Polyhedron makePolyhedron(Array<Vertex> vertices, Array<Polygon> faces)
	{

		std::vector<CK_Point> points;
		std::vector<std::vector<size_t>> polygons;

		points.resize(vertices.size);
		polygons.resize(faces.size);

		printf("hello1\n");
		for (int i = 0; i < vertices.size; i++)
		{
			points[i] = CK_Point(vertices[i].coords.x, vertices[i].coords.y, vertices[i].coords.z);
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
		printf("hello4\n");
		C_Polyhedron mesh = {};
		CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);
		printf("hello5\n");
		Assert(is_valid(mesh, VERBOSE_LOGGING));
		printf("hello6\n");
		Assert(is_closed(mesh));

		// edges etc. 

		printf("hello7\n");
		// ensure that the mesh orientation goes outward
		if (!CGAL::Polygon_mesh_processing::is_outward_oriented(mesh))
		{
			printf("hello8\n");
			CGAL::Polygon_mesh_processing::reverse_face_orientations(mesh);
		}
		printf("hello9\n");
		if (TRIANGULATE_MESHES)
		{
			printf("hello10\n");
			Assert(CGAL::Polygon_mesh_processing::triangulate_faces(mesh));
		}
		printf("hello11\n");
		Assert(is_valid(mesh, VERBOSE_LOGGING));
		printf("hello12\n");
		Assert(is_closed(mesh));
		printf("hello13\n");

		return mesh;
	}

	Mesh polyhedraListToMesh(std::list<C_Polyhedron> polyhedra)
	{
		// initialize the indices
		int vertexIndices = 0;
		int facetsIndices = 0;
		int edgeIndices = 0;

		for (auto it = polyhedra.begin(); it != polyhedra.end(); it++)
		{
			if (TRIANGULATE_MESHES)
			{
				printf("hello18\n");
				Assert(CGAL::Polygon_mesh_processing::triangulate_faces(*it));
			}
			for (C_Polyhedron::Vertex_iterator jt = it->vertices_begin(); jt != it->vertices_end(); jt++)
			{
				jt->id() = vertexIndices++;
			}

			for (C_Polyhedron::Facet_iterator jt = it->facets_begin(); jt != it->facets_end(); jt++)
			{
				jt->id() = facetsIndices++;
			}

			for (C_Polyhedron::Edge_iterator jt = it->edges_begin(); jt != it->edges_end(); jt++)
			{
				jt->id() = edgeIndices++;
			}
		}
		u64 faceCount = 0;

		// build the mesh
		Mesh mesh = {};
		mesh.subMeshes.size = polyhedra.size();
		mesh.subMeshes.data = (SubMesh*)malloc(sizeof(SubMesh) * mesh.subMeshes.size);

		mesh.vertices.size = vertexIndices;
		mesh.vertices.data = (Point*)malloc(sizeof(Point) * mesh.vertices.size);
		int subMeshCounter = 0;

		for (auto cmesh = polyhedra.begin(); cmesh != polyhedra.end(); cmesh++)
		{
			std::map<C_FaceDescriptor, C_Vector> fnormals;
			std::map<C_VertexDescriptor, C_Vector> vnormals;
			printf("hello19\n");
			CGAL::Polygon_mesh_processing::compute_normals(*cmesh,
														   boost::make_assoc_property_map(vnormals),
														   boost::make_assoc_property_map(fnormals));

			SubMesh subMesh = {};
			subMesh.faces.size = cmesh->size_of_facets();
			subMesh.faces.data = (Face*)malloc(sizeof(Face) * subMesh.faces.size);
			int facetsCounter = 0;
			for (C_Polyhedron::Face_iterator face = cmesh->facets_begin();
				 face != cmesh->facets_end();
				 face++, facetsCounter++)
			{
				C_Polyhedron::Halfedge_around_facet_circulator circ = face->facet_begin();

				int facetCounter = 0;
				std::cout << "Vertex indices of facet: " << face->id();
				do {
					std::cout << " [" << circ->vertex()->id() << "] ";
					std::cout << "{ " << circ->vertex()->point().x() << " ";
					std::cout << circ->vertex()->point().y() << " ";
					std::cout << circ->vertex()->point().z() << " } ";

					size_t index = circ->vertex()->id();
					subMesh.faces(facetsCounter)->indices[facetCounter] = index;

					facetCounter++;
				} while (++circ != face->facet_begin());
				std::cout << '\n';
				Assert(facetCounter == 3);

				subMesh.faces(facetsCounter)->normal[0] = fnormals[face].x().floatValue();
				subMesh.faces(facetsCounter)->normal[1] = fnormals[face].y().floatValue();
				subMesh.faces(facetsCounter)->normal[2] = fnormals[face].z().floatValue();
				faceCount++;
			}

			*mesh.subMeshes(subMeshCounter) = subMesh;
			subMeshCounter++;

			// get vertices
			int meshVertexCounter = 0;
			for (C_Polyhedron::Vertex_iterator vertex = cmesh->vertices_begin(); vertex != cmesh->vertices_end(); vertex++)
			{
				mesh.vertices(meshVertexCounter)->posFloating[0] = (float)vertex->point().x().doubleValue();
				mesh.vertices(meshVertexCounter)->posFloating[1] = (float)vertex->point().y().doubleValue();
				mesh.vertices(meshVertexCounter)->posFloating[2] = (float)vertex->point().z().doubleValue();
				meshVertexCounter++;
			}
		}

		return mesh;
	}


	// todo remove?
	Array<Chunk> convertBlenderDataToLevelFormat(BlenderData blenderData)
	{
		Array<Chunk> chunks;

		Chunk* chunk = (Chunk*)malloc(sizeof(Chunk));

		Array<Chunk> chunks = {};
		chunks.size = 1;
		chunks.data = chunk;

		Array<Mesh>* meshes = &chunk->meshes;
		meshes->size = blenderData.meshes.size;
		meshes->data = (Mesh*)malloc(meshes->size * sizeof(Mesh));

		Mesh* mesh = meshes->data;
		BlenderMesh* blenderMesh = blenderData.meshes.data;
		for (u32 meshIndex = 0; meshIndex < meshes->size; meshIndex++, mesh++, blenderMesh++)
		{
			// copy the vertices
			Point* points = mesh->vertices.data;
			Vertex* blenderVertex = blenderMesh->vertices.data;
			for (u32 vertexIndex = 0; vertexIndex < mesh->vertices.size; vertexIndex++, points++, blenderVertex++)
			{
				points->posFloating[0] = blenderVertex->coords.x;
				points->posFloating[1] = blenderVertex->coords.y;
				points->posFloating[2] = blenderVertex->coords.z;
			}

			// copy the faces
			// since this path is not performing a convexification, there is only ever 1 submesh
			mesh->subMeshes.size = 1;
			mesh->subMeshes.data = (SubMesh*)malloc(sizeof(SubMesh));

			SubMesh* subMesh = mesh->subMeshes.data;

			subMesh->faces.size = blenderMesh->polygons.size;
			subMesh->faces.data = (Face*)malloc(subMesh->faces.size * sizeof(Face));

			Face* faces = subMesh->faces.data;
			Polygon* blenderFaces = blenderMesh->polygons.data;
			for (u32 faceIndex = 0; faceIndex < subMesh->faces.size; faceIndex++, faces++, blenderFaces++)
			{
				if (blenderFaces->numVertices == 3)
				{
					faces->indices[0] = blenderFaces->vertices[0];
					faces->indices[1] = blenderFaces->vertices[1];
					faces->indices[2] = blenderFaces->vertices[2];
				}
				
			}

		}
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



	Array<BlenderMesh> extractMeshArray(PyObject* meshList)
	{
		Array<BlenderMesh> result = {};
		result.size = PyObject_Length(meshList);

		result.data = (BlenderMesh *)malloc(sizeof(Vertex)*result.size);
		for (int index = 0; index < result.size; index++)
		{

			PyObject* item = PySequence_GetItem(meshList, index);

			BlenderMesh mesh = {};

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
}