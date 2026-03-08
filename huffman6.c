/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:02:18 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void	gtid_repeat(unsigned int *temp, unsigned int *idx,
			unsigned int num, unsigned int value, unsigned int repeat)
{
	while (repeat > 0 && *idx < num)
	{
		temp[*idx] = value;
		++(*idx);
		--repeat;
	}
}

static void	gtid_read_cl(unsigned int *bitlen_cl,
			const unsigned char *in, size_t *bp, unsigned int hclen)
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
		bitlen_cl[g_clcl_order[i]] = read_bits_from_stream(bp, in, 3);
		++i;
	}
}

static unsigned int	gtid_decode_temp(unsigned int *temp, unsigned int num,
				const unsigned char *in, size_t *bp,
				t_huffman_tree *tree_cl, size_t inbitlength)
{
	unsigned int	idx;
	unsigned int	code;

	idx = 0;
	while (idx < num)
	{
		code = huffman_decode_symbol(in, bp, tree_cl, inbitlength);
		if (code <= 15)
			temp[idx++] = code;
		else if (code == 16)
		{
			if (idx == 0)
				return (54);
			gtid_repeat(temp, &idx, num, temp[idx - 1],
				read_bits_from_stream(bp, in, 2) + 3);
		}
		else if (code == 17)
			gtid_repeat(temp, &idx, num, 0,
				read_bits_from_stream(bp, in, 3) + 3);
		else if (code == 18)
			gtid_repeat(temp, &idx, num, 0,
				read_bits_from_stream(bp, in, 7) + 11);
		else
			return (16);
	}
	return (0);
}

static unsigned int	gtid_decode_all(unsigned int *bitlen_ll, unsigned int *bitlen_d,
				const unsigned char *in, size_t *bp,
				size_t inlength, unsigned int hlit, unsigned int hdist)
{
	unsigned int		bitlen_cl[NUM_CODE_LENGTH_CODES];
	unsigned int		temp[286 + 30];
	unsigned int		error;
	t_huffman_tree		tree_cl;

	gtid_read_cl(bitlen_cl, in, bp,
		read_bits_from_stream(bp, in, 4) + 4);
	huffman_tree_init(&tree_cl);
	error = huffman_tree_make_from_len(&tree_cl,
			bitlen_cl, NUM_CODE_LENGTH_CODES, 7);
	if (!error)
		error = gtid_decode_temp(temp, hlit + hdist,
				in, bp, &tree_cl, inlength * 8);
	huffman_tree_cleanup(&tree_cl);
	if (error)
		return (error);
	memcpy(bitlen_ll, temp, hlit * sizeof(unsigned int));
	memcpy(bitlen_d, temp + hlit, hdist * sizeof(unsigned int));
	return (0);
}

unsigned int	get_tree_inflate_dynamic(t_huffman_tree *tree_ll,
			t_huffman_tree *tree_d, const unsigned char *in,
			size_t *bp, size_t inlength)
{
	unsigned int	bitlen_ll[286];
	unsigned int	bitlen_d[30];
	unsigned int	hlit;
	unsigned int	hdist;
	unsigned int	error;

	if ((*bp + 14) > inlength * 8)
		return (49);
	hlit = read_bits_from_stream(bp, in, 5) + 257;
	hdist = read_bits_from_stream(bp, in, 5) + 1;
	error = gtid_decode_all(bitlen_ll, bitlen_d,
			in, bp, inlength, hlit, hdist);
	if (error)
		return (error);
	error = huffman_tree_make_from_len(tree_ll, bitlen_ll, hlit, 15);
	if (error)
		return (error);
	return (huffman_tree_make_from_len(tree_d, bitlen_d, hdist, 15));
}
