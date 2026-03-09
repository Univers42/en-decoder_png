/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	ldf_decode(unsigned char **out, t_png_state *st,
		const char *filename)
{
	unsigned char	*buf;
	size_t			bsz;
	unsigned int	error;

	buf = 0;
	error = lodepng_load_file(&buf, &bsz, filename);
	if (!error)
		error = lodepng_decode(out, st, buf, bsz);
	lodepng_free(buf);
	return (error);
}

unsigned int	lodepng_decode32_file(unsigned char **out, unsigned int *w,
		unsigned int *h, const char *filename)
{
	t_png_state		st;
	unsigned int	error;

	lodepng_state_init(&st);
	st.info_raw.colortype = LCT_RGBA;
	st.info_raw.bitdepth = 8;
	error = ldf_decode(out, &st, filename);
	*w = st.info_png.width;
	*h = st.info_png.height;
	lodepng_state_cleanup(&st);
	return (error);
}

unsigned int	lodepng_decode24_file(unsigned char **out, unsigned int *w,
		unsigned int *h, const char *filename)
{
	t_png_state		st;
	unsigned int	error;

	lodepng_state_init(&st);
	st.info_raw.colortype = LCT_RGB;
	st.info_raw.bitdepth = 8;
	error = ldf_decode(out, &st, filename);
	*w = st.info_png.width;
	*h = st.info_png.height;
	lodepng_state_cleanup(&st);
	return (error);
}
