/*
 * kld.c
 *
 * Created on: Apr 16, 2016
 * Author: Seokyong Hong
 */
#include "distance/kld.h"

double compute_kld(p_gaussian p, p_gaussian q) {
	double divergence = 0.0;
	int i, j, k;
	double determinant1, determinant2;
	double **inversed_covariance2;
	double temp_matrix[q -> dimension][q -> dimension];
	double mean_difference[q -> dimension];
	double temp[q -> dimension];

	if(!p || !q) {
		fprintf(stderr, "Correct Gaussians must be passed.");
		return divergence;
	}

	determinant1 = compute_determinant(p -> covariance, p -> dimension);
	determinant2 = compute_determinant(q -> covariance, q -> dimension);

	inversed_covariance2 = (double **)malloc(q -> dimension * sizeof(double *));
	for(i = 0;i < q -> dimension;i++)
		inversed_covariance2[i] = (double *)malloc(q -> dimension * sizeof(double));

	if(!inversed_covariance2) {
		fprintf(stderr, "Cannot allocate heap for matrix.");
		return divergence;
	}

	inverse_matrix(q -> covariance, inversed_covariance2, q -> dimension, determinant2);
	divergence = log10(determinant2 / determinant1);

	for(i = 0;i < q -> dimension;i++) {
		for(j = 0;j < q -> dimension;j++) {
			temp_matrix[i][j] = 0.0;
			for(k = 0;k < q -> dimension;k++) {
				temp_matrix[i][j] += inversed_covariance2[i][k] * p -> covariance[k][j];
			}
		}
	}

	for(i = 0;i < q -> dimension;i++)
		divergence += temp_matrix[i][i];

	for(i = 0;i < q -> dimension;i++)
		*(mean_difference + i) = *(q -> mean + i) - *(p -> mean + i);

	for(i = 0;i < q -> dimension;i++) {
		temp[i] = 0.0;
		for(j = 0;j < q -> dimension;j++) {
			temp[i] = mean_difference[j] * inversed_covariance2[j][i];
		}
	}

	for(i = 0;i < q -> dimension;i++)
		divergence += temp[i] * mean_difference[i];

	for(i = 0;i < q -> dimension;i++)
		free(inversed_covariance2[i]);
	free(inversed_covariance2);

	return divergence * 0.5;
}

double compute_symmetric_kld(p_gaussian p, p_gaussian q) {
	double divergence = 0.0;
	double d;
	if(!p || !q) {
		fprintf(stderr, "Correct Gaussians must be passed.");
		return divergence;
	}

	divergence = compute_kld(p, q);
	divergence += compute_kld(q, p);

	return divergence / 2.0;
}

