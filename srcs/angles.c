/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:32:20 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 15:32:33 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

double	deg_to_rad(double degrees)
{
	return (degrees * (M_PI / 180.0f));
}

double	getset_rad_x(t_data *data)
{
	static double	rad_x;

	if (data)
		rad_x = deg_to_rad(data->angle_x);
	return (rad_x);
}

double	getset_rad_y(t_data *data)
{
	static double	rad_y;

	if (data)
		rad_y = deg_to_rad(data->angle_y);
	return (rad_y);
}

double	getset_rad_z(t_data *data)
{
	static double	rad_z;

	if (data)
		rad_z = deg_to_rad(data->angle_z);
	return (rad_z);
}
