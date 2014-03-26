#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	float *map;

	size_t width, height;

	float minZ, maxZ;

} Heightmap;

/**
 * Debug prints the given heightmap
 */
void Heightmap_print(Heightmap *map);

/**
 * Read the heightmap from the given file.
 *
 * File is expected to be in the following format:
 * First row tells the dimension.
 * e.g 512   (-> expects 512x512)
 *
 * then
 * each row has 512 float numbers separated by spaces.
 *
 * @param heightmap on succes, NULL on failure.
 */
Heightmap *Heightmap_read(const char *filename);

/**
 * Normalizes the given heightmap so that all values are between [0, 1]
 *
 * @param map heightmap
 */
void Heightmap_normalize(Heightmap *map);

/**
 * Return the height value at given coordinates.
 *
 * @param map heightmap
 * @param x coordinate
 * @param y coordinate
 *
 * @return height
 */
float Heightmap_get(Heightmap *map, int x, int y);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // HEIGHTMAP_H
