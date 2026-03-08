/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:41:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_ANCILLARY_CHUNKS

unsigned int	add_chunk_ztxt(ucvector *out, const char *keyword,
			const char *textstring,
			t_compress_settings *zlibsettings)
{
	unsigned int	error;
	ucvector		data;
	ucvector		compressed;
	size_t			i;
	size_t			textsize;

	textsize = strlen(textstring);
	ucvector_init(&data);
	ucvector_init(&compressed);
	i = 0;
	while (keyword[i] != 0)
		ucvector_push_back(&data, (unsigned char)keyword[i++]);
	if (i < 1 || i > 79)
		return (89);
	ucvector_push_back(&data, 0);
	ucvector_push_back(&data, 0);
	error = zlib_compress(&compressed.data, &compressed.size,
			(unsigned char *)textstring, textsize, zlibsettings);
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

static void	bkgd_fill(ucvector *bkgd, const t_png_info *info)
{
	if (info->color.colortype == LCT_GREY
		|| info->color.colortype == LCT_GREY_ALPHA)
	{
		ucvector_push_back(bkgd, (unsigned char)(info->background_r >> 8));
		ucvector_push_back(bkgd, (unsigned char)(info->background_r & 255));
	}
	else if (info->color.colortype == LCT_RGB
		|| info->color.colortype == LCT_RGBA)
	{
		ucvector_push_back(bkgd, (unsigned char)(info->background_r >> 8));
		ucvector_push_back(bkgd, (unsigned char)(info->background_r & 255));
		ucvector_push_back(bkgd, (unsigned char)(info->background_g >> 8));
		ucvector_push_back(bkgd, (unsigned char)(info->background_g & 255));
		ucvector_push_back(bkgd, (unsigned char)(info->background_b >> 8));
		ucvector_push_back(bkgd, (unsigned char)(info->background_b & 255));
	}
	else if (info->color.colortype == LCT_PALETTE)
		ucvector_push_back(bkgd, (unsigned char)(info->background_r & 255));
}

unsigned int	add_chunk_bkgd(ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	ucvector		bkgd;

	ucvector_init(&bkgd);
	bkgd_fill(&bkgd, info);
	error = add_chunk(out, "bKGD", bkgd.data, bkgd.size);
	ucvector_cleanup(&bkgd);
	return (error);
}

unsigned int	add_chunk_chrm(ucvector *out, const t_png_info *info)
{
	unsigned int	error;
	ucvector		data;

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

unsigned int	add_chunk_iccp(ucvector *out, const t_png_info *info,
			t_compress_settings *zlibsettings)
{
	unsigned int	error;
	ucvector		data;
	ucvector		compressed;
	size_t			i;

	ucvector_init(&data);
	ucvector_init(&compressed);
	i = 0;
	while (info->iccp_name[i] != 0)
		ucvector_push_back(&data, (unsigned char)info->iccp_name[i++]);
	if (i < 1 || i > 79)
		return (89);
	ucvector_push_back(&data, 0);
	ucvector_push_back(&data, 0);
	error = zlib_compress(&compressed.data, &compressed.size,
			info->iccp_profile, info->iccp_profile_size, zlibsettings);
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
