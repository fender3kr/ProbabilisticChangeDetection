/*
 * app.h
 *
 *  Created on: Feb 27, 2015
 *  Author: Seokyong Hong
 */

#ifndef SRC_SHONG3_APP_H_
#define SRC_SHONG3_APP_H_

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "loader.h"
#include "vlfeat-0.9.20/gmm.h"
#include "vlfeat-0.9.20/host.h"

#define MAX_ITERATION	100
#define NUMBER_THREADS	8

typedef struct {
	VlGMM *			gmm;
	double			loglikelihood;
	int				G;
} _clusters;

int clustering_init(void);
int clustering_close(_clusters *clusters);
int clustering_perform(_clusters *clusters, double *data, int size, int dimension, int from, int to);

int compute_kld_size(int width, int height, int top_margin, int left_margin, int bottom_margin, int right_margin);
int check_plan(float *grid1, float *grid2, int width, int height, int band, float percentage);
int save_clusters(_clusters *clusters, double *klds, int top, int left, int bottom, int right, image_handle *image1, image_handle *image2);
void add_random_pixels(float *image1, float *image2, int band, int width, int height, float percentage);

#endif /* SRC_SHONG3_APP_H_ */
