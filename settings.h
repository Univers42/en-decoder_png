/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:26:56 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

# include "types.h"

void			lodepng_compress_settings_init(t_compress_settings *s);
void			lodepng_decompress_settings_init(
					t_decompress_settings *s);
void			lodepng_decoder_settings_init(
					t_decoder_settings *settings);
void			lodepng_color_profile_init(t_png_color_profile *p);
unsigned int	lodepng_color_profile_add(
					t_png_color_profile *profile,
					const unsigned int *rgba);
void			lodepng_encoder_settings_init(t_encoder_settings *s);

#endif
