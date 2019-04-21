/*
 * gmm_em.h
 *
 * Created on: Apr 18, 2016
 * Author: Seokyong Hong
 */

#ifndef GMM_EM_H_
#define GMM_EM_H_

#include <stdio.h>
#include "vlfeat-0.9.20/gmm.h"
#include "vlfeat-0.9.20/host.h"

#define MAX_ITERATION	100
#define NUMBER_THREADS	8

typedef struct {
	VlGMM *			gmm;
	double			loglikelihood;
	int				G;
} _clusters;

int cluster_gmm_em(_clusters *clusters, double *data, int size, int dimension, int from, int to);
int close_gmm_em(_clusters *clusters);

#endif /* GMM_EM_H_ */
