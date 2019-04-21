/*
 * gaussian.h
 *
 *  Created on: Mar 31, 2015
 *  Author: Seokyong Hong
 */

#ifndef INCLUDE_SHONG3_GAUSSIAN_H_
#define INCLUDE_SHONG3_GAUSSIAN_H_

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
float compute_probability(p_gaussian descriptor, float *observation);
double compute_determinant(double **matrix, int dimension);
void inverse_matrix(double **matrix, double **inversed, int dimension, double determinant);
void close_gaussian(p_gaussian);

#endif /* INCLUDE_SHONG3_GAUSSIAN_H_ */
