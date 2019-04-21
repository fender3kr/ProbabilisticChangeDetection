/*
 * image.c
 *
 * Created on: Mar 8, 2016
 * Author: Seokyong Hong
 */
#include "image.h"

static int compute_kld_size(int width, int height, int top_margin, int left_margin, int bottom_margin, int right_margin);

image_handle *open_image(char *path) {
	GDALDatasetH dataset;
	image_handle *handle = NULL;

	dataset = GDALOpen(path, GA_ReadOnly);
	if(!dataset) {
		fprintf(stderr, "Cannot load image.\n");
		return dataset;
	}

	handle = (image_handle *)malloc(sizeof(image_handle));
	if(!handle) {
		fprintf(stderr, "Cannot allocate memory for image handle.\n");
		return handle;
	}

	handle -> width = GDALGetRasterXSize(dataset);
	handle -> height = GDALGetRasterYSize(dataset);
	handle -> bands = GDALGetRasterCount(dataset);
	handle -> dataset = dataset;
	handle -> image = NULL;
	return handle;
}

int close_image(image_handle *h_image) {
	if(!h_image) {
		fprintf(stderr, "Cannot deallocate memory for image.\n");
		return 0;
	}

	if(h_image -> image)
		free(h_image -> image);

	GDALClose(h_image -> dataset);
	free(h_image);

	return 1;
}

int load_images(image_handle *h_image1, image_handle *h_image2, param_struct *parameters, int processes, int rank) {
	int actual_width = h_image1 -> width - parameters -> margin_left - parameters -> margin_right;
	int actual_height = h_image1 -> height - parameters -> margin_top - parameters -> margin_bottom;
	int sub_height = (int)ceil((double)actual_height / (double)processes);
	int remaining_height = actual_height - sub_height * (processes - 1);
	int window_half_width = (parameters -> window_width - 1) / 2;
	int window_half_height = (parameters -> window_height - 1) / 2;
	int band;

	h_image1 -> internal_width = actual_width;
	h_image2 -> internal_width = actual_width;
	h_image1 -> external_width = actual_width + (window_half_width * 2);
	h_image2 -> external_width = actual_width + (window_half_width * 2);

	if(rank < processes - 1) {
		h_image1 -> internal_height = sub_height;
		h_image2 -> internal_height = sub_height;
		h_image1 -> external_height = sub_height + (window_half_height * 2);
		h_image2 -> external_height = sub_height + (window_half_height * 2);
	}
	else {
		h_image1 -> internal_height = remaining_height;
		h_image2 -> internal_height = remaining_height;
		h_image1 -> external_height = remaining_height + (window_half_height * 2);
		h_image2 -> external_height = remaining_height + (window_half_height * 2);
	}
	h_image1 -> internal_x = parameters -> margin_left;
	h_image2 -> internal_x = parameters -> margin_left;
	h_image1 -> internal_y = sub_height * rank + parameters -> margin_top;
	h_image2 -> internal_y = sub_height * rank + parameters -> margin_top;
	h_image1 -> external_x = parameters -> margin_left - window_half_width;
	h_image2 -> external_x = parameters -> margin_left - window_half_width;
	h_image1 -> external_y = h_image1 -> internal_y - window_half_height;
	h_image2 -> external_y = h_image2 -> internal_y - window_half_height;

	h_image1 -> image = (float *)malloc(h_image1 -> external_width * h_image1 -> external_height * h_image1 -> bands * sizeof(float));
	if(!h_image1 -> image) {
		fprintf(stderr, "Cannot allocate memory for flattened image.\n");
		return -1;
	}
	h_image2 -> image = (float *)malloc(h_image2 -> external_width * h_image2 -> external_height * h_image2 -> bands * sizeof(float));
	if(!h_image2 -> image) {
		fprintf(stderr, "Cannot allocate memory for flattened image.\n");
		return -1;
	}

	for(band = 0;band < h_image1 -> bands;band++) {
		GDALRasterBandH p_band = GDALGetRasterBand(h_image1 -> dataset, band + 1);
		GDALRasterIO(p_band, GF_Read, h_image1 -> external_x, h_image1 -> external_y, h_image1 -> external_width, h_image1 -> external_height, h_image1 -> image + h_image1 -> external_width * h_image1 -> external_height * band, h_image1 -> external_width, h_image1 -> external_height, GDT_Float32, 0, 0);
		p_band = GDALGetRasterBand(h_image2 -> dataset, band + 1);
		GDALRasterIO(p_band, GF_Read, h_image2 -> external_x, h_image2 -> external_y, h_image2 -> external_width, h_image2 -> external_height, h_image2 -> image + h_image2 -> external_width * h_image2 -> external_height * band, h_image2 -> external_width, h_image2 -> external_height, GDT_Float32, 0, 0);
	}

	return h_image1 -> internal_width * h_image1 -> internal_height;
}

