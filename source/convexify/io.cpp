
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

namespace Cy
{
	struct Stack
	{
		char* buffer;
		int size;
		int capacity;

		char* push(int amount)
		{
			Assert(size + amount <= capacity);

			char* ptr = buffer + size;
			size += amount;
			return ptr;
		}
	};

	Stack initStack(int amount)
	{
		Stack stack = {};
		stack.buffer = (char*)malloc(amount);
		memset(stack.buffer, 0, stack.capacity);
		stack.size = 0;
		stack.capacity = amount;

		return stack;
	}

	void buildCyLevel(Array<Chunk> chunks, char* filePath)
	{

		int fileSize = 0;
		fileSize += sizeof(CylHeader);
		fileSize += chunks.size * sizeof(*chunks.data);
		for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
		{
			Chunk chunk = chunks.data[currentChunk];
			fileSize += chunk.meshes.size * sizeof(*chunk.meshes.data);
			for (int currentMesh = 0; currentMesh < chunk.meshes.size; currentMesh++)
			{
				Mesh mesh = chunk.meshes[currentMesh];
				fileSize += mesh.subMeshes.size * sizeof(*mesh.subMeshes.data);
				fileSize += mesh.vertices.size * sizeof(*mesh.vertices.data);
				for (int currentSubMesh = 0; currentSubMesh < mesh.subMeshes.size; currentSubMesh++)
				{
					SubMesh subMesh = mesh.subMeshes[currentSubMesh];
					fileSize += subMesh.faces.size * sizeof(*subMesh.faces.data);
				}
			}
		}

#define PREREPETITIVE(index, previousPreviousType, previousPreviousTypeEntry, previousType) \
	previousType* source##previousType = &source##previousPreviousType->previousPreviousTypeEntry[index]; \
	previousType* dest##previousType = &dest##previousPreviousType->previousPreviousTypeEntry[index];

#define REPETITIVE(index, previousPreviousType, previousPreviousTypeEntry, previousType, previousTypeEntry, newType) \
	newType* buffer##newType = (newType*)buffer.push(sizeof(*source##previousType->previousTypeEntry.data) * source##previousType->previousTypeEntry.size); \
	dest##previousType->previousTypeEntry.data = REL_PTR(buffer.buffer, buffer##newType); \
	memcpy(buffer##newType, source##previousType, sizeof(*source##previousPreviousType->previousPreviousTypeEntry.data) * source##previousPreviousType->previousPreviousTypeEntry.size);

		Stack buffer = initStack(fileSize);

		CylHeader source = {};
		strcpy(source.magic, MAGIC);
		source.version = MAKE_VERSION(0, 0, 1);
		source.chunks.size = chunks.size;
		source.chunks.data = chunks.data;
		source.containsMagic = DEBUG_WITH_MAGIC;
		source.numberType = FloatingPoint;
		source.fixedPointNumFractionalBits = 0;


		CylHeader* destCylHeader = (CylHeader*)buffer.push(sizeof(CylHeader));
		CylHeader* sourceCylHeader = &source;
		memcpy(destCylHeader, sourceCylHeader, sizeof(CylHeader));

		destCylHeader->chunks.size = chunks.size;
		destCylHeader->chunks.data = (Chunk*)buffer.push(sizeof(Chunk) * destCylHeader->chunks.size);
		memcpy(destCylHeader->chunks.data, sourceCylHeader->chunks.data, sizeof(Chunk) * destCylHeader->chunks.size);
		Chunk* bufferChunk = destCylHeader->chunks.data;
		destCylHeader->chunks.data = REL_PTR(destCylHeader->chunks.data, buffer.buffer);

		for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
		{
			Chunk* sourceChunk = sourceCylHeader->chunks(currentChunk);
			Chunk* destChunk = &bufferChunk[currentChunk];
			BUILD_MAGIC(destChunk, Chunk);
			int meshesSizeBytes = sizeof(*sourceChunk->meshes.data) * sourceChunk->meshes.size;
			Mesh* bufferMesh = (Mesh*)buffer.push(meshesSizeBytes);
			destChunk->meshes.data = REL_PTR(bufferMesh, buffer.buffer);
			memcpy(bufferMesh, sourceChunk->meshes.data, meshesSizeBytes);

			for (int currentMesh = 0; currentMesh < sourceChunk->meshes.size; currentMesh++)
			{
				Mesh* sourceMesh = sourceChunk->meshes(currentMesh);
				Mesh* destMesh = &bufferMesh[currentMesh];
				BUILD_MAGIC(destMesh, Mesh);

				int subMeshesSizeBytes = sizeof(*sourceMesh->subMeshes.data) * sourceMesh->subMeshes.size;
				SubMesh* bufferSubMesh = (SubMesh*)buffer.push(subMeshesSizeBytes);
				destMesh->subMeshes.data = REL_PTR(bufferSubMesh, buffer.buffer);
				memcpy(bufferSubMesh, sourceMesh->subMeshes.data, subMeshesSizeBytes);

				int verticesSizeBytes = sizeof(*sourceMesh->vertices.data) * sourceMesh->vertices.size;
				Point* bufferVertex = (Point*)buffer.push(verticesSizeBytes);
				destMesh->vertices.data = REL_PTR(bufferVertex, buffer.buffer);
				memcpy(bufferVertex, sourceMesh->vertices.data, verticesSizeBytes);

				for (int currentSubMesh = 0; currentSubMesh < sourceMesh->subMeshes.size; currentSubMesh++)
				{
					SubMesh* sourceSubMesh = sourceMesh->subMeshes(currentSubMesh);
					SubMesh* destSubMesh = &bufferSubMesh[currentSubMesh];
					BUILD_MAGIC(destSubMesh, SubMesh);

					int facesSizeBytes = sizeof(*sourceSubMesh->faces.data) * sourceSubMesh->faces.size;
					Face* bufferFaces = (Face*)buffer.push(facesSizeBytes);
					destSubMesh->faces.data = REL_PTR(bufferFaces, buffer.buffer);
					memcpy(bufferFaces, sourceSubMesh->faces.data, facesSizeBytes);

					for (int currentFace = 0; currentFace < sourceSubMesh->faces.size; currentFace++)
					{
						Face* destFace = &bufferFaces[currentFace];
						BUILD_MAGIC(destFace, Face);
					}
				}

				for (int currentPoint = 0; currentPoint < sourceMesh->vertices.size; currentPoint++)
				{
					Point* destPoint = &bufferVertex[currentPoint];
					BUILD_MAGIC(destPoint, Point);
				}
			}
		}


		std::string path = filePath;
		path += CYL_EXTENSION;

		std::ofstream file(path, std::ios::out | std::ios::binary);

		char* currentOffset = 0;
		if (file.is_open())
		{
			file.write(buffer.buffer, fileSize);

			file.close();
		}

		free(buffer.buffer);
	}
}
#undef PREREPETITIVE
#undef REPETITIVE