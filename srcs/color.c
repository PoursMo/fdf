/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:22:01 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/16 12:25:47 by aloubry          ###   ########.fr       */
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
