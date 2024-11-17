/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:26 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 17:59:58 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	count_lines(char *map_file)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(map_file, O_RDONLY);
	if (fd == -1)
	{
		perror("count_lines : file open");
		exit(EXIT_FAILURE);
	}
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



void	parse_map(char *map_file, t_data *data)
{
	t_point	**map;
	char	*line;
	char	**split;
	int		y;
	int		x;
	int		fd;

	map = malloc(sizeof(t_point *) * (count_lines(map_file) + 1));
	if (!map)
	{
		perror("parse_map : map allocation");
		exit(EXIT_FAILURE);
	}
	fd = open(map_file, O_RDONLY);
	if (fd == -1)
	{
		perror("parse_map : file open");
		exit(EXIT_FAILURE);
	}
	y = 0;
	line = get_next_line(fd);
	while (line)
	{
		split = ft_split(line, ' ');
		if (!split)
		{
			perror("parse_map : split");
			exit(EXIT_FAILURE);
		}
		if (y == 0)
			data->x_size = count_cols(split);
		if(count_cols(split) < data->x_size)
		{
			ft_putstr_fd("Found wrong line length. Exiting.\n", 2);
			exit(EXIT_FAILURE);
		}
		map[y] = malloc(sizeof(t_point) * data->x_size);
		if (!map[y])
		{
			perror("parse_map : row allocation");
			exit(EXIT_FAILURE);
		}
		x = 0;
		while (x < data->x_size)
		{
			map[y][x].z = ft_atoi(split[x]);
			char *color = ft_strchr(split[x], ',');
			if (color)
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
