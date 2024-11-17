#include "fdf.h"

static double deg_to_rad(double degrees)
{
	return (degrees * (M_PI / 180.0f));
}

t_vector2 isometric_project(int x, int y, t_data data)
{
    t_vector2 projection;
    double rad_x = deg_to_rad(data.angle_x);
    double rad_y = deg_to_rad(data.angle_y);
    double rad_z = deg_to_rad(data.angle_z);

    // Apply rotation around the x-axis
    double rotated_y = y * cos(rad_x) - data.heightmap[y][x].z * sin(rad_x);
    double rotated_z = y * sin(rad_x) + data.heightmap[y][x].z * cos(rad_x);

    // Apply rotation around the y-axis
    double rotated_x = x * cos(rad_y) + rotated_z * sin(rad_y);
    rotated_z = -x * sin(rad_y) + rotated_z * cos(rad_y);

    // Apply rotation around the z-axis
    double final_x = rotated_x * cos(rad_z) - rotated_y * sin(rad_z);
    double final_y = rotated_x * sin(rad_z) + rotated_y * cos(rad_z);

    // Apply isometric projection
    projection.x = ((final_x - final_y) * cos(deg_to_rad(30)) * data.zoom);
    projection.y = ((final_x + final_y) * sin(deg_to_rad(30)) * data.zoom) - (rotated_z * data.zoom);
    return projection;
}

t_vector2 perspective_project(int x, int y, t_data data)
{
    t_vector2 projection;
    double rad_x = deg_to_rad(data.angle_x);
    double rad_y = deg_to_rad(data.angle_y);
    double rad_z = deg_to_rad(data.angle_z);

    // Apply rotation around the x-axis
    double rotated_y = y * cos(rad_x) - data.heightmap[y][x].z * sin(rad_x);
    double rotated_z = y * sin(rad_x) + data.heightmap[y][x].z * cos(rad_x);

    // Apply rotation around the y-axis
    double rotated_x = x * cos(rad_y) + rotated_z * sin(rad_y);
    rotated_z = -x * sin(rad_y) + rotated_z * cos(rad_y);

    // Apply rotation around the z-axis
    double final_x = rotated_x * cos(rad_z) - rotated_y * sin(rad_z);
    double final_y = rotated_x * sin(rad_z) + rotated_y * cos(rad_z);

    // Apply orthographic projection
    projection.x = final_x * data.zoom;
    projection.y = final_y * data.zoom;
    return projection;
}
