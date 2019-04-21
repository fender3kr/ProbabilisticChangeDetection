/*
 * gaussian.h
 *
 * Created on: Apr 16, 2016
 * Author: Seokyong Hong
 */

#ifndef GAUSSIAN_GAUSSIAN_H_
#define GAUSSIAN_GAUSSIAN_H_

#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

typedef struct {
	int dimension;
	double *mean;
	double **covariance;
} gaussian;
typedef gaussian *p_gaussian;

p_gaussian init_gaussian(int dimension);
int compute_gaussian(p_gaussian descriptor, float *data, int length);
void close_gaussian(p_gaussian);

double compute_determinant(double **matrix, int dimension);
void inverse_matrix(double **matrix, double **inversed, int dimension, double determinant);
void add_random_pixels(float *image1, float *image2, int band, int width, int height, float percentage);

#endif /* GAUSSIAN_GAUSSIAN_H_ */
