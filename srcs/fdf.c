#include "fdf.h"

int get_color(int red, int green, int blue)
{
	return((red << 16) | (green << 8) | blue);
}

double deg_to_rad(double degrees)
{
	return (degrees * (M_PI / 180));
}

t_vector2 isometric_project(int x, int y, int z)
{
	t_vector2 projection;
	double rad;

	rad = deg_to_rad(30);
	projection.x = ((x - y) * cos(rad) * SCALE) + WIDTH / 2;
	projection.y = ((x + y) * sin(rad) * SCALE - z * SCALE) + HEIGHT / 2;
	return (projection);
}

void get_projection(t_point **map)
{
	int x;
	int y;

	y = 0;
	while(map[y])
	{
		x = 0;
		while(x < 4)
		{
			map[y][x].projection = isometric_project(x, y, map[y][x].z);
			printf("x: %d, y: %d\n", map[y][x].projection.x, map[y][x].projection.y);
			x++;
		}
		y++;
	}
}

void bresenham_line(void* mlx, void* mlx_win, t_vector2 v1, t_vector2 v2)
{
	int dx = abs(v2.x - v1.x);
	int dy = abs(v2.y - v1.y);
	int sx = (v1.x < v2.x) ? 1 : -1;
	int sy = (v1.y < v2.y) ? 1 : -1;
	int err = dx - dy;

	while (1)
	{
		mlx_pixel_put(mlx, mlx_win, v1.x, v1.y, 0x00FFFFFF);
		if (v1.x == v2.x && v1.y == v2.y)
			break;
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			v1.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			v1.y += sy;
		}
	}
}

void print_point(t_point **map, int y, int x)
{
	printf("x: %d, y: %d, z: %d\n", x, y , map[y][x].z);
}

int main(int argc, char **argv)
{
	void *mlx = mlx_init();
	void *mlx_win = mlx_new_window(mlx, WIDTH, HEIGHT, "fdf");
	t_point **map = parse_map(argv[argc - 1]);	
	get_projection(map);
		int x;
	int y;
	y = 0;
	while(map[y])
	{
		x = 0;
		while(x < 4)
		{
			if(map[y + 1])
				bresenham_line(mlx, mlx_win, map[y][x].projection, map[y + 1][x].projection);
			if(x < 3)
				bresenham_line(mlx, mlx_win, map[y][x].projection, map[y][x + 1].projection);
			x++;
		}
		y++;
	}
	mlx_loop(mlx);
}
