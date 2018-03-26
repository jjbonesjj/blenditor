
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

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
	stack.size = 0;
	stack.capacity = amount;

	return stack;
}

void buildCyLevel(Array<Chunk> chunks, char* filePath)
{

	int fileSize = 0;
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


	CylHeader* destCylHeader = (CylHeader*)buffer.push(sizeof(CylHeader));
	CylHeader* sourceCylHeader = &source;
	memcpy(destCylHeader, sourceCylHeader, sizeof(CylHeader));

#if 0
	for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
	{
		PREREPETITIVE(currentChunk, CylHeader, chunks, Chunk);
		REPETITIVE(currentChunk, CylHeader, chunks, Chunk, meshes, Mesh);

		for (int currentMesh = 0; currentMesh < sourceChunk->meshes.size; currentMesh++)
		{
			PREREPETITIVE(currentMesh, Chunk, meshes, Mesh);
			REPETITIVE(currentMesh, Chunk, meshes, Mesh, subMeshes, SubMesh);
			REPETITIVE(currentMesh, Chunk, meshes, Mesh, vertices, Vertex);
		}
	}
#else
	for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
	{
		Chunk* sourceChunk = sourceCylHeader->chunks(currentChunk); 
		Chunk* destChunk = destCylHeader->chunks(currentChunk);
		Mesh* bufferMesh = (Mesh*)buffer.push(sizeof(*sourceChunk->meshes.data) * sourceChunk->meshes.size); 
		destChunk->meshes.data = decltype(bufferMesh)((char*)buffer.buffer - (char*)bufferMesh); 
		memcpy(bufferMesh, sourceChunk, sizeof(*sourceCylHeader->chunks.data) * sourceCylHeader->chunks.size);;

		for (int currentMesh = 0; currentMesh < sourceChunk->meshes.size; currentMesh++)
		{
			Mesh* sourceMesh = sourceChunk->meshes(currentMesh); 
			Mesh* destMesh = destChunk->meshes(currentMesh);

			SubMesh* bufferSubMesh = (SubMesh*)buffer.push(sizeof(*sourceMesh->subMeshes.data) * sourceMesh->subMeshes.size); 
			destMesh->subMeshes.data = decltype(bufferSubMesh)((char*)buffer.buffer - (char*)bufferSubMesh);
			memcpy(bufferSubMesh, sourceMesh, sizeof(*sourceChunk->meshes.data) * sourceChunk->meshes.size);

			Vertex* bufferVertex = (Vertex*)buffer.push(sizeof(*sourceMesh->vertices.data) * sourceMesh->vertices.size); 
			destMesh->vertices.data = decltype(bufferVertex)((char*)buffer.buffer - (char*)bufferVertex); 
			memcpy(bufferVertex, sourceMesh, sizeof(*sourceChunk->meshes.data) * sourceChunk->meshes.size);
		}


	}
#endif


	std::string path = filePath;
	path += CYL_EXTENSION;

	std::ofstream file(path, std::ios::out | std::ios::binary);

	char* currentOffset = 0;
	if (file.is_open())
	{
		file.write(buffer.buffer, fileSize);
		
		file.close();
	}

}

#undef PREREPETITIVE
#undef REPETITIVE