#include "fdf.h"



int get_color(int red, int green, int blue)
{
	return((red << 16) | (green << 8) | blue);
}

double deg_to_rad(double degrees)
{
	return (degrees * (M_PI / 180));
}

t_vector2 isometric_project(t_vector3 target)
{
	t_vector2 projection;
	double rad;

	rad = deg_to_rad(30);
	projection.x = ((target.x - target.y) * cos(rad) * SCALE) + WIDTH / 2;
	projection.y = ((target.x + target.y) * sin(rad) * SCALE - target.z * SCALE) + HEIGHT / 2;
	return (projection);
}

int try_open(char *file, int flag)
{
	int fd;
	fd = open(file, flag);
	if (fd == -1)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void try_close(int fd)
{
	if(close(fd) == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
}

void free_split(char **split)
{
	int i;

	i = 0;
	while(split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void print_vector3(t_vector3 vector3)
{
	printf("x: %f, y: %f, z: %f\n", vector3.x, vector3.y, vector3.z);
}

void print_vector2(t_vector2 vector2)
{
	printf("x: %d, y: %d\n", vector2.x, vector2.y);
}

t_vector3 new_point(int x, int y, int z)
{
	t_vector3 new;

	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
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

t_vector3 *parse_map(char *map_file)
{
	t_vector3 *map;
	map = malloc(sizeof(t_vector3) * 100);
	if(!map)
	{
		perror("parse_map");
		exit(EXIT_FAILURE);
	}
	int fd = try_open(map_file, O_RDONLY);
	int i = 0;
	int y = 0;
	char *line = get_next_line(fd);
	while(line)
	{
		char **split = ft_split(line, ' ');
		int x = 0;
		while(split[x])
		{
			map[i].x = x;
			map[i].y = y;
			map[i].z = ft_atoi(split[x]);
			i++;
			x++;
		}
		free_split(split);
		free(line);
		line = get_next_line(fd);
		y++;
	}
	free(line);
	try_close(fd);
	return (map);
}

int main(int argc, char **argv)
{
	void *mlx = mlx_init();
	void *mlx_win = mlx_new_window(mlx, WIDTH, HEIGHT, "fdf");
	// int color = get_color(255, 0, 0);
	t_vector3 *map = parse_map(argv[argc - 1]);
	for (size_t i = 1; i < 100; i++)
	{
		bresenham_line(mlx, mlx_win, isometric_project(map[i]), isometric_project(map[i - 1]));
	}
	mlx_loop(mlx);
}
