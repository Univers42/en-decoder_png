/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:25:04 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:25:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZLIB_H
# define ZLIB_H

# include "inflate.h"
# include "deflate.h"

unsigned	zlib_decompress(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const LodePNGDecompressSettings *settings);
unsigned	zlib_compress(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const LodePNGCompressSettings *settings);

#endif
