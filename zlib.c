/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:25:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 01:42:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	zlib_decompress(t_ucvector *out,
			const unsigned char *in, size_t insize,
			const t_decompress_settings *settings)
{
	unsigned int	cmf;
	unsigned int	flgs;

	if (insize < 2)
		return (53);
	cmf = in[0];
	flgs = in[1];
	if ((cmf & 0x0F) != 8)
		return (24);
	if ((cmf >> 4) > 7)
		return (25);
	if (((cmf << 8) + flgs) % 31 != 0)
		return (24);
	if (flgs & 32)
		return (26);
	return (lodepng_inflatev(out, in + 2, insize - 2, settings));
}

static void	zc_build_header(unsigned char header[2], unsigned int ws)
{
	unsigned int	cinfo;
	unsigned int	flg;
	unsigned int	fcheck;

	if (ws < 256)
		ws = 256;
	if (ws > 32768)
		ws = 32768;
	cinfo = 0;
	while (cinfo < 7 && (1u << (cinfo + 8)) < ws)
		++cinfo;
	header[0] = (unsigned char)((cinfo << 4) | 8);
	flg = (2u << 6);
	fcheck = (31 - (((header[0] << 8) | flg) % 31)) % 31;
	header[1] = (unsigned char)(flg | fcheck);
}

static void	zc_append_data(t_ucvector *out,
			t_ucvector *def, unsigned char h[2], unsigned int adl)
{
	size_t	i;

	ucvector_push_back(out, h[0]);
	ucvector_push_back(out, h[1]);
	i = 0;
	while (i < def->size)
		ucvector_push_back(out, def->data[i++]);
	ucvector_push_back(out, (unsigned char)((adl >> 24) & 0xff));
	ucvector_push_back(out, (unsigned char)((adl >> 16) & 0xff));
	ucvector_push_back(out, (unsigned char)((adl >> 8) & 0xff));
	ucvector_push_back(out, (unsigned char)(adl & 0xff));
}

unsigned int	zlib_compress(t_ucvector *out,
			const unsigned char *in, size_t insize,
			const t_compress_settings *settings)
{
	unsigned int	error;
	t_ucvector		defdata;
	unsigned char	header[2];
	unsigned int	adler_val;

	adler_val = adler32(in, (unsigned int)insize);
	if (settings)
		zc_build_header(header, settings->windowsize);
	else
		zc_build_header(header, 32768);
	ucvector_init(&defdata);
	error = lodepng_deflatev(&defdata, in, insize, settings);
	if (!error)
		zc_append_data(out, &defdata, header, adler_val);
	ucvector_cleanup(&defdata);
	return (error);
}
