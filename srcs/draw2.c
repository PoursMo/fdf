/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:59:59 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 20:00:45 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_vector2	translate(t_vector2 point, int tx, int ty)
{
	t_vector2	translated;

	translated.x = point.x + tx;
	translated.y = point.y + ty;
	return (translated);
}

static t_vector2	calc_center_offset(t_vector2 **projections, t_data data)
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

static t_vector2	**translate_projections(t_vector2 **projs, t_data data)
{
	int			x;
	int			y;
	t_vector2	center_offset;

	center_offset = calc_center_offset(projs, data);
	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			projs[y][x] = translate(projs[y][x], center_offset.x + data.tx,
					center_offset.y + data.ty);
			x++;
		}
		y++;
	}
	return (projs);
}

static void	allocate_and_project_row(t_data data, t_vector2 **projs, int y)
{
	int	x;

	projs[y] = malloc(sizeof(t_vector2) * data.x_size);
	if (!projs[y])
	{
		perror("get_projections : row allocation");
		exit(EXIT_FAILURE);
	}
	x = 0;
	while (x < data.x_size)
	{
		projs[y][x] = data.project(x, y, data);
		x++;
	}
}

t_vector2	**get_projections(t_data data)
{
	int			i;
	int			y;
	t_vector2	**projections;

	i = 0;
	while (data.heightmap[i])
		i++;
	projections = malloc(sizeof(t_vector2 *) * i);
	if (!projections)
	{
		perror("get_projections : allocation");
		exit(EXIT_FAILURE);
	}
	y = 0;
	while (data.heightmap[y])
		allocate_and_project_row(data, projections, y++);
	return (translate_projections(projections, data));
}
