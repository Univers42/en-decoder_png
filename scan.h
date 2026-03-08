/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:40 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:57:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
# define SCAN_H

# include <stddef.h>
# include "types.h"
# include "padding.h"
# include "bit.h"
# include "colors.h"

unsigned int	post_process_scanlines(unsigned char *out, unsigned char *in,
				unsigned int w, unsigned int h,
				const t_png_info	*info_png);
unsigned int	read_chunk_plte(t_png_color_mode *color,
				const unsigned char	*data, size_t chunk_length);
unsigned int	read_chunk_trns(t_png_color_mode *color,
				const unsigned char	*data, size_t chunk_length);
size_t		lodepng_get_raw_size_idat(unsigned int w, unsigned int h,
				const t_png_color_mode	*color);
void		pre_process_scanlines(unsigned char **out, size_t *outsize,
				const unsigned char *in, unsigned int w, unsigned int h,
				const t_png_info *info,
				const t_encoder_settings	*settings);

#endif
