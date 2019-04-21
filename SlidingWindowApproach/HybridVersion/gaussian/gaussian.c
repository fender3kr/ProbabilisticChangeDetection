/*
 * gaussian.c
 *
 * Created on: Apr 16, 2016
 * Author: Seokyong Hong
 */
#include "gaussian.h"

static float compute_probability(p_gaussian descriptor, float *observation);
static void compute_minor_matrix(double **source, double **destination, int row, int col, int dimension);
static double compute_exponent(float *observation, double *mean, double **inversed_covariance, int dimension);

float compute_probability(p_gaussian descriptor, float *observation) {
	double determinant = compute_determinant(descriptor -> covariance, descriptor -> dimension);
	double constant = 1.0 / sqrt(pow(2.0 * M_PI, (double)descriptor -> dimension) * determinant);
	double **inversed_covariance;
	double exponent;
	int index;

	inversed_covariance = (double **)malloc(descriptor -> dimension * sizeof(double *));
	for(index = 0;index < descriptor -> dimension;index++)
		inversed_covariance[index] = (double *)malloc(descriptor -> dimension * sizeof(double));

	inverse_matrix(descriptor -> covariance, inversed_covariance, descriptor -> dimension, determinant);
	exponent = compute_exponent(observation, descriptor -> mean, inversed_covariance, descriptor -> dimension);

	for(index = 0;index < descriptor -> dimension;index++)
		free(inversed_covariance[index]);
	free(inversed_covariance);

	return constant * exp(exponent);
}

double compute_determinant(double **matrix, int dimension) {
	int index;
	double determinant = 0.0;
	double **temp = NULL;

	if(dimension == 1)
		return matrix[0][0];

	if(dimension == 2)
		return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];

	temp = (double **)malloc((dimension - 1) * sizeof(double *));
	for(index = 0;index < dimension - 1;index++)
		temp[index] = (double *)malloc((dimension - 1) * sizeof(double));

	for(index = 0;index < dimension;index++) {
		compute_minor_matrix(matrix, temp, 0, index, dimension);
		determinant += (index % 2 == 1 ? -1.0 : 1.0) * matrix[0][index] * compute_determinant(temp, dimension - 1);
	}

	for(index = 0;index < dimension - 1;index++)
		free(temp[index]);
	free(temp);

	return determinant;
}

void inverse_matrix(double **matrix, double **inversed, int dimension, double determinant) {
	double constant = 1.0;
	double **temp;
	int i, j;

	//if(determinant == 0.0) {
	//	fprintf(stderr, "Determinant is zero.\n");
	//	return;
	//}

	constant /= determinant;
	temp = (double **)malloc((dimension - 1) * sizeof(double *));
	for(i = 0;i < dimension - 1;i++)
		temp[i] = (double *)malloc((dimension - 1) * sizeof(double));

	for(j = 0;j < dimension;j++) {
		for(i = 0;i < dimension;i++) {
			compute_minor_matrix(matrix, temp, j, i, dimension);
			inversed[i][j] = constant * compute_determinant(temp, dimension - 1);
			if((i + j) % 2 == 1) {
				inversed[i][j] = -inversed[i][j];
			}
		}
	}

	for(i = 0;i < dimension - 1;i++)
		free(temp[i]);
	free(temp);
}

void compute_minor_matrix(double **source, double **destination, int row, int col, int dimension) {
	int c_count = 0, r_count = 0;
	int i, j;

	for(i = 0;i < dimension;i++) {
		if(i != row) {
			c_count = 0;
			for(j = 0;j < dimension;j++) {
				if(j != col) {
					destination[r_count][c_count] = source[i][j];
					c_count++;
				}
			}
			r_count++;
		}
	}
}

double compute_exponent(float *observation, double *mean, double **inversed_covariance, int dimension) {
	double temp1[dimension], temp2[dimension];
	int i, j;
	double value = 0.0;

	for(i = 0;i < dimension;i++) {
		temp1[i] = observation[i] - mean[i];
		temp2[i] = 0.0;
	}

	for(j = 0;j < dimension;j++) {
		for(i = 0;i < dimension;i++) {
			temp2[j] += temp1[i] * inversed_covariance[i][j];
		}
	}

	for(i = 0;i < dimension;i++) {
		value += temp2[i] * temp1[i];
	}

	return -(value / 2.0);
}

