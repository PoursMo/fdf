#include "fdf.h"

int get_color(int red, int green, int blue)
{
	return ((red << 16) | (green << 8) | blue);
}

float deg_to_rad(double degrees)
{
	return (degrees * (M_PI / 180.0f));
}

t_vector2 isometric_project(int x, int y, int z, float zoom)
{
	t_vector2 projection;

	projection.x = ((x - y) * cos(deg_to_rad(30)) * zoom) + WIDTH / 2;
	projection.y = ((x + y) * sin(deg_to_rad(30)) * zoom) - (z * zoom) + HEIGHT / 2;
	return (projection);
}

void place_pixel_in_img(char *data, int x, int y, int color)
{
	int i;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
	i = (x * 4) + (y * WIDTH * 4);
	data[i] = color;
	data[i + 1] = (color >> 8);
	data[i + 2] = (color >> 16);
	data[i + 3] = 0;
}

void clear_img(char *data)
{
	ft_memset(data, 0, WIDTH * HEIGHT * 4);
}

void bresenheim_line(t_vector2 start, t_vector2 end, char *data, int color)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while (1)
	{
		place_pixel_in_img(data, start.x, start.y, color);
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
	}
}

void create_img(void *mlx, t_img_data *data)
{
	data->img = mlx_new_image(mlx, WIDTH, HEIGHT);
	data->data = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->size_line, &data->endian);
	clear_img(data->data);
}

int main(int argc, char **argv)
{
	void *mlx_ptr;
	void *mlx_win;
	t_img_data img_data;
	int x_size;
	
	mlx_ptr = mlx_init();
	mlx_win = mlx_new_window(mlx_ptr, WIDTH, HEIGHT, "fdf");
	create_img(mlx_ptr, &img_data);
	int **heightmap = parse_map(argv[argc - 1], &x_size);

	for (int y = 0; heightmap[y]; y++)
	{
		for (int x = 0; x < x_size; x++)
		{
			t_vector2 projection = isometric_project(x, y, heightmap[y][x], 5);
			place_pixel_in_img(img_data.data, projection.x, projection.y, get_color(255, 255, 255));
			if (x < x_size - 1)
			{
				t_vector2 next_projection = isometric_project(x + 1, y, heightmap[y][x + 1], 5);
				bresenheim_line(projection, next_projection, img_data.data, get_color(255, 255, 255));
			}
			if (heightmap[y + 1])
			{
				t_vector2 next_projection = isometric_project(x, y + 1, heightmap[y + 1][x], 5);
				bresenheim_line(projection, next_projection, img_data.data, get_color(255, 255, 255));
			}
		}
	}
	mlx_put_image_to_window(mlx_ptr, mlx_win, img_data.img, 0, 0);
	mlx_loop(mlx_ptr);
}
