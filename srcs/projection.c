/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:25:17 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 15:30:04 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vector2	isometric_project(int x, int y, t_data data)
{
	t_vector2	proj;
	t_proj_data	proj_data;

	proj_data.rotated_y = y * cos(getset_rad_x(NULL))
		- data.heightmap[y][x].z * sin(getset_rad_x(NULL));
	proj_data.rotated_z = y * sin(getset_rad_x(NULL))
		+ data.heightmap[y][x].z * cos(getset_rad_x(NULL));
	proj_data.rotated_x = x * cos(getset_rad_y(NULL))
		+ proj_data.rotated_z * sin(getset_rad_y(NULL));
	proj_data.rotated_z = -x * sin(getset_rad_y(NULL))
		+ proj_data.rotated_z * cos(getset_rad_y(NULL));
	proj_data.final_x = proj_data.rotated_x * cos(getset_rad_z(NULL))
		- proj_data.rotated_y * sin(getset_rad_z(NULL));
	proj_data.final_y = proj_data.rotated_x * sin(getset_rad_z(NULL))
		+ proj_data.rotated_y * cos(getset_rad_z(NULL));
	proj.x = ((proj_data.final_x - proj_data.final_y)
			* cos(deg_to_rad(30)) * data.zoom);
	proj.y = ((proj_data.final_x + proj_data.final_y) * sin(deg_to_rad(30))
			* data.zoom) - (proj_data.rotated_z * data.zoom);
	return (proj);
}

t_vector2	orthographic_project(int x, int y, t_data data)
{
	t_vector2	proj;
	t_proj_data	proj_data;

	proj_data.rotated_y = y * cos(getset_rad_x(NULL))
		- data.heightmap[y][x].z * sin(getset_rad_x(NULL));
	proj_data.rotated_z = y * sin(getset_rad_x(NULL))
		+ data.heightmap[y][x].z * cos(getset_rad_x(NULL));
	proj_data.rotated_x = x * cos(getset_rad_y(NULL))
		+ proj_data.rotated_z * sin(getset_rad_y(NULL));
	proj_data.rotated_z = -x * sin(getset_rad_y(NULL))
		+ proj_data.rotated_z * cos(getset_rad_y(NULL));
	proj_data.final_x = proj_data.rotated_x * cos(getset_rad_z(NULL))
		- proj_data.rotated_y * sin(getset_rad_z(NULL));
	proj_data.final_y = proj_data.rotated_x * sin(getset_rad_z(NULL))
		+ proj_data.rotated_y * cos(getset_rad_z(NULL));
	proj.x = proj_data.final_x * data.zoom;
	proj.y = proj_data.final_y * data.zoom;
	return (proj);
}
