/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 01:42:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_compress_settings_init(t_compress_settings *s)
{
	s->btype = 2;
	s->use_lz77 = 1;
	s->windowsize = 2048;
	s->minmatch = 3;
	s->nicematch = 128;
	s->lazymatching = 1;
}

void	lodepng_decompress_settings_init(t_decompress_settings *s)
{
	s->ignore_adler32 = 0;
}

void	lodepng_decoder_settings_init(t_decoder_settings *settings)
{
	settings->color_convert = 1;
	settings->read_text_chunks = 1;
	settings->remember_unknown_chunks = 0;
	settings->ignore_crc = 0;
	settings->ignore_critical = 0;
	settings->ignore_end = 0;
	lodepng_decompress_settings_init(&settings->zlibsettings);
}

void	lodepng_color_profile_init(t_png_color_profile *p)
{
	p->colored = 0;
	p->alpha = 0;
	p->key = 0;
	p->key_r = 0;
	p->key_g = 0;
	p->key_b = 0;
	p->bits = 1;
	p->numpixels = 0;
	p->numcolors = 0;
}

void	lodepng_encoder_settings_init(t_encoder_settings *s)
{
	lodepng_compress_settings_init(&s->zlibsettings);
	s->filter_palette_zero = 1;
	s->filter_strategy = LFS_MINSUM;
	s->auto_convert = 1;
	s->force_palette = 0;
	s->text_compression = 1;
	s->add_id = 0;
}
