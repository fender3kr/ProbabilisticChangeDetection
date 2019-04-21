/*
 * gmm_em.c
 *
 * Created on: Apr 18, 2016
 * Author: Seokyong Hong
 */
#include "gmm_em.h"

int cluster_gmm_em(_clusters *clusters, double *data, int size, int dimension, int from, int to) {
	int index;
	double loglikelihood;
	VlGMM *gmm;

	if(from > to) {
		fprintf(stderr, "Cannot perform clustering.\n");
		return 0;
	}

	for(index = from;index <= to;index++) {
		gmm = vl_gmm_new(VL_TYPE_DOUBLE, dimension, index);
		vl_gmm_set_max_num_iterations(gmm, MAX_ITERATION);
		vl_gmm_set_num_repetitions(gmm, 3);
		vl_set_num_threads(NUMBER_THREADS);
		vl_gmm_set_initialization(gmm, VlGMMRand);
		vl_gmm_cluster(gmm, data, size);
		loglikelihood = vl_gmm_get_loglikelihood(gmm);

		if(index == from) {
			clusters -> loglikelihood = loglikelihood;
			clusters -> gmm = gmm;
			clusters -> G = index;
			continue;
		}

		if(loglikelihood > clusters -> loglikelihood) {
			clusters -> loglikelihood = loglikelihood;
			if(!clusters -> gmm)
				vl_gmm_delete(clusters -> gmm);

			clusters -> gmm = gmm;
			clusters -> G = index;
		}
		else {
			vl_gmm_delete(gmm);
		}
	}
	printf("G: %d L: %f\n", clusters -> G, clusters -> loglikelihood);

	return 1;
}

int close_gmm_em(_clusters *clusters) {
	if(clusters -> gmm)
		vl_gmm_delete(clusters -> gmm);

	return 1;
}
