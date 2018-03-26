
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

struct Stack
{
	char* buffer;
	int size;
	int used;

	char* push(int amount)
	{
		Assert(used + amount <= size);
		char* ptr = buffer + size;
		size += amount;
		return ptr;
	}
};

Stack initStack(int amount)
{
	Stack stack = {};
	stack.buffer = (char*)malloc(amount);
	stack.used = 0;
	stack.size = amount;
}


void buildCyLevel(Array<Chunk> chunks, char* filePath)
{

	int fileSize = 0;
	fileSize += chunks.size * sizeof(*chunks.data);
	for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
	{
		Chunk chunk = chunks.data[currentChunk];
		fileSize += chunk.numMeshes * sizeof(*chunk.meshes);
		for (int currentMesh = 0; currentMesh < chunk.numMeshes; currentMesh++)
		{
			Mesh mesh = chunk.meshes[currentMesh];
			fileSize += mesh.numSubMeshes * sizeof(*mesh.subMeshes);
			fileSize += mesh.numVertices * sizeof(*mesh.vertices);
			for (int currentSubMesh = 0; currentSubMesh < mesh.numSubMeshes; currentSubMesh++)
			{
				SubMesh subMesh = mesh.subMeshes[currentSubMesh];
				fileSize += subMesh.numFaces * sizeof(*subMesh.faces);
			}
		}
	}

	Stack buffer = initStack(fileSize);

	CylHeader* header = (CylHeader*)buffer.push(sizeof(CylHeader));

	strcpy(header->magic, MAGIC);
	header->version = MAKE_VERSION(0, 0, 1);

	Chunk* bufferChunks = (Chunk*)buffer.push(sizeof(Chunk) * chunks.size);

	header->numChunks = chunks.size;
	header->chunks = REL_PTR(buffer.buffer, bufferChunks);

	memcpy(bufferChunks, chunks.data, chunks.size * sizeof(*chunks.data));

	for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
	{
		Chunk* sourceChunk = &chunks.data[currentChunk];
		Chunk* destChunk = &bufferChunks[currentChunk];
		
		Mesh* bufferMeshes = (Mesh*)buffer.push(sizeof(*sourceChunk->meshes) * sourceChunk->numMeshes);
		destChunk->meshes = REL_PTR(buffer.buffer, bufferMeshes);

		memcpy(bufferMeshes, sourceChunk->meshes, sizeof(*sourceChunk->meshes) * sourceChunk->numMeshes);

		for (int currentMesh = 0; currentMesh < sourceChunk->numMeshes; currentMesh++)
		{
			Mesh* sourceMesh = &sourceChunk->meshes[currentMesh];
			Mesh* destMesh = &destChunk->meshes[currentMesh];

			SubMesh* bufferSubMeshes = (SubMesh*)buffer.push(sizeof(*sourceMesh->subMeshes) * sourceMesh->numSubMeshes);
			destMesh->subMeshes = REL_PTR(buffer.buffer, bufferSubMeshes);

			memcpy(bufferMeshes, sourceChunk->meshes, sizeof(*sourceChunk->meshes) * sourceChunk->numMeshes);
		}


	}

	std::string path = filePath;
	path += CYL_EXTENSION;

	std::ofstream file(path, std::ios::out | std::ios::binary);

	char* currentOffset = 0;
	if (file.is_open())
	{
		CylHeader header = {};
		strcpy(header.magic, MAGIC);
		header.version = MAKE_VERSION(0, 0, 1);

		header.numChunks = 1;
		header.chunks = (Chunk*)sizeof(header);

		WRITE(&header, sizeof(header));



		for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
		{
			WRITE(chunks.data, chunks.size * sizeof(*chunks.data));
		}

		for (int currentChunk = 0; currentChunk < chunks.size; currentChunk++)
		{
			Chunk chunk = chunks.data[currentChunk];
			for (int currentMesh = 0; currentMesh < chunks[currentChunk].numMeshes; currentMesh++)
			{
				Mesh mesh = chunk.meshes[currentMesh];

			}
		}
		

		file.close();
	}

}