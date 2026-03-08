/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:24:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned	deflate(unsigned char **out, size_t *outsize,
		const unsigned char *in, size_t insize,
		const LodePNGCompressSettings *settings)
{
	if (settings->custom_deflate)
		return (settings->custom_deflate(out, outsize,
				in, insize, settings));
	return (lodepng_deflate(out, outsize, in, insize, settings));
}

/*
** RFC 1951 fixed Huffman literal/length tree (288 symbols):
**   0-143   : 8-bit codes
**   144-255 : 9-bit codes
**   256-279 : 7-bit codes
**   280-287 : 8-bit codes
*/
void	generateFixedLitLenTree(HuffmanTree *tree)
{
	unsigned	i;
	unsigned	bl[288];

	i = 0;
	while (i <= 143)
		bl[i++] = 8;
	while (i <= 255)
		bl[i++] = 9;
	while (i <= 279)
		bl[i++] = 7;
	while (i <= 287)
		bl[i++] = 8;
	HuffmanTree_makeFromLengths(tree, bl, 288, 15);
}

void	generateFixedDistanceTree(HuffmanTree *tree)
{
	unsigned	i;
	unsigned	bl[30];

	i = 0;
	while (i < 30)
		bl[i++] = 5;
	HuffmanTree_makeFromLengths(tree, bl, 30, 15);
}

void	getTreeInflateFixed(HuffmanTree *tree_ll, HuffmanTree *tree_d)
{
	generateFixedLitLenTree(tree_ll);
	generateFixedDistanceTree(tree_d);
}
