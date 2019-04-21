/*
 * KLD.h
 *
 *  Created on: Apr 2, 2015
 *  Author: Seokyong Hong
 */

#ifndef INCLUDE_SHONG3_KLD_H_
#define INCLUDE_SHONG3_KLD_H_

#include "gaussian.h"

/*
 A function to compute Kullback-Leibler divergence between two distributions:
 Input: two gaussian descriptors G1(M1, C1), G2(M2, C2)
 Computation: 0.5 * (log(|C2|/|C1|) + trace(inverse(C2) * C1) + transpose(M1 - M2) * inverse(C2) * (M1 - M2))
 Output: double KLD
 */
double compute_kld(p_gaussian p, p_gaussian q);
double compute_symmetric_kld(p_gaussian p, p_gaussian q);

#endif /* INCLUDE_SHONG3_KLD_H_ */
