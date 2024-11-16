#include "fdf.h"

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

t_vector2 translate(t_vector2 point, int tx, int ty)
{
    t_vector2 translated;

    translated.x = point.x + tx;
    translated.y = point.y + ty;
    return (translated);
}






































int getset_min_x(int value, char reset)
{
	static int min_x;
	int temp;

	if(reset)
	{
		temp = min_x;
		min_x = INT_MAX;
		return (temp);
	}
	if(value < min_x)
		min_x = value;
	return (min_x);
}

int getset_max_x(int value, char reset)
{
	static int max_x;
	int temp;

	if(reset)
	{
		temp = max_x;
		max_x = INT_MIN;
		return (temp);
	}
	if(value > max_x)
		max_x = value;
	return (max_x);
}

int getset_min_y(int value, char reset)
{
	static int min_y;
	int temp;

	if(reset)
	{
		temp = min_y;
		min_y = INT_MAX;
		return (temp);
	}
	if(value < min_y)
		min_y = value;
	return (min_y);
}

int getset_max_y(int value, char reset)
{
	static int max_y;
	int temp;

	if(reset)
	{
		temp = max_y;
		max_y = INT_MIN;
		return (temp);
	}
	if(value > max_y)
		max_y = value;
	return (max_y);
}

t_vector2 calculate_center_offset(t_vector2 **projections, int height, int width)
{
	int x;
	int y;
	t_vector2 offset;

	y = 0;
    while (y < height)
    {
		x = 0;
        while (x < width)
        {
			getset_max_x(projections[y][x].x, 0);
			getset_min_x(projections[y][x].x, 0);
			getset_max_y(projections[y][x].y, 0);
			getset_min_y(projections[y][x].y, 0);
			x++;
        }
		y++;
    }
	offset.x = WIDTH / 2 - ((getset_min_x(0, 1) + getset_max_x(0, 1)) / 2);
	offset.y = HEIGHT / 2 - ((getset_min_y(0, 1) + getset_max_y(0, 1)) / 2);
	return (offset);
}

int calculate_initial_zoom(t_data data)
{
	int x;
	int y;
	int zoom_x;
	int zoom_y;
	t_vector2 projection;

	y = 0;
    while (data.heightmap[y])
    {
		x = 0;
        while (x < data.x_size)
        {
            projection = data.project(x, y, data);
            getset_min_x(projection.x, 0);
            getset_max_x(projection.x, 0);
			getset_min_y(projection.y, 0);
			getset_max_y(projection.y, 0);
			x++;
        }
		y++;
    }

	zoom_x = WIDTH / (getset_max_x(0, 1) - getset_min_x(0, 1));
	zoom_y = HEIGHT / (getset_max_y(0, 1) - getset_min_y(0, 1));
	if(zoom_x < zoom_y)
	{
		if(zoom_x < 1)
			return (1);
		else
			return (zoom_x);
	}
	else
	{
		if(zoom_y < 1)
			return (1);
		else
			return (zoom_y);
	}
}

t_vector2 **get_projections(t_data data)
{

	int i = 0;
	for (; data.heightmap[i]; i++);
	t_vector2 **projections = malloc(sizeof(t_vector2*) * i);
	for (int y = 0; data.heightmap[y]; y++)
	{
		projections[y] = malloc(sizeof(t_vector2) * data.x_size);
		for (int x = 0; x < data.x_size; x++)
		{
			projections[y][x] = data.project(x, y, data);
		}
	}
	t_vector2 center_offset = calculate_center_offset(projections, i, data.x_size);
	for (int y = 0; data.heightmap[y]; y++)
	{
		for (int x = 0; x < data.x_size; x++)
		{
			projections[y][x] = translate(projections[y][x], center_offset.x + data.tx, center_offset.y + data.ty);
		}
	}
	return (projections);
}
void draw(t_data data)
{
	t_vector2 **projections = get_projections(data);
	clear_img(data.img_data.data);
	for (int y = 0; data.heightmap[y]; y++)
	{
		for (int x = 0; x < data.x_size; x++)
		{
			if (x < data.x_size - 1)
				bresenheim_line(projections[y][x], projections[y][x + 1], data.img_data, data.heightmap[y][x].color, data.heightmap[y][x + 1].color);
			if (data.heightmap[y + 1])
				bresenheim_line(projections[y][x], projections[y + 1][x], data.img_data, data.heightmap[y][x].color, data.heightmap[y + 1][x].color);
		}
	}
	mlx_put_image_to_window(data.mlx_ptr, data.mlx_win, data.img_data.img, 0, 0);
	print_tooltips(data);
	for (int y = 0; data.heightmap[y]; y++)
		free(projections[y]);
	free(projections);
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
	t_data data = {.img_data = img_data, .mlx_ptr = mlx_ptr, .mlx_win = mlx_win, .zoom = 1.0f, .angle_x = 0.0f, .angle_y = 0.0f, .angle_z = 0.0f, .tx = 0, .ty = 0, .project = isometric_project};
	parse_map(argv[argc - 1], &data);
    data.zoom = calculate_initial_zoom(data);

	draw(data);
	mlx_hook(mlx_win, DestroyNotify, 0, terminate, NULL);
	mlx_key_hook(mlx_win, handle_key, &data);
	mlx_loop(mlx_ptr);
}
