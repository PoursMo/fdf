/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:38:01 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 16:41:27 by aloubry          ###   ########.fr       */
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
	double	angle;

	if (data->project == isometric_project)
		angle = 5.0f;
	else
		angle = 90.0f;
	if (keycode == 110)
		data->angle_z -= angle;
	else if (keycode == 109)
		data->angle_z += angle;
	else if (keycode == 104)
		data->angle_y -= angle;
	else if (keycode == 106)
		data->angle_y += angle;
	else if (keycode == 121)
		data->angle_x -= angle;
	else if (keycode == 117)
		data->angle_x += angle;
	getset_rad_x(data);
	getset_rad_y(data);
	getset_rad_z(data);
	draw(*data);
}

static void	projection(t_data *data)
{
	data->angle_x = 0;
	data->angle_y = 0;
	data->angle_z = 0;
	getset_rad_x(data);
	getset_rad_y(data);
	getset_rad_z(data);
	if (data->project == isometric_project)
		data->project = orthographic_project;
	else
		data->project = isometric_project;
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
		projection(data);
	else if (keycode == 99)
	{
		change_colors(data);
		draw(*data);
	}
	return (0);
}
