/*
 * main.c
 *
 * Created on: Mar 8, 2016
 * Author: Seokyong Hong
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mpi.h"
#include "params.h"
#include "image/image.h"
#include "distance/kld.h"
#include "cluster/gmm_em.h"
#include "gaussian/gaussian.h"

#define RANDOM_PIXELS		0.1F
#define OUTPUT_PATH			"/home/phantom/Desktop/Output.tif"

static void print_information(void) {
	printf("[Usage]\n");
	printf("./HybridChangeDetection path1 path2 kld_output_path window_width window_height margin_top margin_left margin_bottom margin_right\n");
}

static int file_exists(char *path) {
	struct stat st;
	int result = stat(path, &st);

	return result == 0;
}

int create_parameters(param_struct *parameters, MPI_Datatype *type, int argc, char *argv[], int rank) {
	MPI_Aint base, first, second, third, fourth, fifth, sixth, seventh, eighth;
	MPI_Aint displacements[9];
	MPI_Datatype datatypes[9];
	int block_lengths[9];

	if(rank == 0) {
		if(argc != 10) {
			print_information();
			return 0;
		}
		if(!file_exists(argv[1])) {
			fprintf(stderr, "File %s does not exist.\n", argv[1]);
			return 0;
		}
		if(!file_exists(argv[2])) {
			fprintf(stderr, "File %s does not exist.\n", argv[2]);
			return 0;
		}
		if(strlen(argv[1]) > PATH_LENGTH || strlen(argv[2]) > PATH_LENGTH || strlen(argv[3]) > PATH_LENGTH) {
			fprintf(stderr, "The length of file path must be less than or equal to %d.\n", PATH_LENGTH);
			return 0;
		}
		strcpy(parameters -> path1, argv[1]);
		strcpy(parameters -> path2, argv[2]);
		strcpy(parameters -> kld_output_path, argv[3]);
		parameters -> window_width = atoi(argv[4]);
		parameters -> window_height = atoi(argv[5]);
		parameters -> margin_top = atoi(argv[6]);
		parameters -> margin_left = atoi(argv[7]);
		parameters -> margin_bottom = atoi(argv[8]);
		parameters -> margin_right = atoi(argv[9]);


		if(parameters -> window_width <= 0 || parameters -> window_height <= 0) {
			fprintf(stderr, "Window width and height must be greater than 0.");
			return 0;
		}
	}
	MPI_Get_address(parameters, &base);
	MPI_Get_address(&parameters -> window_height, &first);
	MPI_Get_address(&parameters -> margin_left, &second);
	MPI_Get_address(&parameters -> margin_right, &third);
	MPI_Get_address(&parameters -> margin_top, &fourth);
	MPI_Get_address(&parameters -> margin_bottom, &fifth);
	MPI_Get_address(&parameters -> path1, &sixth);
	MPI_Get_address(&parameters -> path2, &seventh);
	MPI_Get_address(&parameters -> kld_output_path, &eighth);
	displacements[0] = 0;
	displacements[1] = first - base;
	displacements[2] = second - base;
	displacements[3] = third - base;
	displacements[4] = fourth - base;
	displacements[5] = fifth - base;
	displacements[6] = sixth - base;
	displacements[7] = seventh - base;
	displacements[8] = eighth - base;
	datatypes[0] = MPI_INT;
	datatypes[1] = MPI_INT;
	datatypes[2] = MPI_INT;
	datatypes[3] = MPI_INT;
	datatypes[4] = MPI_INT;
	datatypes[5] = MPI_INT;
	datatypes[6] = MPI_CHAR;
	datatypes[7] = MPI_CHAR;
	datatypes[8] = MPI_CHAR;
	block_lengths[0] = 1;
	block_lengths[1] = 1;
	block_lengths[2] = 1;
	block_lengths[3] = 1;
	block_lengths[4] = 1;
	block_lengths[5] = 1;
	block_lengths[6] = PATH_LENGTH;
	block_lengths[7] = PATH_LENGTH;
	block_lengths[8] = PATH_LENGTH;

	MPI_Type_create_struct(9, block_lengths, displacements, datatypes, type);
	return 1;
}


int main(int argc, char *argv[]) {
	int processes, rank;
	param_struct parameters;
	MPI_Datatype parameter_type;
	MPI_File file;
	MPI_Status status;
	image_handle *image_t1, *image_t2;
	double *klds;
	bounding_box window;
	float *grid_t1, *grid_t2;
	int index;
	p_gaussian gaussian_t1, gaussian_t2;
	_clusters clusters;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(!create_parameters(&parameters, &parameter_type, argc, argv, rank)) {
		MPI_Finalize();
		return 0;
	}

	MPI_Type_commit(&parameter_type);
	MPI_Bcast(&parameters, 1, parameter_type, 0, MPI_COMM_WORLD);
	GDALAllRegister();

	image_t1 = open_image(parameters.path1);
	image_t2 = open_image(parameters.path2);

	if(!image_t1 || !image_t2) {
		if(image_t1)
			close_image(image_t1);
		if(image_t2)
			close_image(image_t2);
		MPI_Type_free(&parameter_type);
		MPI_Finalize();
		return 0;
	}

	int size = load_images(image_t1, image_t2, &parameters, processes, rank);
	if(size < 0) {
		close_image(image_t1);
		close_image(image_t2);
		MPI_Type_free(&parameter_type);
		MPI_Finalize();
		return 0;
	}

	klds = (double *)malloc(size * sizeof(double));
	if(!klds) {
		fprintf(stderr, "Cannot allocate memory for KLDs.\n");
		close_image(image_t1);
		close_image(image_t2);
		MPI_Type_free(&parameter_type);
		MPI_Finalize();
		return 0;
	}

	for(index = 0;index < size;index++) {
		window.height = parameters.window_height;
		window.width = parameters.window_width;
		window.left = index % image_t1 -> internal_width + parameters.margin_left - window.width / 2;
		//window.top = index / image_t1 -> internal_width + parameters.margin_top - window.height / 2;
		window.top = index / image_t1 -> internal_width;

		grid_t1 = retrieve_subimage(image_t1, window);
		grid_t2 = retrieve_subimage(image_t2, window);
		gaussian_t1 = init_gaussian(image_t1 -> bands);
		gaussian_t2 = init_gaussian(image_t2 -> bands);
		compute_gaussian(gaussian_t1, grid_t1, window.width * window.height);
		compute_gaussian(gaussian_t2, grid_t2, window.width * window.height);
		klds[index] = compute_symmetric_kld(gaussian_t1, gaussian_t2);

		while(isnan(klds[index])) {
			//fprintf(stderr, "Since KLD is NaN, it is recalculated.\n");
			add_random_pixels(grid_t1, grid_t2, image_t1 -> bands, window.width, window.height, RANDOM_PIXELS);
			compute_gaussian(gaussian_t1, grid_t1, window.width * window.height);
			compute_gaussian(gaussian_t2, grid_t2, window.width * window.height);
			klds[index] = compute_symmetric_kld(gaussian_t1, gaussian_t2);
		}

		free(grid_t1);
		free(grid_t2);
		close_gaussian(gaussian_t1);
		close_gaussian(gaussian_t2);
	}

	MPI_File_open(MPI_COMM_WORLD, (void *)parameters.kld_output_path, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
	MPI_File_write_ordered(file, klds, size, MPI_DOUBLE, &status);
	MPI_File_close(&file);

	MPI_Barrier(MPI_COMM_WORLD);
	/*
	if(rank == 0) {
		cluster_gmm_em(&clusters, klds, size, 1, 15, 15);
		save_change_map(OUTPUT_PATH, &clusters, klds, parameters.margin_top, parameters.margin_left, parameters.margin_bottom, parameters.margin_right, image_t1, image_t2);
		close_gmm_em(&clusters);
	}
	*/
	free(klds);
	close_image(image_t1);
	close_image(image_t2);
	MPI_Type_free(&parameter_type);
	MPI_Finalize();
	return 0;
}

