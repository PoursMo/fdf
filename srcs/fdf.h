/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:53:27 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/08 16:56:25 by aloubry          ###   ########.fr       */
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

# define WIDTH 500
# define HEIGHT 500
# define SCALE 20

typedef struct s_vector2
{
	int x;
	int y;
} t_vector2;

typedef struct s_point
{
	int z;
	t_vector2 projection;
} t_point;

//utils
int try_open(char *file, int flag);
void try_close(int fd);
void free_split(char **split);

//parse
t_point **parse_map(char *map_file);


#endif