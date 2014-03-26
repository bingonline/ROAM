#include "heightmap.h"
#include "util.h"

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Heightmap_print(Heightmap *map)
{
	printf("Heightmap {\n");
	printf("  %ld x %ld\n", map->width, map->height);
	printf("  m_map: %p\n", map->map);
	printf("  m_map[0]: %f\n", map->map[0]);
	printf("  m_map[last]: %f\n", map->map[map->width*map->height - 1]);

	size_t histogram[11] = {0};
	size_t i;

	for (i=0; i<map->width*map->height; ++i) {
		float val = map->map[i];
		val *= 10;
		if (val >10) {
			printf("WAD: %f\n", map->map[i]);
			histogram[10]++;
		} else {
			histogram[(int)val]++;
		}
	}

	printf("  histogram {\n");
	printf("    0.0 - 0.1 : %lu,\n", histogram[0]);
	printf("    0.1 - 0.2 : %lu,\n", histogram[1]);
	printf("    0.2 - 0.3 : %lu,\n", histogram[2]);
	printf("    0.3 - 0.4 : %lu,\n", histogram[3]);
	printf("    0.4 - 0.5 : %lu,\n", histogram[4]);
	printf("    0.5 - 0.6 : %lu,\n", histogram[5]);
	printf("    0.6 - 0.7 : %lu,\n", histogram[6]);
	printf("    0.7 - 0.8 : %lu,\n", histogram[7]);
	printf("    0.8 - 0.9 : %lu,\n", histogram[8]);
	printf("    0.9 - 1.0 : %lu,\n", histogram[9]);
	printf("    else      : %lu,\n", histogram[10]);
	printf("  }\n");

	printf("}\n");
}

Heightmap *Heightmap_read(const char *filename)
{
	Heightmap *map = NULL;
	FILE *fd = fopen(filename, "r");
	if (!fd) {
		printf("Unable to open file %s : %s\n", filename, strerror(errno));
		return map;
	}

	map = malloc(sizeof(Heightmap));

	fscanf(fd, "%lu ", &map->width);
	fscanf(fd, "%lu", &map->height);
	//map->height = map->width;

	map->map = malloc(map->width*map->height*sizeof(float));
	memset(map->map, 0, map->width*map->height*sizeof(float));

	float *ptr = map->map;

	size_t x, y;

	for (y=0; y<map->height; ++y) {
		for (x=0; x<map->width; ++x) {
			fscanf(fd, "%f", ptr);
			map->minZ = MIN(map->minZ, *ptr);
			map->maxZ = MAX(map->maxZ, *ptr);
			++ptr;
		}
	}

	fclose(fd);

	return map;
}

void Heightmap_normalize(Heightmap *map)
{
	int i;
	for (i=0; i<map->height*map->width; ++i) {
		map->map[i] /= map->maxZ;
	}
	map->maxZ /= map->maxZ;
	map->minZ /= map->maxZ;
}

float Heightmap_get(Heightmap *map, int x, int y)
{
	assert((map->height*y + x) < (map->height*map->width));
	assert((map->height*y + x) >= 0);
	return (map->map[map->height*y + x]);
}
