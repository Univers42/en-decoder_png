/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman7.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:49:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	ihb_dist_error(unsigned int code_d, size_t bp,
				size_t inbitlength)
{
	if (code_d == (unsigned int)(-1))
	{
		if (bp > inbitlength)
			return (10);
		return (11);
	}
	return (18);
}

static unsigned int	ihb_copy_back(ucvector *out, size_t *pos,
				size_t length, unsigned int distance)
{
	size_t	start;
	size_t	backward;
	size_t	forward;

	start = *pos;
	if (distance > start)
		return (52);
	backward = start - distance;
	if (!ucvector_resize(out, *pos + length))
		return (83);
	if (distance < length)
	{
		forward = 0;
		while (forward < length)
		{
			out->data[(*pos)++] = out->data[backward++];
			++forward;
		}
	}
	else
	{
		memcpy(out->data + *pos, out->data + backward, length);
		*pos += length;
	}
	return (0);
}

static unsigned int	ihb_length_match(ucvector *out, const unsigned char *in,
				size_t *bp, size_t *pos, t_huffman_tree *tree_d,
				size_t inbitlength, unsigned int code_ll)
{
	unsigned int	code_d;
	unsigned int	distance;
	unsigned int	numextra;
	size_t		length;

	length = g_lengthbase[code_ll - FIRST_LENGTH_CODE_INDEX];
	numextra = g_lengthextra[code_ll - FIRST_LENGTH_CODE_INDEX];
	if ((*bp + numextra) > inbitlength)
		return (51);
	length += read_bits_from_stream(bp, in, numextra);
	code_d = huffman_decode_symbol(in, bp, tree_d, inbitlength);
	if (code_d > 29)
		return (ihb_dist_error(code_d, *bp, inbitlength));
	distance = g_distancebase[code_d];
	numextra = g_distanceextra[code_d];
	if ((*bp + numextra) > inbitlength)
		return (51);
	distance += read_bits_from_stream(bp, in, numextra);
	return (ihb_copy_back(out, pos, length, distance));
}

static unsigned int	ihb_decode_one(ucvector *out, const unsigned char *in,
				size_t *bp, size_t *pos, t_huffman_tree *tree_ll,
				t_huffman_tree *tree_d, size_t inbitlength)
{
	unsigned int	code_ll;

	code_ll = huffman_decode_symbol(in, bp, tree_ll, inbitlength);
	if (code_ll <= 255)
	{
		if (!ucvector_resize(out, (*pos) + 1))
			return (83);
		out->data[*pos] = (unsigned char)code_ll;
		++(*pos);
		return (0);
	}
	if (code_ll >= FIRST_LENGTH_CODE_INDEX
		&& code_ll <= LAST_LENGTH_CODE_INDEX)
		return (ihb_length_match(out, in, bp, pos, tree_d,
				inbitlength, code_ll));
	if (code_ll == 256)
		return (256);
	if (*bp > inbitlength)
		return (10);
	return (11);
}

unsigned int	inflate_huffman_block(ucvector *out,
			const unsigned char *in, size_t *bp,
			size_t *pos, size_t inlength, unsigned int btype)
{
	unsigned int	error;
	t_huffman_tree	tree_ll;
	t_huffman_tree	tree_d;

	error = 0;
	huffman_tree_init(&tree_ll);
	huffman_tree_init(&tree_d);
	if (btype == 1)
		get_tree_inflate_fixed(&tree_ll, &tree_d);
	else if (btype == 2)
		error = get_tree_inflate_dynamic(&tree_ll, &tree_d,
				in, bp, inlength);
	while (!error)
		error = ihb_decode_one(out, in, bp, pos,
				&tree_ll, &tree_d, inlength * 8);
	if (error == 256)
		error = 0;
	huffman_tree_cleanup(&tree_ll);
	huffman_tree_cleanup(&tree_d);
	return (error);
}
