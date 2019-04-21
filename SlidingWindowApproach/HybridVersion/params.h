/*
 * params.h
 *
 * Created on: Mar 8, 2016
 * Author: Seokyong Hong
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#define PATH_LENGTH		128

typedef struct _param_struct {
	int		window_width;
	int		window_height;
	int		margin_left;
	int		margin_right;
	int		margin_top;
	int		margin_bottom;
	char	path1[PATH_LENGTH];
	char	path2[PATH_LENGTH];
	char	kld_output_path[PATH_LENGTH];
} param_struct;

typedef struct {
	unsigned int	top;
	unsigned int	left;
	unsigned int	width;
	unsigned int	height;
} bounding_box;

#endif /* PARAMS_H_ */
