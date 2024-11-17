/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:48:20 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 17:53:12 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_vector2	calculate_center_offset(t_vector2 **projections, t_data data)
{
	int			x;
	int			y;
	t_vector2	offset;

	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			getset_max_x(projections[y][x].x, 0);
			getset_min_x(projections[y][x].x, 0);
			getset_max_y(projections[y][x].y, 0);
			getset_min_y(projections[y][x].y, 0);
			x++;
		}
		y++;
	}
	offset.x = WIDTH / 2 - ((getset_min_x(0, 1) + getset_max_x(0, 1)) / 2);
	offset.y = HEIGHT / 2 - ((getset_min_y(0, 1) + getset_max_y(0, 1)) / 2);
	return (offset);
}

static t_vector2	**translate_projections(t_vector2 **projections, t_data data)
{
	int			x;
	int			y;
	t_vector2	center_offset;

	center_offset = calculate_center_offset(projections, data);
	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			projections[y][x] = translate(projections[y][x], center_offset.x + data.tx, center_offset.y + data.ty);
			x++;
		}
		y++;
	}
	return (projections);
}

static t_vector2 **get_projections(t_data data)
{
	int i;
	int x;
	int y;
	t_vector2 **projections;

	i = 0;
	while (data.heightmap[i])
		i++;
	projections = malloc(sizeof(t_vector2*) * i);
	if (!projections)
	{
		perror("get_projections : allocation");
		exit(EXIT_FAILURE);
	}
	y = 0;
	while (data.heightmap[y])
	{
		projections[y] = malloc(sizeof(t_vector2) * data.x_size);
		if (!projections[y])
		{
			perror("get_projections : row allocation");
			exit(EXIT_FAILURE);
		}
		x = 0;
		while (x < data.x_size)
		{
			projections[y][x] = data.project(x, y, data);
			x++;
		}
		y++;
	}
	return (translate_projections(projections, data));
}
void draw(t_data data)
{
	int x;
	int y;
	t_vector2 **projections;
	t_line_data line_data;
	
	projections = get_projections(data);
	clear_img(data.img_data.data);
	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			if (x < data.x_size - 1)
			{
				line_data.start = projections[y][x];
				line_data.end = projections[y][x + 1];
				line_data.start_color = data.heightmap[y][x].color;
				line_data.end_color = data.heightmap[y][x + 1].color;
				bresenheim_line(line_data, data.img_data);
			}
			if (data.heightmap[y + 1])
			{
				line_data.start = projections[y][x];
				line_data.end = projections[y + 1][x];
				line_data.start_color = data.heightmap[y][x].color;
				line_data.end_color = data.heightmap[y + 1][x].color;
				bresenheim_line(line_data, data.img_data);
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data.mlx_ptr, data.mlx_win, data.img_data.img, 0, 0);
	print_tooltips(data);
	for (int y = 0; data.heightmap[y]; y++)
		free(projections[y]);
	free(projections);
}
