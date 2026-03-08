/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:25:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:55:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	zlib_decompress(unsigned char **out, size_t *outsize,
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
	return (inflate(out, outsize, in + 2, insize - 2, settings));
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

static void	zc_assemble(unsigned char **out, size_t *outsize,
			unsigned char *deflatedata, size_t deflatesize,
			unsigned char header[2], unsigned int adler_val)
{
	ucvector	v;
	size_t		i;

	ucvector_init(&v);
	ucvector_push_back(&v, header[0]);
	ucvector_push_back(&v, header[1]);
	i = 0;
	while (i < deflatesize)
		ucvector_push_back(&v, deflatedata[i++]);
	ucvector_push_back(&v, (unsigned char)((adler_val >> 24) & 0xff));
	ucvector_push_back(&v, (unsigned char)((adler_val >> 16) & 0xff));
	ucvector_push_back(&v, (unsigned char)((adler_val >> 8) & 0xff));
	ucvector_push_back(&v, (unsigned char)(adler_val & 0xff));
	*out = v.data;
	*outsize = v.size;
}

unsigned int	zlib_compress(unsigned char **out, size_t *outsize,
			const unsigned char *in, size_t insize,
			const t_compress_settings *settings)
{
	unsigned int	error;
	unsigned char	*deflatedata;
	size_t			deflatesize;
	unsigned char	header[2];
	unsigned int	adler_val;

	deflatedata = 0;
	deflatesize = 0;
	adler_val = adler32(in, (unsigned int)insize);
	if (settings)
		zc_build_header(header, settings->windowsize);
	else
		zc_build_header(header, 32768);
	error = deflate(&deflatedata, &deflatesize, in, insize, settings);
	if (!error)
		zc_assemble(out, outsize, deflatedata, deflatesize,
			header, adler_val);
	lodepng_free(deflatedata);
	return (error);
}
