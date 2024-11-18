/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenheim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:06:09 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 18:33:46 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	place_pixel_in_img(t_img_data img_data, int x, int y, int color)
{
	int	i;
	int	bytes_per_pixel;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	bytes_per_pixel = img_data.bits_per_pixel / 8;
	i = (x * bytes_per_pixel) + (y * WIDTH * bytes_per_pixel);
	if (img_data.endian == 0)
	{
		img_data.data[i] = color & 0xFF;
		img_data.data[i + 1] = (color >> 8) & 0xFF;
		img_data.data[i + 2] = (color >> 16) & 0xFF;
		img_data.data[i + 3] = (color >> 24) & 0xFF;
	}
	else
	{
		img_data.data[i] = (color >> 24) & 0xFF;
		img_data.data[i + 1] = (color >> 16) & 0xFF;
		img_data.data[i + 2] = (color >> 8) & 0xFF;
		img_data.data[i + 3] = color & 0xFF;
	}
}

static void	init_line_data(t_line_data *line)
{
	line->dx = abs(line->end.x - line->start.x);
	line->dy = abs(line->end.y - line->start.y);
	if (line->start.x < line->end.x)
		line->sx = 1;
	else
		line->sx = -1;
	if (line->start.y < line->end.y)
		line->sy = 1;
	else
		line->sy = -1;
	line->err = line->dx - line->dy;
}

static void	plot_point(t_line_data line, int step, int stepsmax, t_img_data img)
{
	double	percentage;
	int		color;

	percentage = (double)step / stepsmax;
	color = interpolate_color(line.start_color, line.end_color, percentage);
	place_pixel_in_img(img, line.start.x, line.start.y, color);
}

void	bresenheim_line(t_line_data line, t_img_data img_data)
{
	int	total_steps;
	int	step;
	int	e2;

	init_line_data(&line);
	total_steps = fmax(line.dx, line.dy);
	step = 0;
	while (1)
	{
		plot_point(line, step, total_steps, img_data);
		if (line.start.x == line.end.x && line.start.y == line.end.y)
			break ;
		e2 = line.err * 2;
		if (e2 > -line.dy)
		{
			line.err -= line.dy;
			line.start.x += line.sx;
		}
		if (e2 < line.dx)
		{
			line.err += line.dx;
			line.start.y += line.sy;
		}
		step++;
	}
}
