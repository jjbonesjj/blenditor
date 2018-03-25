
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

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
			Chunk chunk = chunks.data[currentChunk];
			for (int currentMesh = 0; currentMesh < chunks[currentChunk].numMeshes; currentMesh++)
			{
				Mesh mesh = chunk.meshes[currentMesh];

			}
		}
		

		file.close();
	}

}