/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 01:42:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

static unsigned int	push_keyword(t_ucvector *data, const char *key)
{
	size_t	i;

	i = 0;
	while (key[i] != 0)
		ucvector_push_back(data, (unsigned char)key[i++]);
	if (i < 1 || i > 79)
		return (89);
	ucvector_push_back(data, 0);
	ucvector_push_back(data, 0);
	return (0);
}

unsigned int	add_chunk_ztxt(t_ucvector *out, const char *keyword,
			const char *textstring,
			t_compress_settings *zlibsettings)
{
	unsigned int	error;
	t_ucvector		data;
	t_ucvector		compressed;
	size_t			i;

	ucvector_init(&data);
	ucvector_init(&compressed);
	error = push_keyword(&data, keyword);
	if (error)
		return (error);
	error = zlib_compress(&compressed,
			(unsigned char *)textstring, strlen(textstring),
			zlibsettings);
	if (!error)
	{
		i = 0;
		while (i != compressed.size)
			ucvector_push_back(&data, compressed.data[i++]);
		error = add_chunk(out, "zTXt", data.data, data.size);
	}
	ucvector_cleanup(&compressed);
	ucvector_cleanup(&data);
	return (error);
}

unsigned int	add_chunk_bkgd(t_ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	t_ucvector		bkgd;

	ucvector_init(&bkgd);
	if (info->color.colortype == LCT_GREY
		|| info->color.colortype == LCT_GREY_ALPHA)
	{
		ucvector_push_back(&bkgd, (unsigned char)(info->background_r >> 8));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_r & 255));
	}
	else if (info->color.colortype == LCT_RGB
		|| info->color.colortype == LCT_RGBA)
	{
		ucvector_push_back(&bkgd, (unsigned char)(info->background_r >> 8));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_r & 255));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_g >> 8));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_g & 255));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_b >> 8));
		ucvector_push_back(&bkgd, (unsigned char)(info->background_b & 255));
	}
	else if (info->color.colortype == LCT_PALETTE)
		ucvector_push_back(&bkgd, (unsigned char)(info->background_r & 255));
	error = add_chunk(out, "bKGD", bkgd.data, bkgd.size);
	ucvector_cleanup(&bkgd);
	return (error);
}

unsigned int	add_chunk_chrm(t_ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	t_ucvector		data;

	ucvector_init(&data);
	lodepng_add_32bit_int(&data, info->chrm_white_x);
	lodepng_add_32bit_int(&data, info->chrm_white_y);
	lodepng_add_32bit_int(&data, info->chrm_red_x);
	lodepng_add_32bit_int(&data, info->chrm_red_y);
	lodepng_add_32bit_int(&data, info->chrm_green_x);
	lodepng_add_32bit_int(&data, info->chrm_green_y);
	lodepng_add_32bit_int(&data, info->chrm_blue_x);
	lodepng_add_32bit_int(&data, info->chrm_blue_y);
	error = add_chunk(out, "cHRM", data.data, data.size);
	ucvector_cleanup(&data);
	return (error);
}

unsigned int	add_chunk_iccp(t_ucvector *out, const t_png_info *info,
			t_compress_settings *zlibsettings)
{
	unsigned int	error;
	t_ucvector		data;
	t_ucvector		compressed;
	size_t			i;

	ucvector_init(&data);
	ucvector_init(&compressed);
	error = push_keyword(&data, info->iccp_name);
	if (error)
		return (error);
	error = zlib_compress(&compressed,
			info->iccp_profile, info->iccp_profile_size,
			zlibsettings);
	if (!error)
	{
		i = 0;
		while (i != compressed.size)
			ucvector_push_back(&data, compressed.data[i++]);
		error = add_chunk(out, "iCCP", data.data, data.size);
	}
	ucvector_cleanup(&compressed);
	ucvector_cleanup(&data);
	return (error);
}

#endif
