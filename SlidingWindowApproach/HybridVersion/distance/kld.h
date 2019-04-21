/*
 * kld.h
 *
 * Created on: Apr 16, 2016
 * Author: Seokyong Hong
 */

#ifndef KLD_H_
#define KLD_H_

#include "gaussian/gaussian.h"

double compute_kld(p_gaussian p, p_gaussian q);
double compute_symmetric_kld(p_gaussian p, p_gaussian q);

#endif /* KLD_H_ */