float *retrieve_subimage(image_handle *h_image, bounding_box window) {
	int index, band;
	float *subimage;

	if(!h_image) {
		fprintf(stderr, "Image handle is NULL.\n");
		return (float *)h_image;
	}
	if(!h_image -> image) {
		fprintf(stderr, "Image is NULL.\n");
		return h_image -> image;
	}

	subimage = (float *)malloc(window.width * window.height * h_image -> bands * sizeof(float));

	if(!subimage) {
		fprintf(stderr, "Memory for grid cannot be allocated.\n");
		return subimage;
	}

	for(band = 0;band < h_image -> bands;band++) {
		for(index = 0;index < window.height;index++) {
			memcpy(subimage + index * window.width + band * window.width * window.height, h_image -> image + window.left + (window.top + index) * h_image -> external_width + band * (h_image -> external_width * h_image -> external_height), window.width * sizeof(float));
		}
	}

	return subimage;
}

int save_change_map(char *path, _clusters *clusters, double *klds, int top, int left, int bottom, int right, image_handle *image1, image_handle *image2) {
	GDALDriverH driver;
	GDALDatasetH dataset;
	GDALRasterBandH band;
	char **options = NULL;
	double geo_transform[6];
	const double *posteriors;
	const double *means;
	const double *sigmas;
	int index, clust, max_index, kld_size;
	double max;
	int i, j, width, height;
	unsigned char *cluster_map;

	driver = GDALGetDatasetDriver(image1 -> dataset);
	GDALGetGeoTransform(image1 -> dataset, geo_transform);

	cluster_map = (unsigned char *)malloc(image1 -> width * image2 -> height * sizeof(unsigned char));
	memset(cluster_map, 255, image1 -> width * image2 -> height * sizeof(unsigned char));
	posteriors = (double *)vl_gmm_get_posteriors(clusters -> gmm);
	means = (double *)vl_gmm_get_means(clusters -> gmm);
	sigmas = (double *)vl_gmm_get_covariances(clusters -> gmm) ;
	for(index = 0;index < clusters -> G;index++) {
		for(i = 0;i < vl_gmm_get_dimension(clusters -> gmm);i++) {
			printf("M: %f ", ((double *)means)[index * vl_gmm_get_dimension(clusters -> gmm) + i]);
		}
		for(i = 0;i < vl_gmm_get_dimension(clusters -> gmm);i++) {
			printf("S: %f ", ((double *)sigmas)[index * vl_gmm_get_dimension(clusters -> gmm) + i]);
		}
		printf("\n");
	}

	kld_size = compute_kld_size(image1 -> width, image1 -> height, top, left, bottom, right);
	for(index = 0;index < kld_size;index++) {
		max = posteriors[index * clusters -> G];
		max_index = 0;
		for(clust = 1;clust < clusters -> G;clust++) {
			if(posteriors[index * clusters -> G + clust] > max) {
				max = posteriors[index * clusters -> G + clust];
				max_index = clust;
			}
		}


		width = image1 -> width - left - right;
		height = image1 -> height - top - bottom;
		i = (index % width + left);
		j = (index / width + top) * image1 -> width;
		*(cluster_map + i + j) = (unsigned char)max_index;
	}

	dataset = GDALCreate(driver, path, image1 -> width, image1 -> height, 1, GDT_Byte, options);
	GDALSetGeoTransform(dataset, geo_transform);
	GDALSetProjection(dataset, GDALGetProjectionRef(image1 -> dataset));
	band = GDALGetRasterBand(dataset, 1);
	GDALRasterIO(band, GF_Write, 0, 0, image1 -> width, image1 -> height, cluster_map, image1 -> width, image1 -> height, GDT_Byte, 0, 0);
	GDALClose(dataset);
	free(cluster_map);
	return 1;
}

int compute_kld_size(int width, int height, int top_margin, int left_margin, int bottom_margin, int right_margin) {
	return (width - left_margin - right_margin) * (height - top_margin - bottom_margin);
}
