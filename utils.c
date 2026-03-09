/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 23:03:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	*lodepng_malloc(size_t size)
{
	return (malloc(size));
}

void	lodepng_free(void *ptr)
{
	free(ptr);
}

void	*lodepng_realloc(void *ptr, size_t size)
{
	return (realloc(ptr, size));
}

unsigned int	lodepng_max_uint(unsigned int a, unsigned int b)
{
	if (a > b)
		return (a);
	return (b);
}

unsigned int	lodepng_min_uint(unsigned int a, unsigned int b)
{
	if (a < b)
		return (a);
	return (b);
}
