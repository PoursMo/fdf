#include "fdf.h"

int get_color(int red, int green, int blue)
{
	return ((red << 16) | (green << 8) | blue);
}

int interpolate_color(int start_color, int end_color, double percentage)
{
	int red = ((start_color >> 16) & 0xFF) + (((end_color >> 16) & 0xFF) - ((start_color >> 16) & 0xFF)) * percentage;
	int green = ((start_color >> 8) & 0xFF) + (((end_color >> 8) & 0xFF) - ((start_color >> 8) & 0xFF)) * percentage;
	int blue = (start_color & 0xFF) + ((end_color & 0xFF) - (start_color & 0xFF)) * percentage;
	return (get_color(red, green, blue));
}

double deg_to_rad(double degrees)
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

void calculate_bounding_box(t_data data, int *min_x, int *max_x, int *min_y, int *max_y)
{
    *min_x = INT_MAX;
    *max_x = INT_MIN;
    *min_y = INT_MAX;
    *max_y = INT_MIN;

    for (int y = 0; data.heightmap[y]; y++)
    {
        for (int x = 0; x < data.x_size; x++)
        {
            t_vector2 projection = isometric_project(x, y, data);
            if (projection.x < *min_x) *min_x = projection.x;
            if (projection.x > *max_x) *max_x = projection.x;
            if (projection.y < *min_y) *min_y = projection.y;
            if (projection.y > *max_y) *max_y = projection.y;
        }
    }
}

void calculate_center(int min_x, int max_x, int min_y, int max_y, int *center_x, int *center_y)
{
    *center_x = (min_x + max_x) / 2;
    *center_y = (min_y + max_y) / 2;
}

void place_pixel_in_img(char *data, int x, int y, int color, int endian)
{
    int i;

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    i = (x * 4) + (y * WIDTH * 4);

    if (endian == 0)
    {
        data[i] = color & 0xFF;
        data[i + 1] = (color >> 8) & 0xFF;
        data[i + 2] = (color >> 16) & 0xFF;
        data[i + 3] = (color >> 24) & 0xFF;
    }
    else
    {
        data[i] = (color >> 24) & 0xFF;
        data[i + 1] = (color >> 16) & 0xFF;
        data[i + 2] = (color >> 8) & 0xFF;
        data[i + 3] = color & 0xFF;
    }
}

void bresenheim_line(t_vector2 start, t_vector2 end, t_img_data img_data, int start_color, int end_color)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;
	int total_steps = (dx > dy) ? dx : dy; 

	int step = 0;
	while (1)
	{
		double percentage = (double)step / total_steps;
		int color = interpolate_color(start_color, end_color, percentage);
		place_pixel_in_img(img_data.data, start.x, start.y, color, img_data.endian);
		if (start.x == end.x && start.y == end.y)
			break;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			start.x += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			start.y += sy;
		}
		step++;
	}
}



int terminate()
{
	exit(0);
}

t_vector2 translate(t_vector2 point, int tx, int ty)
{
    t_vector2 translated;
    translated.x = point.x + tx;
    translated.y = point.y + ty;
    return (translated);
}

void print_tooltips(t_data data)
{
	static char *tooltips[] = {"+/- : zoom", "w/a/s/d : move", "y/u : rotate x", "h/j : rotate y", "n/m : rotate z", NULL};
	int y;
	int i;

	y = 15;
	i = 0;
	while(tooltips[i])
	{
		mlx_string_put(data.mlx_ptr, data.mlx_win, 10, y, get_color(100, 255, 100), tooltips[i]);
		y += 15;
		i++;
	}
}

