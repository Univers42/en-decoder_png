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

static void	gtid_repeat(unsigned *temp, unsigned *idx,
			unsigned num, unsigned value, unsigned repeat)
{
	while (repeat > 0 && *idx < num)
	{
		temp[*idx] = value;
		++(*idx);
		--repeat;
	}
}

static void	gtid_read_cl(unsigned *bitlen_cl,
			const unsigned char *in, size_t *bp, unsigned hclen)
{
	unsigned	i;

	i = 0;
	while (i < NUM_CODE_LENGTH_CODES)
	{
		bitlen_cl[i] = 0;
		++i;
	}
	i = 0;
	while (i < hclen)
	{
		bitlen_cl[g_clcl_order[i]] = readBitsFromStream(bp, in, 3);
		++i;
	}
}

static unsigned	gtid_decode_temp(unsigned *temp, unsigned num,
				const unsigned char *in, size_t *bp,
				HuffmanTree *tree_cl, size_t inbitlength)
{
	unsigned	idx;
	unsigned	code;

	idx = 0;
	while (idx < num)
	{
		code = huffmanDecodeSymbol(in, bp, tree_cl, inbitlength);
		if (code <= 15)
			temp[idx++] = code;
		else if (code == 16)
		{
			if (idx == 0)
				return (54);
			gtid_repeat(temp, &idx, num, temp[idx - 1],
				readBitsFromStream(bp, in, 2) + 3);
		}
		else if (code == 17)
			gtid_repeat(temp, &idx, num, 0,
				readBitsFromStream(bp, in, 3) + 3);
		else if (code == 18)
			gtid_repeat(temp, &idx, num, 0,
				readBitsFromStream(bp, in, 7) + 11);
		else
			return (16);
	}
	return (0);
}

static unsigned	gtid_decode_all(unsigned *bitlen_ll, unsigned *bitlen_d,
				const unsigned char *in, size_t *bp,
				size_t inlength, unsigned hlit, unsigned hdist)
{
	unsigned		bitlen_cl[NUM_CODE_LENGTH_CODES];
	unsigned		temp[286 + 30];
	unsigned		error;
	HuffmanTree		tree_cl;

	gtid_read_cl(bitlen_cl, in, bp,
		readBitsFromStream(bp, in, 4) + 4);
	HuffmanTree_init(&tree_cl);
	error = HuffmanTree_makeFromLengths(&tree_cl,
			bitlen_cl, NUM_CODE_LENGTH_CODES, 7);
	if (!error)
		error = gtid_decode_temp(temp, hlit + hdist,
				in, bp, &tree_cl, inlength * 8);
	HuffmanTree_cleanup(&tree_cl);
	if (error)
		return (error);
	memcpy(bitlen_ll, temp, hlit * sizeof(unsigned));
	memcpy(bitlen_d, temp + hlit, hdist * sizeof(unsigned));
	return (0);
}

unsigned	getTreeInflateDynamic(HuffmanTree *tree_ll,
			HuffmanTree *tree_d, const unsigned char *in,
			size_t *bp, size_t inlength)
{
	unsigned	bitlen_ll[286];
	unsigned	bitlen_d[30];
	unsigned	hlit;
	unsigned	hdist;
	unsigned	error;

	if ((*bp + 14) > inlength * 8)
		return (49);
	hlit = readBitsFromStream(bp, in, 5) + 257;
	hdist = readBitsFromStream(bp, in, 5) + 1;
	error = gtid_decode_all(bitlen_ll, bitlen_d,
			in, bp, inlength, hlit, hdist);
	if (error)
		return (error);
	error = HuffmanTree_makeFromLengths(tree_ll, bitlen_ll, hlit, 15);
	if (error)
		return (error);
	return (HuffmanTree_makeFromLengths(tree_d, bitlen_d, hdist, 15));
}
