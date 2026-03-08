/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_chunk_type(char type[5], const unsigned char *chunk)
{
	unsigned int	i;

	i = 0;
	while (i != 4)
	{
		type[i] = (char)chunk[4 + i];
		++i;
	}
	type[4] = 0;
}

unsigned char	lodepng_chunk_type_equals(const unsigned char *chunk,
				const char *type)
{
	if (strlen(type) != 4)
		return (0);
	return (chunk[4] == type[0] && chunk[5] == type[1]
		&& chunk[6] == type[2] && chunk[7] == type[3]);
}

unsigned char	lodepng_chunk_ancillary(const unsigned char *chunk)
{
	return ((chunk[4] & 32) != 0);
}

unsigned char	lodepng_chunk_private(const unsigned char *chunk)
{
	return ((chunk[6] & 32) != 0);
}

unsigned char	lodepng_chunk_safetocopy(const unsigned char *chunk)
{
	return ((chunk[7] & 32) != 0);
}
