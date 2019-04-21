/*
 * loader.h
 *
 *  Created on: Apr 23, 2015
 *  Author: Seokyong Hong (shong3)
 */

#ifndef INCLUDE_SHONG3_LOADER_H_
#define INCLUDE_SHONG3_LOADER_H_

#include <gdal/gdal.h>
#include <gdal/cpl_conv.h>

typedef struct {
	unsigned int	top;
	unsigned int	left;
	unsigned int	width;
	unsigned int	height;
} bounding_box;

typedef struct {
	unsigned int	width;
	unsigned int	height;
	unsigned int	band;
	GDALDatasetH	dataset;
	float *			image;
} image_handle;

image_handle *open_image(char *path);
int load_image(image_handle *h_image);
float *retrieve_grid(image_handle *h_image, bounding_box box);
void close_grid(float *grid);
unsigned char *return_display_image(image_handle *h_image);
int close_image(image_handle *h_image);

#endif /* INCLUDE_SHONG3_LOADER_H_ */
