/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:38:01 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 17:44:54 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	zoom(int keycode, t_data *data)
{
	if (keycode == 61)
		data->zoom += 1;
	else if (keycode == 45)
		data->zoom -= 1;
	draw(*data);
}

static void	translation(int keycode, t_data *data)
{
	if (keycode == 119)
		data->ty -= 20;
	else if (keycode == 97)
		data->tx -= 20;
	else if (keycode == 115)
		data->ty += 20;
	else if (keycode == 100)
		data->tx += 20;
	draw(*data);
}

static void	rotation(int keycode, t_data *data)
{
	if (keycode == 110)
		data->angle_z -= 5.0f;
	else if (keycode == 109)
		data->angle_z += 5.0f;
	else if (keycode == 104)
		data->angle_y -= 5.0f;
	else if (keycode == 106)
		data->angle_y += 5.0f;
	else if (keycode == 121)
		data->angle_x -= 5.0f;
	else if (keycode == 117)
		data->angle_x += 5.0f;
	draw(*data);
}

int	handle_key(int keycode, t_data *data)
{
	if (keycode == 65307)
		terminate(*data);
	else if ((keycode == 61 && data->zoom < 100)
		|| (keycode == 45 && data->zoom > 1))
		zoom(keycode, data);
	else if (keycode == 119 || keycode == 97
		|| keycode == 115 || keycode == 100)
		translation(keycode, data);
	else if (keycode == 110 || keycode == 109 || keycode == 104
		|| keycode == 106 || keycode == 121 || keycode == 117)
		rotation(keycode, data);
	else if (keycode == 112)
	{
		data->angle_x = 0;
		data->angle_y = 0;
		data->angle_z = 0;
		data->project = (data->project == perspective_project) ? isometric_project : perspective_project;
		draw(*data);
	}
	return (0);
}

void	print_tooltips(t_data data)
{
	static char	*tooltips[] = {
		"+/- : zoom",
		"w/a/s/d : move",
		"y/u : rotate x",
		"h/j : rotate y",
		"n/m : rotate z",
		NULL
	};
	int			y;
	int			i;

	y = 15;
	i = 0;
	while (tooltips[i])
	{
		mlx_string_put(data.mlx_ptr, data.mlx_win, 10, y,
			get_color(100, 255, 100), tooltips[i]);
		y += 15;
		i++;
	}
}
