#include "terrain_patch.hpp"
#include "gfx/opengl_render.hpp"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <terrain_file>\n", argv[0]);
		return -1;
	}

	TerrainPatch patch(argv[1]);
	patch.computeVariance();
	render(&patch);

	return 0;
}
