/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	dg_copy_idat(t_png_state *st, ucvector *idat,
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

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS
static int	dg_anc_text(t_png_state *st, const unsigned char *ch,
		const unsigned char *d, unsigned int len)
{
	if (!st->decoder.read_text_chunks)
		return (lodepng_chunk_type_equals(ch, "tEXt")
			|| lodepng_chunk_type_equals(ch, "zTXt")
			|| lodepng_chunk_type_equals(ch, "iTXt"));
	if (lodepng_chunk_type_equals(ch, "tEXt"))
		st->error = read_chunk_text(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "zTXt"))
		st->error = read_chunk_ztxt(&st->info_png,
				&st->decoder.zlibsettings, d, len);
	else if (lodepng_chunk_type_equals(ch, "iTXt"))
		st->error = read_chunk_itxt(&st->info_png,
				&st->decoder.zlibsettings, d, len);
	else
		return (0);
	return (1);
}

static int	dg_anc_other(t_png_state *st, const unsigned char *ch,
		const unsigned char *d, unsigned int len)
{
	if (lodepng_chunk_type_equals(ch, "bKGD"))
		st->error = read_chunk_bkgd(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "tIME"))
		st->error = read_chunk_time(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "pHYs"))
		st->error = read_chunk_phys(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "gAMA"))
		st->error = read_chunk_gama(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "cHRM"))
		st->error = read_chunk_chrm(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "sRGB"))
		st->error = read_chunk_srgb(&st->info_png, d, len);
	else if (lodepng_chunk_type_equals(ch, "iCCP"))
		st->error = read_chunk_iccp(&st->info_png,
				&st->decoder.zlibsettings, d, len);
	else
		return (0);
	return (1);
}

static void	dg_handle_extra(t_png_state *st, const unsigned char *ch,
		const unsigned char *d, unsigned int len, unsigned int *unk)
{
	if (dg_anc_text(st, ch, d, len))
		return ;
	if (dg_anc_other(st, ch, d, len))
		return ;
	if (!st->decoder.ignore_critical && !lodepng_chunk_ancillary(ch))
		st->error = 69;
	*unk = 1;
}
#else
static void	dg_handle_extra(t_png_state *st, const unsigned char *ch,
		const unsigned char *d, unsigned int len, unsigned int *unk)
{
	(void)d;
	(void)len;
	if (!st->decoder.ignore_critical && !lodepng_chunk_ancillary(ch))
		st->error = 69;
	*unk = 1;
}
#endif

void	dg_handle_chunk(t_png_state *st, ucvector *idat,
		const unsigned char *ch, unsigned int *iend,
		unsigned int *unk, unsigned int *cpos)
{
	const unsigned char	*d;
	unsigned int			len;

	len = lodepng_chunk_length(ch);
	d = lodepng_chunk_data_const(ch);
	*unk = 0;
	if (lodepng_chunk_type_equals(ch, "IDAT"))
	{
		dg_copy_idat(st, idat, d, len);
		*cpos = 3;
	}
	else if (lodepng_chunk_type_equals(ch, "IEND"))
		*iend = 1;
	else if (lodepng_chunk_type_equals(ch, "PLTE"))
	{
		st->error = read_chunk_plte(&st->info_png.color, d, len);
		*cpos = 2;
	}
	else if (lodepng_chunk_type_equals(ch, "tRNS"))
		st->error = read_chunk_trns(&st->info_png.color, d, len);
	else
		dg_handle_extra(st, ch, d, len, unk);
}