p_gaussian init_gaussian(int dimension) {
	p_gaussian descriptor = NULL;
	int index;

	if(dimension <= 0) {
		fprintf(stderr, "Dimension must be larger than 0.\n");
		return descriptor;
	}

	descriptor = (p_gaussian)malloc(sizeof(gaussian));
	if(!descriptor) {
		fprintf(stderr, "Cannot allocate memory for gaussian.\n");
		return descriptor;
	}

	descriptor -> dimension = dimension;
	descriptor -> mean = (double *)malloc(dimension * sizeof(double));
	if(!descriptor) {
		fprintf(stderr, "Cannot allocate memory for gaussian means.\n");
		close_gaussian(descriptor);
		return descriptor;
	}

	descriptor -> covariance = (double **)malloc(dimension * sizeof(double));
	for(index = 0;index < dimension;index++)
		descriptor -> covariance[index] = (double *)malloc(dimension * sizeof(double));

	if(!descriptor -> covariance) {
		fprintf(stderr, "Cannot allocate memory for gaussian covariance matrix.\n");
		close_gaussian(descriptor);
		return descriptor;
	}

	return descriptor;
}

int compute_gaussian(p_gaussian descriptor, float *data, int length) {
	float *copied_data;
	int dimension, x, y, index;
	float *pointer, *end;

	if(!descriptor) {
		fprintf(stderr, "Descriptor cannot be NULL.\n");
		return 0;
	}

	if(length <= 0) {
		fprintf(stderr, "Length should be a positive integer.\n");
		return 0;
	}

	copied_data = (float *)malloc(descriptor -> dimension * length * sizeof(float));
	if(!copied_data) {
		fprintf(stderr, "Cannot allocate heap space for data.\n");
		return 0;
	}
	memcpy(copied_data, data, descriptor -> dimension * length * sizeof(float));

	for(dimension = 0;dimension < descriptor -> dimension;dimension++) {
		pointer = copied_data + (dimension * length);
		end = copied_data + ((dimension + 1) * length);

		*(descriptor -> mean + dimension) = 0.0;
		while(pointer < end) {
			*(descriptor -> mean + dimension) += *(pointer++);
		}
		*(descriptor -> mean + dimension) /= (double)length;

		pointer = copied_data + (dimension * length);
		while(pointer < end) {
			*(pointer++) -= *(descriptor -> mean + dimension);
		}
	}

	for(x = 0;x < descriptor -> dimension;x++) {
		for(y = x;y < descriptor -> dimension;y++) {
			double covariance = 0.0;
			for(index = 0;index < length;index++) {
				covariance += *(copied_data + (x * length) + index) * *(copied_data + (y * length) + index);
			}
			covariance /= ((double)length - 1.0);
			descriptor -> covariance[x][y] = covariance;
			descriptor -> covariance[y][x] = covariance;
		}
	}

	free(copied_data);
	return 1;
}

void close_gaussian(p_gaussian descriptor) {
	int index;

	if(descriptor) {
		if(descriptor -> mean)
			free(descriptor -> mean);
		if(descriptor -> covariance) {
			for(index = 0;index < descriptor -> dimension;index++)
				free(descriptor -> covariance[index]);
			free(descriptor -> covariance);
		}
		free(descriptor);
	}
}

void add_random_pixels(float *image1, float *image2, int band, int width, int height, float percentage) {
	int image_size = width * height;
	int pixels = (int)(image_size * percentage);
	int i, j;

	for(i = 0;i < pixels;i++) {
		int random_position1 = rand() % image_size;
		int random_position2 = rand() % image_size;

		for(j = 0;j < band;j++) {
			*(image1 + random_position1 + image_size * j) = (float)(rand() % 256);
			*(image2 + random_position2 + image_size * j) = (float)(rand() % 256);
		}
	}
}
