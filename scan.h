/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:40 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:37:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
# define SCAN_H

# include <stddef.h>
# include "types.h"
# include "padding.h"
# include "bit.h"
# include "colors.h"

unsigned int		read_chunk_plte(t_png_color_mode *color,
						const unsigned char *data, size_t chunk_length);
unsigned int		read_chunk_trns(t_png_color_mode *color,
						const unsigned char *data, size_t chunk_length);
size_t				lodepng_get_raw_size_idat(unsigned int w, unsigned int h,
						const t_png_color_mode *color);
void				pre_process_scanlines(t_enc_ctx *ctx,
						const unsigned char *in);

#endif
