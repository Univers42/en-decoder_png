/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	gtid_read_cl(unsigned int *bitlen_cl,
			t_inflate_ctx *s, unsigned int hclen)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_CODE_LENGTH_CODES)
	{
		bitlen_cl[i] = 0;
		++i;
	}
	i = 0;
	while (i < hclen)
	{
		bitlen_cl[clcl_order()[i]] = read_bits_from_stream(
				&s->bp, s->in, 3);
		++i;
	}
}

static unsigned int	gtid_do_code(unsigned int code, unsigned int *dest,
				unsigned int rem, t_inflate_ctx *s)
{
	unsigned int	val;
	unsigned int	rep;
	unsigned int	n;

	val = 0;
	if (code == 16)
	{
		val = dest[-1];
		rep = read_bits_from_stream(&s->bp, s->in, 2) + 3;
	}
	else if (code == 17)
		rep = read_bits_from_stream(&s->bp, s->in, 3) + 3;
	else
		rep = read_bits_from_stream(&s->bp, s->in, 7) + 11;
	n = 0;
	while (rep > 0 && n < rem)
	{
		dest[n++] = val;
		--rep;
	}
	return (n);
}

static unsigned int	gtid_decode_temp(unsigned int *temp, unsigned int num,
				t_inflate_ctx *s, t_huffman_tree *tree_cl)
{
	unsigned int	idx;
	unsigned int	code;

	idx = 0;
	while (idx < num)
	{
		code = huffman_decode_symbol(s->in, &s->bp,
				tree_cl, s->insize * 8);
		if (code <= 15)
			temp[idx++] = code;
		else if (code >= 16 && code <= 18)
		{
			if (code == 16 && idx == 0)
				return (54);
			idx += gtid_do_code(code, temp + idx,
					num - idx, s);
		}
		else
			return (16);
	}
	return (0);
}

static unsigned int	gtid_decode_all(unsigned int *bitlen,
				unsigned int total, t_inflate_ctx *s)
{
	unsigned int	bitlen_cl[NUM_CODE_LENGTH_CODES];
	unsigned int	error;
	t_huffman_tree	tree_cl;

	gtid_read_cl(bitlen_cl, s,
		read_bits_from_stream(&s->bp, s->in, 4) + 4);
	huffman_tree_init(&tree_cl);
	error = huffman_tree_make_from_len(&tree_cl,
			bitlen_cl, NUM_CODE_LENGTH_CODES, 7);
	if (!error)
		error = gtid_decode_temp(bitlen, total,
				s, &tree_cl);
	huffman_tree_cleanup(&tree_cl);
	return (error);
}

unsigned int	get_tree_inflate_dynamic(t_huffman_tree *tree_ll,
			t_huffman_tree *tree_d, t_inflate_ctx *s)
{
	unsigned int	bitlen[316];
	unsigned int	hlit;
	unsigned int	hdist;
	unsigned int	error;

	if ((s->bp + 14) > s->insize * 8)
		return (49);
	hlit = read_bits_from_stream(&s->bp, s->in, 5) + 257;
	hdist = read_bits_from_stream(&s->bp, s->in, 5) + 1;
	error = gtid_decode_all(bitlen, hlit + hdist, s);
	if (error)
		return (error);
	error = huffman_tree_make_from_len(tree_ll, bitlen, hlit, 15);
	if (error)
		return (error);
	return (huffman_tree_make_from_len(tree_d,
			bitlen + hlit, hdist, 15));
}
