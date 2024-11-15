/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubry <aloubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:20:54 by aloubry           #+#    #+#             */
/*   Updated: 2024/11/15 19:14:22 by aloubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi_hex(const char *nptr)
{
	int	mult;
	int	num;

	mult = 1;
	while (*nptr && ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			mult = -1;
		nptr++;
	}
	if (*nptr == '0' && (*(nptr + 1) == 'x' || *(nptr + 1) == 'X'))
		nptr += 2;
	num = 0;
	while (*nptr && ft_isxdigit(*nptr))
	{
		if (ft_isdigit(*nptr))
			num = num * 16 + (*nptr - '0');
		else
			num = num * 16 + (ft_toupper(*nptr) - 55);
		nptr++;
	}
	return (num * mult);
}
