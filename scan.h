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

unsigned	postProcessScanlines(unsigned char *out, unsigned char *in,
				unsigned w, unsigned h,
				const LodePNGInfo *info_png);
unsigned	readChunk_PLTE(LodePNGColorMode *color,
				const unsigned char *data, size_t chunkLength);
unsigned	readChunk_tRNS(LodePNGColorMode *color,
				const unsigned char *data, size_t chunkLength);
size_t		lodepng_get_raw_size_idat(unsigned w, unsigned h,
				const LodePNGColorMode *color);
void		preProcessScanlines(unsigned char **out, size_t *outsize,
				const unsigned char *in, unsigned w, unsigned h,
				const LodePNGInfo *info,
				const LodePNGEncoderSettings *settings);

#endif
