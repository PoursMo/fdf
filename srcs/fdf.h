/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:53:27 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/16 14:39:40 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "mlx.h"
# include "mlx_int.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <limits.h>

# define WIDTH 800
# define HEIGHT 600

typedef struct s_vector2
{
	int x;
	int y;
} t_vector2;

typedef struct s_point
{
	int z;
	int color;
} t_point;

typedef struct s_img_data
{
	void *img;
	char *data;
	int bits_per_pixel;
	int size_line;
	int endian;
} t_img_data;

typedef struct s_data
{
	t_point **heightmap;
	t_img_data img_data;
	void *mlx_ptr;
	void *mlx_win;
	int x_size;
	int tx;
	int ty;
	int zoom;
	double angle_x;
	double angle_y;
	double angle_z;
	t_vector2 (*project)(int x, int y, struct s_data data);
} t_data;

//fdf
void draw(t_data data);

//input
int handle_key(int keycode, t_data *data);
void print_tooltips(t_data data);

//color
int get_color(int red, int green, int blue);
int interpolate_color(int start_color, int end_color, double ratio);

//projection
t_vector2 isometric_project(int x, int y, t_data data);
t_vector2 perspective_project(int x, int y, t_data data);

//image
void create_img(void *mlx, t_img_data *data);
void clear_img(char *data);

//utils
int try_open(char *file, int flag);
void try_close(int fd);
void free_split(char **split);
int terminate();

//parse
void parse_map(char *map_file, t_data *data);

#endif