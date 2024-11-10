#include "fdf.h"

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