/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:53:27 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/15 23:44:16 by aloubry          ###   ########.fr       */
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
	double zoom;
	double angle_x;
	double angle_y;
	double angle_z;
	t_vector2 (*project)(int x, int y, struct s_data data);
} t_data;

//image
void create_img(void *mlx, t_img_data *data);
void clear_img(char *data);

//utils
int try_open(char *file, int flag);
void try_close(int fd);
void free_split(char **split);

//parse
void parse_map(char *map_file, t_data *data);

#endif