
#include "io.h"

#define WRITE(bytes, size) currentOffset += size; file.write((char*)bytes, size)

void buildCyLevel(C_NefPolyhedron nef, char* filePath)
{
	std::list<C_Polyhedron> convex_parts;

	// the first volume is the outer volume, which is 
	// ignored in the decomposition
	typedef C_NefPolyhedron::Volume_const_iterator Volume_const_iterator;
	Volume_const_iterator ci = ++nef.volumes_begin();
	for (; ci != nef.volumes_end(); ++ci) {
		if (ci->mark()) {
			C_Polyhedron P;
			nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);
			convex_parts.push_back(P);
		}
	}
	std::cout << "decomposition into " << convex_parts.size() << " convex parts " << std::endl;

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



}