#include "heightmap.h"
#include "util.h"

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
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
	printf("  min: %f\n", map->minZ);
	printf("  max: %f\n", map->maxZ);

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
	map->normal_map = NULL;

	fscanf(fd, "%lu ", &map->width);
	fscanf(fd, "%lu", &map->height);

	map->map = malloc(map->width*map->height*sizeof(float));
	memset(map->map, 0, map->width*map->height*sizeof(float));

	float *ptr = map->map;

	size_t i;

	for (i=0; i<map->height*map->width; ++i) {
		fscanf(fd, "%f", ptr);
		map->minZ = MIN(map->minZ, *ptr);
		map->maxZ = MAX(map->maxZ, *ptr);
		++ptr;
	}

	fclose(fd);

	return map;
}

void Heightmap_delete(Heightmap *map)
{
	if (map->map)
		free(map->map);
	if (map->normal_map)
		free(map->normal_map);
	free(map);
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

void Heightmap_calculate_normals(Heightmap *map)
{
	int x, y;

	map->normal_map = malloc(3*map->width*map->height*sizeof(float));
	memset(map->normal_map, 0, 3*map->width*map->height*sizeof(float));

	for (y=0; y<map->height; ++y) {
		for (x=0; x<map->width; ++x) {
			int k = 3*(map->height*y + x);

			// corner cases
			if (x == 0 || x == map->width-1 || y == 0 || y == map->height-1) {
				map->normal_map[k+0] = 0;
				map->normal_map[k+1] = 0;
				map->normal_map[k+2] = 1.0;
				continue;
			}

			// dx: Sobel filter
			//  -1  0  1
			//  -2  0  2
			//  -1  0  1
			//
			// dy: Sobel filter
			//  -1 -2 -1
			//   0  0  0
			//   1  2  1
			float tl = Heightmap_get(map, x-1, y-1);
			float l  = Heightmap_get(map, x-1, y  );
			float bl = Heightmap_get(map, x-1, y+1);
			float b  = Heightmap_get(map, x,   y+1);
			float br = Heightmap_get(map, x+1, y+1);
			float r  = Heightmap_get(map, x+1, y  );
			float tr = Heightmap_get(map, x+1, y-1);
			float t  = Heightmap_get(map, x,   y-1);

			float dx = tr + 2 * r + br - tl - 2 * l - bl;
			float dy = bl + 2 * b + br - tl - 2 * t - tr;

			// trial & error value.
			float str = 32.0;

			float length = sqrtf(dx*dx + dy*dy + 1.0/str*1.0/str);

			map->normal_map[k+0] = dx / length;
			map->normal_map[k+1] = dy / length;
			map->normal_map[k+2] = 1.0 / (str*length);
		}
	}
}

void Heightmap_get_normal(Heightmap *map, int x, int y, float *nx, float *ny, float *nz)
{
	assert(x >= 0 && x < map->width);
	assert(y >= 0 && x < map->height);
	int k = 3*(map->height*y + x);
	*nx = map->normal_map[k+0];
	*ny = map->normal_map[k+1];
	*nz = map->normal_map[k+2];
}

float Heightmap_get(Heightmap *map, int x, int y)
{
	assert((map->height*y + x) < (map->height*map->width));
	assert((map->height*y + x) >= 0);
	return (map->map[map->height*y + x]);
}
