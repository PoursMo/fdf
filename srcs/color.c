/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:22:01 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 14:09:13 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	get_color(int red, int green, int blue)
{
	return ((red << 16) | (green << 8) | blue);
}

int	interpolate_color(int start_color, int end_color, double ratio)
{
	int	red;
	int	green;
	int	blue;

	red = ((start_color >> 16) & 0xFF)
		+ (((end_color >> 16) & 0xFF) - ((start_color >> 16) & 0xFF)) * ratio;
	green = ((start_color >> 8) & 0xFF)
		+ (((end_color >> 8) & 0xFF) - ((start_color >> 8) & 0xFF)) * ratio;
	blue = (start_color & 0xFF)
		+ ((end_color & 0xFF) - (start_color & 0xFF)) * ratio;
	return (get_color(red, green, blue));
}

static int	shift_col(int color)
{
	int	new_red;
	int	new_green;
	int	new_blue;

	new_red = (((color << 8) & 0xFF0000) >> 16) & 0xFF;
	new_green = (((color << 8) & 0x00FF00) >> 8) & 0xFF;
	new_blue = ((color >> 16) & 0x0000FF) & 0xFF;
	return (get_color(new_red, new_green, new_blue));
}

void	change_colors(t_data *dat)
{	
	int	x;
	int	y;

	y = 0;
	while (dat->heightmap[y])
	{
		x = 0;
		while (x < dat->x_size)
		{
			dat->heightmap[y][x].color = shift_col(dat->heightmap[y][x].color);
			x++;
		}
		y++;
	}
}
