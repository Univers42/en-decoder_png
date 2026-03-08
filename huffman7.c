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

static unsigned	ihb_dist_error(unsigned code_d, size_t bp,
				size_t inbitlength)
{
	if (code_d == (unsigned)(-1))
	{
		if (bp > inbitlength)
			return (10);
		return (11);
	}
	return (18);
}

static unsigned	ihb_copy_back(ucvector *out, size_t *pos,
				size_t length, unsigned distance)
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

static unsigned	ihb_length_match(ucvector *out, const unsigned char *in,
				size_t *bp, size_t *pos, HuffmanTree *tree_d,
				size_t inbitlength, unsigned code_ll)
{
	unsigned	code_d;
	unsigned	distance;
	unsigned	numextra;
	size_t		length;

	length = LENGTHBASE[code_ll - FIRST_LENGTH_CODE_INDEX];
	numextra = LENGTHEXTRA[code_ll - FIRST_LENGTH_CODE_INDEX];
	if ((*bp + numextra) > inbitlength)
		return (51);
	length += readBitsFromStream(bp, in, numextra);
	code_d = huffmanDecodeSymbol(in, bp, tree_d, inbitlength);
	if (code_d > 29)
		return (ihb_dist_error(code_d, *bp, inbitlength));
	distance = DISTANCEBASE[code_d];
	numextra = DISTANCEEXTRA[code_d];
	if ((*bp + numextra) > inbitlength)
		return (51);
	distance += readBitsFromStream(bp, in, numextra);
	return (ihb_copy_back(out, pos, length, distance));
}

static unsigned	ihb_decode_one(ucvector *out, const unsigned char *in,
				size_t *bp, size_t *pos, HuffmanTree *tree_ll,
				HuffmanTree *tree_d, size_t inbitlength)
{
	unsigned	code_ll;

	code_ll = huffmanDecodeSymbol(in, bp, tree_ll, inbitlength);
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

unsigned	inflateHuffmanBlock(ucvector *out,
			const unsigned char *in, size_t *bp,
			size_t *pos, size_t inlength, unsigned btype)
{
	unsigned	error;
	HuffmanTree	tree_ll;
	HuffmanTree	tree_d;

	error = 0;
	HuffmanTree_init(&tree_ll);
	HuffmanTree_init(&tree_d);
	if (btype == 1)
		getTreeInflateFixed(&tree_ll, &tree_d);
	else if (btype == 2)
		error = getTreeInflateDynamic(&tree_ll, &tree_d,
				in, bp, inlength);
	while (!error)
		error = ihb_decode_one(out, in, bp, pos,
				&tree_ll, &tree_d, inlength * 8);
	if (error == 256)
		error = 0;
	HuffmanTree_cleanup(&tree_ll);
	HuffmanTree_cleanup(&tree_d);
	return (error);
}
