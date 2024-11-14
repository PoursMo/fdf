/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:53:27 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/14 15:04:15 by aloubry          ###   ########.fr       */
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

typedef struct s_img_data
{
	void *img;
	char *data;
	int bits_per_pixel;
	int size_line;
	int endian;
} t_img_data;

//utils
int try_open(char *file, int flag);
void try_close(int fd);
void free_split(char **split);

//parse
int **parse_map(char *map_file, int *x_size);

#endif