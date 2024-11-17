#include "fdf.h"

int main(int argc, char **argv)
{
	t_data data;
	
	if(argc != 2)
	{
		ft_putstr_fd("Usage: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(" <filename>\n", 2);
		return (1);
	}
	setup(&data, argv[argc - 1]);
	draw(data);
	mlx_hook(data.mlx_win, DestroyNotify, 0, terminate, NULL);
	mlx_key_hook(data.mlx_win, handle_key, &data);
	mlx_loop(data.mlx_ptr);
	free_map(data.heightmap);
}
