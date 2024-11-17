/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:26 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 18:56:52 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	count_lines(char *map_file)
{
	int		fd;
	int		count;
	char	*line;

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

static int	count_cols(char **split_line)
{
	int	count;

	count = 0;
	while (*split_line)
	{
		count++;
		split_line++;
	}
	return (count);
}

static void	fill_line(char **split, t_point **map, t_data *data, int y)
{
	int		x;
	char	*color;

	x = 0;
	while (x < data->x_size)
	{
		map[y][x].z = ft_atoi(split[x]);
		color = ft_strchr(split[x], ',');
		if (color)
			map[y][x].color = ft_atoi_hex(color + 1);
		else
			map[y][x].color = 0xFFFFFF;
		x++;
	}
}

static void	parse_line(char *line, t_point **map, t_data *data, int y)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split)
	{
		perror("parse_line : split");
		exit(EXIT_FAILURE);
	}
	if (y == 0)
		data->x_size = count_cols(split);
	if (count_cols(split) < data->x_size)
	{
		ft_putstr_fd("Found wrong line length. Exiting.\n", 2);
		exit(EXIT_FAILURE);
	}
	map[y] = malloc(sizeof(t_point) * data->x_size);
	if (!map[y])
	{
		perror("parse_line : row allocation");
		exit(EXIT_FAILURE);
	}
	fill_line(split, map, data, y);
	free_split(split);
}

void	parse_map(char *map_file, t_data *data)
{
	t_point	**map;
	char	*line;
	int		y;
	int		fd;

	map = malloc(sizeof(t_point *) * (count_lines(map_file) + 1));
	if (!map)
	{
		perror("parse_map : map allocation");
		exit(EXIT_FAILURE);
	}
	fd = try_open(map_file, O_RDONLY);
	y = 0;
	line = get_next_line(fd);
	while (line)
	{
		parse_line(line, map, data, y);
		y++;
		free(line);
		line = get_next_line(fd);
	}
	map[y] = NULL;
	try_close(fd);
	data->heightmap = map;
}
