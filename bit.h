/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:52:08 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 22:52:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef BIT_H
#  define BIT_H

#  include <unistd.h>
#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>
#  include "ucvector.h"
#  include "uivector.h"

#  ifndef READBIT
#   define READBIT(bp, bs) (((bs)[(bp) >> 3] >> ((bp) & 0x7)) & 1u)
#  endif

unsigned int	lodepng_read32bitInt(const unsigned char *buffer);
void			lodepng_set32bitInt(unsigned char *buffer,
					unsigned int value);
void			lodepng_add32bitInt(ucvector *buffer, unsigned int value);
void			addBitToStream(ucvector *bs, size_t *bp, unsigned int bit);
void			addBitsToStream(size_t *bp, ucvector *bs,
					unsigned int value, size_t nbits);
void			addBitsToStreamReversed(size_t *bp, ucvector *bs,
					unsigned int value, size_t nbits);
unsigned char	readBit(size_t bitpointer, const unsigned char *bs);
unsigned char	readBitFromStream(size_t *bp, const unsigned char *bs);
unsigned int	readBitsFromStream(size_t *bp, const unsigned char *bs,
					size_t nbits);
unsigned char	readBitFromReversedStream(size_t *bp,
					const unsigned char *bs);
unsigned int	readBitsFromReversedStream(size_t *bp,
					const unsigned char *bs, size_t nbits);
void			setBitOfReversedStream0(size_t *bp, unsigned char *bs,
					unsigned char bit);
void			setBitOfReversedStream(size_t *bp, unsigned char *bs,
					unsigned char bit);
unsigned int	lodepng_chunk_length(const unsigned char *chunk);
unsigned int	getValueRequiredBits(unsigned char value);

# endif