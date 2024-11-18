/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:53:27 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 18:07:17 by aloubry          ###   ########.fr       */
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

# define WIDTH 1920
# define HEIGHT 1080

typedef struct s_vector2
{
	int	x;
	int	y;
}	t_vector2;

typedef struct s_point
{
	int	z;
	int	color;
}	t_point;

typedef struct s_img_data
{
	void	*img;
	char	*data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}	t_img_data;

typedef struct s_proj_data
{
	double	rotated_x;
	double	rotated_y;
	double	rotated_z;
	double	final_x;
	double	final_y;
}	t_proj_data;

typedef struct s_line_data
{
	t_vector2	start;
	t_vector2	end;
	int			start_color;
	int			end_color;
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			err;
}	t_line_data;

typedef struct s_data	t_data;
typedef t_vector2		(*t_project_func)(int x, int y, t_data data);
struct s_data
{
	t_point			**heightmap;
	t_img_data		img_data;
	void			*mlx_ptr;
	void			*mlx_win;
	int				x_size;
	int				tx;
	int				ty;
	int				zoom;
	double			angle_x;
	double			angle_y;
	double			angle_z;
	t_project_func	project;
};

// angles
double		deg_to_rad(double degrees);
double		getset_rad_x(t_data *data);
double		getset_rad_y(t_data *data);
double		getset_rad_z(t_data *data);

// fdf
void		draw(t_data data);

// minmax
int			getset_min_x(int value, char reset);
int			getset_max_x(int value, char reset);
int			getset_min_y(int value, char reset);
int			getset_max_y(int value, char reset);

// setup
void		setup(t_data *data, char *file);

// input
int			handle_key(int keycode, t_data *data);

// color
int			get_color(int red, int green, int blue);
int			interpolate_color(int start_color, int end_color, double ratio);
void		change_colors(t_data *data);

// projection
t_vector2	isometric_project(int x, int y, t_data data);
t_vector2	orthographic_project(int x, int y, t_data data);

// image
void		create_img(void *mlx, t_img_data *data);
void		clear_img(char *data);

// utils
int			terminate(void);
int			try_open(char *file, int flags);
void		try_close(int fd);
void		free_map(t_point **map);
void		free_split(char **split);

// draw1
void		draw(t_data data);

// draw2
t_vector2	**get_projections(t_data data);

// bresenheim
void		bresenheim_line(t_line_data line_data, t_img_data img_data);

// parse
void		parse_map(char *map_file, t_data *data);

#endif