#include "fdf.h"

static int count_lines(char *map_file)
{
    int fd;
    int count;
    char *line;
    
    fd = try_open(map_file, O_RDONLY);
    count = 0;
    line = get_next_line(fd);
    while (line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    try_close(fd);
    return (count);
}

static int count_cols(char **split_line)
{
    int count;

    count = 0;
    while(*split_line)
    {
        count++;
        split_line++;
    }
    return (count);
}

void parse_map(char *map_file, t_data *data)
{
	t_point **map;
    char *line;
    char **split;
    int y;
    int x;

	map = malloc(sizeof(t_point*) * (count_lines(map_file) + 1));
	if(!map)
	{
		perror("parse_map");
		exit(EXIT_FAILURE);
	}
	int fd = try_open(map_file, O_RDONLY);
	line = get_next_line(fd);
    y = 0;
    while (line)
    {
		split = ft_split(line, ' ');
        if(!split)
        {
            perror("parse_map");
            exit(EXIT_FAILURE);
        }
        if(y == 0)
            data->x_size = count_cols(split);
        map[y] = malloc(sizeof(t_point) * data->x_size);
        if(!map[y])
        {
            perror("parse_map");
            exit(EXIT_FAILURE);
        }
        x = 0;
        while (x < data->x_size)
        {
            map[y][x].z = ft_atoi(split[x]);
            char *color = ft_strchr(split[x], ',');
            if(color)
                map[y][x].color = ft_atoi_hex(color + 1);
            else
                map[y][x].color = 0xFFFFFF;
            x++;
        }
        y++;
        free_split(split);
		free(line);
		line = get_next_line(fd);
	}
    map[y] = NULL;
	try_close(fd);
	data->heightmap = map;
}
