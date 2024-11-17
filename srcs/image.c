/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:49:40 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/17 16:56:51 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	create_img(void *mlx, t_img_data *data)
{
	data->img = mlx_new_image(mlx, WIDTH, HEIGHT);
	data->data = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->size_line, &data->endian);
	clear_img(data->data);
}

void	clear_img(char *data)
{
	ft_memset(data, 0, WIDTH * HEIGHT * 4);
}
