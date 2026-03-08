/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_state_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:23:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_state_init(LodePNGState *state)
{
	lodepng_decoder_settings_init(&state->decoder);
	lodepng_encoder_settings_init(&state->encoder);
	lodepng_color_mode_init(&state->info_raw);
	lodepng_info_init(&state->info_png);
	state->error = 0;
}

void	lodepng_state_cleanup(LodePNGState *state)
{
	lodepng_color_mode_cleanup(&state->info_raw);
	lodepng_info_cleanup(&state->info_png);
}

void	lodepng_state_copy(LodePNGState *dest, const LodePNGState *source)
{
	lodepng_state_cleanup(dest);
	*dest = *source;
	lodepng_color_mode_init(&dest->info_raw);
	lodepng_info_init(&dest->info_png);
	dest->error = lodepng_color_mode_copy(&dest->info_raw,
			&source->info_raw);
	if (dest->error)
		return ;
	dest->error = lodepng_info_copy(&dest->info_png, &source->info_png);
}
