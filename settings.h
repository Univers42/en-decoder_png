/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:26:56 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:26:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PNGWRITER_SETTINGS_H
#  define PNGWRITER_SETTINGS_H

#  include "types.h"

void	lodepng_compress_settings_init(t_compress_settings *s);
void	lodepng_decompress_settings_init(t_decompress_settings *s);
void	lodepng_decoder_settings_init(t_decoder_settings *settings);
void	lodepng_color_profile_init(t_png_color_profile *p);
unsigned int	lodepng_color_profile_add(t_png_color_profile *profile,
				unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void	lodepng_encoder_settings_init(t_encoder_settings *s);

# endif