void draw(t_data data)
{
	int min_x, max_x, min_y, max_y;
    calculate_bounding_box(data, &min_x, &max_x, &min_y, &max_y);
    int center_x, center_y;
    calculate_center(min_x, max_x, min_y, max_y, &center_x, &center_y);
	int offsetx = WIDTH / 2 - center_x;
	int offsety = HEIGHT / 2 - center_y;

	clear_img(data.img_data.data);
	for (int y = 0; data.heightmap[y]; y++)
	{
		for (int x = 0; x < data.x_size; x++)
		{
			t_vector2 projection = isometric_project(x, y, data);
			projection = translate(projection, data.tx + offsetx, data.ty + offsety);
			if (x < data.x_size - 1)
			{
				t_vector2 next_projection = isometric_project(x + 1, y, data);
				next_projection = translate(next_projection, data.tx + offsetx, data.ty + offsety);
				bresenheim_line(projection, next_projection, data.img_data, data.heightmap[y][x].color, data.heightmap[y][x + 1].color);
			}
			if (data.heightmap[y + 1])
			{
				t_vector2 next_projection = isometric_project(x, y + 1, data);
				next_projection = translate(next_projection, data.tx + offsetx, data.ty + offsety);
				bresenheim_line(projection, next_projection, data.img_data, data.heightmap[y][x].color, data.heightmap[y + 1][x].color);
			}
		}
	}
	mlx_put_image_to_window(data.mlx_ptr, data.mlx_win, data.img_data.img, 0, 0);
	print_tooltips(data);
}

int handle_key(int keycode, t_data *data)
{
	if(keycode == 65307)
		terminate();
	else if(keycode == 61 && data->zoom < 100.0f)
	{
		data->zoom += 1.0f;
		draw(*data);
	}
	else if(keycode == 45 && data->zoom > 1.0f)
	{
		data->zoom -= 1.0f;
		draw(*data);
	}
	else if(keycode == 119)
	{
		data->ty -= 20;
		draw(*data);
	}
	else if(keycode == 97)
	{
		data->tx -= 20;
		draw(*data);
	}
	else if(keycode == 115)
	{
		data->ty += 20;
		draw(*data);
	}
	else if(keycode == 100)
	{
		data->tx += 20;
		draw(*data);
	}
	else if(keycode == 110)
	{
		data->angle_z -= 5.0f;
		draw(*data);
	}
	else if(keycode == 109)
	{
		data->angle_z += 5.0f;
		draw(*data);
	}
	else if(keycode == 104)
	{
		data->angle_y -= 5.0f;
		draw(*data);
	}
	else if(keycode == 106)
	{
		data->angle_y += 5.0f;
		draw(*data);
	}
	else if(keycode == 121)
	{
		data->angle_x -= 5.0f;
		draw(*data);
	}
	else if(keycode == 117)
	{
		data->angle_x += 5.0f;
		draw(*data);
	}
	return (0);
}

double calculate_initial_zoom(int min_x, int max_x, int min_y, int max_y)
{
    double zoom_x = (double)WIDTH / (max_x - min_x);
    double zoom_y = (double)HEIGHT / (max_y - min_y);
    return fmin(zoom_x, zoom_y);
}

int main(int argc, char **argv)
{
	void *mlx_ptr;
	void *mlx_win;
	t_img_data img_data;
	
	if(argc != 2)
	{
		ft_putstr_fd("Usage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" <filename>\n", 2);
		return (1);
	}
	mlx_ptr = mlx_init();
	mlx_win = mlx_new_window(mlx_ptr, WIDTH, HEIGHT, "fdf");
	create_img(mlx_ptr, &img_data);
	t_data data = {.img_data = img_data, .mlx_ptr = mlx_ptr, .mlx_win = mlx_win, .zoom = 1.0f, .angle_x = 0.0f, .angle_y = 0.0f, .angle_z = 0.0f, .tx = 0, .ty = 0};
	parse_map(argv[argc - 1], &data);

	//find initial zoom
	int min_x, max_x, min_y, max_y;
    calculate_bounding_box(data, &min_x, &max_x, &min_y, &max_y);
    data.zoom = calculate_initial_zoom(min_x, max_x, min_y, max_y);

	draw(data);
	mlx_hook(mlx_win, DestroyNotify, 0, terminate, NULL);
	mlx_key_hook(mlx_win, handle_key, &data);
	mlx_loop(mlx_ptr);
}
