/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

unsigned int	add_chunk_text(t_ucvector *out, const char *keyword,
			const char *textstring)
{
	unsigned int	error;
	size_t			i;
	t_ucvector		text;

	ucvector_init(&text);
	i = 0;
	while (keyword[i] != 0)
		ucvector_push_back(&text, (unsigned char)keyword[i++]);
	if (i < 1 || i > 79)
		return (89);
	ucvector_push_back(&text, 0);
	i = 0;
	while (textstring[i] != 0)
		ucvector_push_back(&text, (unsigned char)textstring[i++]);
	error = add_chunk(out, "tEXt", text.data, text.size);
	ucvector_cleanup(&text);
	return (error);
}

unsigned int	add_chunk_srgb(t_ucvector *out, const t_png_info *info)
{
	unsigned char	data;

	data = info->srgb_intent;
	return (add_chunk(out, "sRGB", &data, 1));
}

unsigned int	add_chunk_gama(t_ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	t_ucvector		data;

	ucvector_init(&data);
	lodepng_add_32bit_int(&data, info->gama_gamma);
	error = add_chunk(out, "gAMA", data.data, data.size);
	ucvector_cleanup(&data);
	return (error);
}

unsigned int	add_chunk_phys(t_ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	t_ucvector		data;

	ucvector_init(&data);
	lodepng_add_32bit_int(&data, info->phys_x);
	lodepng_add_32bit_int(&data, info->phys_y);
	ucvector_push_back(&data, info->phys_unit);
	error = add_chunk(out, "pHYs", data.data, data.size);
	ucvector_cleanup(&data);
	return (error);
}

unsigned int	add_chunk_time(t_ucvector *out, const t_png_time *time)
{
	unsigned int	error;
	unsigned char	*data;

	data = (unsigned char *)lodepng_malloc(7);
	if (!data)
		return (83);
	data[0] = (unsigned char)(time->year >> 8);
	data[1] = (unsigned char)(time->year & 255);
	data[2] = (unsigned char)time->month;
	data[3] = (unsigned char)time->day;
	data[4] = (unsigned char)time->hour;
	data[5] = (unsigned char)time->minute;
	data[6] = (unsigned char)time->second;
	error = add_chunk(out, "tIME", data, 7);
	lodepng_free(data);
	return (error);
}

#endif
