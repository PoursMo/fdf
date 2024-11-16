#include "fdf.h"

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
            t_vector2 projection = data.project(x, y, data);
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

int calculate_initial_zoom(int min_x, int max_x, int min_y, int max_y)
{
    int zoom_x = WIDTH / (max_x - min_x);
    int zoom_y = HEIGHT / (max_y - min_y);
    return fmin(zoom_x, zoom_y);
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

t_vector2 translate(t_vector2 point, int tx, int ty)
{
    t_vector2 translated;
    translated.x = point.x + tx;
    translated.y = point.y + ty;
    return (translated);
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
			t_vector2 projection = data.project(x, y, data);
			projection = translate(projection, data.tx + offsetx, data.ty + offsety);
			if (x < data.x_size - 1)
			{
				t_vector2 next_projection = data.project(x + 1, y, data);
				next_projection = translate(next_projection, data.tx + offsetx, data.ty + offsety);
				bresenheim_line(projection, next_projection, data.img_data, data.heightmap[y][x].color, data.heightmap[y][x + 1].color);
			}
			if (data.heightmap[y + 1])
			{
				t_vector2 next_projection = data.project(x, y + 1, data);
				next_projection = translate(next_projection, data.tx + offsetx, data.ty + offsety);
				bresenheim_line(projection, next_projection, data.img_data, data.heightmap[y][x].color, data.heightmap[y + 1][x].color);
			}
		}
	}
	mlx_put_image_to_window(data.mlx_ptr, data.mlx_win, data.img_data.img, 0, 0);
	print_tooltips(data);
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
	t_data data = {.img_data = img_data, .mlx_ptr = mlx_ptr, .mlx_win = mlx_win, .zoom = 1.0f, .angle_x = 0.0f, .angle_y = 0.0f, .angle_z = 0.0f, .tx = 0, .ty = 0, .project = perspective_project};
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
