/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 01:08:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	dg_copy_idat(t_png_state *st, t_ucvector *idat,
		const unsigned char *d, unsigned int len)
{
	size_t	oldsize;
	size_t	newsize;
	size_t	i;

	oldsize = idat->size;
	if (lodepng_addofl(oldsize, len, &newsize))
	{
		st->error = 95;
		return ;
	}
	if (!ucvector_resize(idat, newsize))
	{
		st->error = 83;
		return ;
	}
	i = 0;
	while (i != len)
	{
		idat->data[oldsize + i] = d[i];
		++i;
	}
}

static void	dg_handle_extra(t_png_state *st,
		const unsigned char *ch, unsigned int *unk)
{
	if (!st->decoder.ignore_critical && !lodepng_chunk_ancillary(ch))
		st->error = 69;
	*unk = 1;
}

void	dg_handle_chunk(t_png_state *st, t_ucvector *idat,
		const unsigned char *ch, unsigned int *out)
{
	const unsigned char	*d;
	unsigned int		len;

	len = lodepng_chunk_length(ch);
	d = lodepng_chunk_data_const(ch);
	out[1] = 0;
	if (lodepng_chunk_type_equals(ch, "IDAT"))
	{
		dg_copy_idat(st, idat, d, len);
		out[2] = 3;
	}
	else if (lodepng_chunk_type_equals(ch, "IEND"))
		out[0] = 1;
	else if (lodepng_chunk_type_equals(ch, "PLTE"))
	{
		st->error = read_chunk_plte(&st->info_png.color, d, len);
		out[2] = 2;
	}
	else if (lodepng_chunk_type_equals(ch, "tRNS"))
		st->error = read_chunk_trns(&st->info_png.color, d, len);
	else
		dg_handle_extra(st, ch, &out[1]);
}
