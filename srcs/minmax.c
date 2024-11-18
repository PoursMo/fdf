/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minmax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 13:10:21 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/18 18:13:46 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	getset_min_x(int value, char reset)
{
	static int	min_x;
	int			temp;

	if (reset)
	{
		temp = min_x;
		min_x = INT_MAX;
		return (temp);
	}
	if (value < min_x)
		min_x = value;
	return (min_x);
}

int	getset_max_x(int value, char reset)
{
	static int	max_x;
	int			temp;

	if (reset)
	{
		temp = max_x;
		max_x = INT_MIN;
		return (temp);
	}
	if (value > max_x)
		max_x = value;
	return (max_x);
}

int	getset_min_y(int value, char reset)
{
	static int	min_y;
	int			temp;

	if (reset)
	{
		temp = min_y;
		min_y = INT_MAX;
		return (temp);
	}
	if (value < min_y)
		min_y = value;
	return (min_y);
}

int	getset_max_y(int value, char reset)
{
	static int	max_y;
	int			temp;

	if (reset)
	{
		temp = max_y;
		max_y = INT_MIN;
		return (temp);
	}
	if (value > max_y)
		max_y = value;
	return (max_y);
}
