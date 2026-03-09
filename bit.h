/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:52:08 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 23:16:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIT_H
# define BIT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "ucvector.h"
# include "uivector.h"

unsigned int	lodepng_read_32bit_int(const unsigned char *buffer);
void			lodepng_set_32bit_int(unsigned char *buffer,
					unsigned int value);
void			lodepng_add_32bit_int(t_ucvector *buffer, unsigned int value);
void			add_bit_to_stream(t_ucvector *bs, size_t *bp, unsigned int bit);
void			add_bits_to_stream(size_t *bp, t_ucvector *bs,
					unsigned int value, size_t nbits);
void			add_bits_to_stream_rev(size_t *bp, t_ucvector *bs,
					unsigned int value, size_t nbits);
unsigned char	read_bit(size_t bitpointer, const unsigned char *bs);
unsigned char	read_bit_from_stream(size_t *bp, const unsigned char *bs);
unsigned int	read_bits_from_stream(size_t *bp, const unsigned char *bs,
					size_t	nbits);
unsigned char	read_bit_from_rev_stream(size_t *bp,
					const unsigned char	*bs);
unsigned int	read_bits_from_rev_stream(size_t *bp,
					const unsigned char	*bs, size_t nbits);
void			set_bit_of_rev_stream0(size_t *bp, unsigned char *bs,
					unsigned char bit);
void			set_bit_of_rev_stream(size_t *bp, unsigned char *bs,
					unsigned char bit);
unsigned int	lodepng_chunk_length(const unsigned char *chunk);
unsigned int	get_value_required_bits(unsigned char value);

#endif
