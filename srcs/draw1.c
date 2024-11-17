/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:48:20 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 20:16:17 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_line_data	pld(t_vector2 start, t_vector2 end, int st_col, int end_col)
{
	t_line_data	line_data;

	line_data.start = start;
	line_data.end = end;
	line_data.start_color = st_col;
	line_data.end_color = end_col;
	return (line_data);
}

static void	fill_img(t_data data, t_vector2 **projs)
{
	int			x;
	int			y;

	y = 0;
	while (data.heightmap[y])
	{
		x = 0;
		while (x < data.x_size)
		{
			if (x < data.x_size - 1)
				bresenheim_line(pld(projs[y][x], projs[y][x + 1],
						data.heightmap[y][x].color,
						data.heightmap[y][x + 1].color), data.img_data);
			if (data.heightmap[y + 1])
				bresenheim_line(pld(projs[y][x], projs[y + 1][x],
						data.heightmap[y][x].color,
						data.heightmap[y + 1][x].color), data.img_data);
			x++;
		}
		y++;
	}
}

void	draw(t_data data)
{
	t_vector2	**projections;
	int			y;

	projections = get_projections(data);
	clear_img(data.img_data.data);
	fill_img(data, projections);
	mlx_put_image_to_window(data.mlx_ptr, data.mlx_win,
		data.img_data.img, 0, 0);
	print_tooltips(data);
	y = 0;
	while (data.heightmap[y])
		free(projections[y++]);
	free(projections);
}
