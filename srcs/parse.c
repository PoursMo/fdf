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

int **parse_map(char *map_file, int *x_size)
{
	int **map;
    char *line;
    char **split;
    int y;
    int x;

	map = malloc(sizeof(int*) * count_lines(map_file) + 1);
	if(!map)
	{
		perror("parse_map");
		exit(EXIT_FAILURE);
	}
	int fd = try_open(map_file, O_RDONLY);
	line = get_next_line(fd);
    y = 0;
	while(line)
	{
		split = ft_split(line, ' ');
        if(!split)
        {
            perror("parse_map");
            exit(EXIT_FAILURE);
        }
        if(y == 0)
            *x_size = count_cols(split);
        map[y] = malloc(sizeof(int) * *x_size);
        if(!map[y])
        {
            perror("parse_map");
            exit(EXIT_FAILURE);
        }
        x = 0;
        while (x < *x_size)
        {
            if (split[x][0] == '0' && split[x][1] == 'x')
                map[y][x] = ft_atoi_hex(split[x] + 2);
            else
                map[y][x] = ft_atoi(split[x]);
            x++;
        }
        y++;
        free_split(split);
		free(line);
		line = get_next_line(fd);
	}
    map[y] = NULL;
	try_close(fd);
	return (map);
}