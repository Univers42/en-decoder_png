/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman7.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 04:01:28 by dlesieur         ###   ########.fr       */
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

static unsigned int	ihb_copy_back(t_ucvector *out, size_t *pos,
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

static unsigned int	ihb_length_match(t_inflate_ctx *s,
				t_huffman_tree *tree_d, unsigned int code_ll)
{
	unsigned int	code_d;
	unsigned int	distance;
	unsigned int	numextra;
	size_t			length;

	length = hft()->lengthbase[code_ll - FIRST_LENGTH_CODE_INDEX];
	numextra = hft()->lengthextra[code_ll - FIRST_LENGTH_CODE_INDEX];
	if ((s->bp + numextra) > s->insize * 8)
		return (51);
	length += read_bits_from_stream(&s->bp, s->in, numextra);
	code_d = huffman_decode_symbol(s->in, &s->bp, tree_d,
			s->insize * 8);
	if (code_d > 29)
		return (ihb_dist_error(code_d, s->bp, s->insize * 8));
	distance = hft()->distancebase[code_d];
	numextra = hft()->distanceextra[code_d];
	if ((s->bp + numextra) > s->insize * 8)
		return (51);
	distance += read_bits_from_stream(&s->bp, s->in, numextra);
	return (ihb_copy_back(s->out, &s->pos, length, distance));
}

static unsigned int	ihb_decode_one(t_inflate_ctx *s,
				t_huffman_tree *tree_ll, t_huffman_tree *tree_d)
{
	unsigned int	code_ll;

	code_ll = huffman_decode_symbol(s->in, &s->bp, tree_ll,
			s->insize * 8);
	if (code_ll <= 255)
	{
		if (!ucvector_resize(s->out, s->pos + 1))
			return (83);
		s->out->data[s->pos] = (unsigned char)code_ll;
		++s->pos;
		return (0);
	}
	if (code_ll >= FIRST_LENGTH_CODE_INDEX
		&& code_ll <= LAST_LENGTH_CODE_INDEX)
		return (ihb_length_match(s, tree_d, code_ll));
	if (code_ll == 256)
		return (256);
	if (s->bp > s->insize * 8)
		return (10);
	return (11);
}

unsigned int	inflate_huffman_block(t_inflate_ctx *s,
			unsigned int btype)
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
		error = get_tree_inflate_dynamic(&tree_ll, &tree_d, s);
	while (!error)
		error = ihb_decode_one(s, &tree_ll, &tree_d);
	if (error == 256)
		error = 0;
	huffman_tree_cleanup(&tree_ll);
	huffman_tree_cleanup(&tree_d);
	return (error);
}
