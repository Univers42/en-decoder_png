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

void	lodepng_compress_settings_init(LodePNGCompressSettings *s);
void	lodepng_decompress_settings_init(LodePNGDecompressSettings *s);
void	lodepng_decoder_settings_init(LodePNGDecoderSettings *settings);
void	lodepng_color_profile_init(LodePNGColorProfile *p);
void	lodepng_encoder_settings_init(LodePNGEncoderSettings *s);

# endif