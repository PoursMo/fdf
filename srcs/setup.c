/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:07:50 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 18:04:37 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	get_zoom(void)
{
	int	zoom_x;
	int	zoom_y;

	zoom_x = WIDTH / (getset_max_x(0, 1) - getset_min_x(0, 1));
	zoom_y = HEIGHT / (getset_max_y(0, 1) - getset_min_y(0, 1));
	if (zoom_x < zoom_y)
	{
		if (zoom_x < 1)
			return (1);
		else
			return (zoom_x);
	}
	else
	{
		if (zoom_y < 1)
			return (1);
		else
			return (zoom_y);
	}
}

static int	calculate_initial_zoom(t_data data)
{
	int			x;
	int			y;
	t_vector2	projection;

	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			projection = data.project(x, y, data);
			getset_min_x(projection.x, 0);
			getset_max_x(projection.x, 0);
			getset_min_y(projection.y, 0);
			getset_max_y(projection.y, 0);
			x++;
		}
		y++;
	}
	return (get_zoom());
}

static void	set_initial_values(t_data *data)
{
	data->zoom = 1;
	data->angle_x = 0;
	data->angle_y = 0;
	data->angle_z = 0;
	data->tx = 0;
	data->ty = 0;
	data->project = isometric_project;
}

void	setup(t_data *data, char *file)
{
	t_img_data	img_data;

	parse_map(file, data);
	data->mlx_ptr = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx_ptr, WIDTH, HEIGHT, "fdf");
	create_img(data->mlx_ptr, &img_data);
	data->img_data = img_data;
	set_initial_values(data);
	data->zoom = calculate_initial_zoom(*data);
}
