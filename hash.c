/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:17:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	hash_alloc(Hash *hash, unsigned int windowsize)
{
	hash->head = (int *)lodepng_malloc(sizeof(int) * HASH_NUM_VALUES);
	hash->val = (int *)lodepng_malloc(sizeof(int) * windowsize);
	hash->chain = (unsigned short *)lodepng_malloc(
			sizeof(unsigned short) * windowsize);
	hash->zeros = (unsigned short *)lodepng_malloc(
			sizeof(unsigned short) * windowsize);
	hash->headz = (int *)lodepng_malloc(
			sizeof(int) * (MAX_SUPPORTED_DEFLATE_LENGTH + 1));
	hash->chainz = (unsigned short *)lodepng_malloc(
			sizeof(unsigned short) * windowsize);
	if (!hash->head || !hash->chain || !hash->val
		|| !hash->headz || !hash->chainz || !hash->zeros)
		return (83);
	return (0);
}

static void	hash_fill_tables(Hash *hash, unsigned int windowsize)
{
	unsigned int	i;

	i = 0;
	while (i != HASH_NUM_VALUES)
	{
		hash->head[i] = -1;
		i++;
	}
	i = 0;
	while (i != windowsize)
	{
		hash->val[i] = -1;
		hash->chain[i] = i;
		hash->chainz[i] = i;
		i++;
	}
	i = 0;
	while (i <= MAX_SUPPORTED_DEFLATE_LENGTH)
	{
		hash->headz[i] = -1;
		i++;
	}
}

unsigned int	hash_init(Hash *hash, unsigned int windowsize)
{
	int	err;

	err = hash_alloc(hash, windowsize);
	if (err)
		return (err);
	hash_fill_tables(hash, windowsize);
	return (0);
}

void	hash_cleanup(Hash *hash)
{
	lodepng_free(hash->head);
	lodepng_free(hash->val);
	lodepng_free(hash->chain);
	lodepng_free(hash->zeros);
	lodepng_free(hash->headz);
	lodepng_free(hash->chainz);
}

void	updateHashChain(Hash *hash, size_t wpos,
		unsigned int hashval, unsigned short numzeros)
{
	hash->val[wpos] = (int)hashval;
	if (hash->head[hashval] != -1)
		hash->chain[wpos] = hash->head[hashval];
	hash->head[hashval] = (int)wpos;
	hash->zeros[wpos] = numzeros;
	if (hash->headz[numzeros] != -1)
		hash->chainz[wpos] = hash->headz[numzeros];
	hash->headz[numzeros] = (int)wpos;
}
