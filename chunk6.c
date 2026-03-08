/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	trns_palette(ucvector *trns, const t_png_color_mode *info)
{
	size_t	amount;
	size_t	i;

	amount = info->palettesize;
	i = info->palettesize;
	while (i != 0)
	{
		if (info->palette[4 * (i - 1) + 3] == 255)
			--amount;
		else
			break ;
		--i;
	}
	i = 0;
	while (i != amount)
	{
		ucvector_push_back(trns, info->palette[4 * i + 3]);
		++i;
	}
}

static void	trns_keys(ucvector *trns, const t_png_color_mode *info)
{
	if (info->colortype == LCT_GREY && info->key_defined)
	{
		ucvector_push_back(trns, (unsigned char)(info->key_r >> 8));
		ucvector_push_back(trns, (unsigned char)(info->key_r & 255));
	}
	else if (info->colortype == LCT_RGB && info->key_defined)
	{
		ucvector_push_back(trns, (unsigned char)(info->key_r >> 8));
		ucvector_push_back(trns, (unsigned char)(info->key_r & 255));
		ucvector_push_back(trns, (unsigned char)(info->key_g >> 8));
		ucvector_push_back(trns, (unsigned char)(info->key_g & 255));
		ucvector_push_back(trns, (unsigned char)(info->key_b >> 8));
		ucvector_push_back(trns, (unsigned char)(info->key_b & 255));
	}
}

unsigned int	add_chunk_trns(ucvector *out, const t_png_color_mode *info)
{
	unsigned int	error;
	ucvector		trns;

	ucvector_init(&trns);
	if (info->colortype == LCT_PALETTE)
		trns_palette(&trns, info);
	else
		trns_keys(&trns, info);
	error = add_chunk(out, "tRNS", trns.data, trns.size);
	ucvector_cleanup(&trns);
	return (error);
}

unsigned int	add_chunk_idat(ucvector *out, const unsigned char *data,
			size_t datasize, t_compress_settings *zlibsettings)
{
	ucvector		zlibdata;
	unsigned int	error;

	ucvector_init(&zlibdata);
	error = zlib_compress(&zlibdata.data, &zlibdata.size,
			data, datasize, zlibsettings);
	if (!error)
		error = add_chunk(out, "IDAT", zlibdata.data, zlibdata.size);
	ucvector_cleanup(&zlibdata);
	return (error);
}
