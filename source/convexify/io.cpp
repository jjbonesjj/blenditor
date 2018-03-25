
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

void buildCyLevel(C_NefPolyhedron nef, char* filePath)
{

	/*
	std::string path = filePath;
	path += CYL_EXTENSION;

	std::ofstream file(path, std::ios::out | std::ios::binary);

	char* currentOffset = 0;
	if (file.is_open())
	{
		CylHeader header = {};
		strcpy(header.magic, MAGIC);
		header.version = MAKE_VERSION(0, 0, 1);

		header.numChunks = convex_parts.size();
		header.chunks = sizeof(header);

		WRITE(&header, sizeof(header));

		for (auto it = convex_parts.begin(); it != convex_parts.end(); it++)
		{

		}
		

		file.close();
	}

	*/

}