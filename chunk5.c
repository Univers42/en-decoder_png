/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_unk_chunks_init(t_png_info *info)
{
	unsigned int	i;

	i = 0;
	while (i != 3)
	{
		info->unknown_chunks_data[i] = 0;
		info->unknown_chunks_size[i] = 0;
		++i;
	}
}

void	lodepng_unk_chunks_cleanup(t_png_info *info)
{
	unsigned int	i;

	i = 0;
	while (i != 3)
	{
		lodepng_free(info->unknown_chunks_data[i]);
		++i;
	}
}

static unsigned int	unk_copy_one(t_png_info *dest,
				const t_png_info *src, unsigned int i)
{
	size_t	j;

	dest->unknown_chunks_size[i] = src->unknown_chunks_size[i];
	dest->unknown_chunks_data[i] = (unsigned char *)lodepng_malloc(
			src->unknown_chunks_size[i]);
	if (!dest->unknown_chunks_data[i] && dest->unknown_chunks_size[i])
		return (83);
	j = 0;
	while (j < src->unknown_chunks_size[i])
	{
		dest->unknown_chunks_data[i][j] = src->unknown_chunks_data[i][j];
		++j;
	}
	return (0);
}

unsigned int	lodepng_unk_chunks_copy(t_png_info *dest,
			const t_png_info *src)
{
	unsigned int	i;
	unsigned int	err;

	lodepng_unk_chunks_cleanup(dest);
	i = 0;
	while (i != 3)
	{
		err = unk_copy_one(dest, src, i);
		if (err)
			return (err);
		++i;
	}
	return (0);
}

unsigned int	add_chunk_plte(ucvector *out, const t_png_color_mode *info)
{
	unsigned int	error;
	size_t		i;
	ucvector	plte;

	ucvector_init(&plte);
	i = 0;
	while (i != info->palettesize * 4)
	{
		if (i % 4 != 3)
			ucvector_push_back(&plte, info->palette[i]);
		++i;
	}
	error = add_chunk(out, "PLTE", plte.data, plte.size);
	ucvector_cleanup(&plte);
	return (error);
}
