/*
 * image.h
 *
 * Created on: Mar 8, 2016
 * Author: Seokyong Hong
 */

#ifndef IMAGE_IMAGE_H_
#define IMAGE_IMAGE_H_

#include <math.h>
#include <stdlib.h>
#include <gdal/gdal.h>
#include "cluster/gmm_em.h"
#include "../params.h"

typedef struct {
	GDALDatasetH	dataset;
	unsigned int	width;
	unsigned int	height;
	unsigned int	bands;
	unsigned int	external_x;
	unsigned int	external_y;
	unsigned int	internal_x;
	unsigned int	internal_y;
	unsigned int	external_width;
	unsigned int	external_height;
	unsigned int	internal_width;
	unsigned int	internal_height;
	float *			image;
} image_handle;

image_handle *open_image(char *path);
int load_images(image_handle *h_image1, image_handle *h_image2, param_struct *parameters, int processes, int rank);
float *retrieve_subimage(image_handle *h_image, bounding_box window);
int close_image(image_handle *h_image);
int save_change_map(char *path, _clusters *clusters, double *klds, int top, int left, int bottom, int right, image_handle *image1, image_handle *image2);

#endif /* IMAGE_IMAGE_H_ */
