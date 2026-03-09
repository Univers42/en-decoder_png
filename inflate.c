/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:15:46 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static unsigned int	inc_header(t_inflate_ctx *s,
				size_t *p, unsigned int *len)
{
	unsigned int	nlen;

	while ((s->bp & 0x7) != 0)
		++s->bp;
	*p = s->bp / 8;
	if (*p + 4 >= s->insize)
		return (52);
	*len = s->in[*p] + 256u * s->in[*p + 1];
	*p += 2;
	nlen = s->in[*p] + 256u * s->in[*p + 1];
	*p += 2;
	if (*len + nlen != 65535)
		return (21);
	return (0);
}

unsigned int	inflate_no_compression(t_inflate_ctx *s)
{
	size_t			p;
	unsigned int	len;
	unsigned int	n;
	unsigned int	error;

	error = inc_header(s, &p, &len);
	if (error)
		return (error);
	if (!ucvector_resize(s->out, s->pos + len))
		return (83);
	if (p + len > s->insize)
		return (23);
	n = 0;
	while (n < len)
	{
		s->out->data[s->pos++] = s->in[p++];
		++n;
	}
	s->bp = p * 8;
	return (0);
}

static unsigned int	iv_inflate_loop(t_inflate_ctx *s)
{
	unsigned int	bfinal;
	unsigned int	btype;
	unsigned int	error;

	bfinal = 0;
	error = 0;
	while (!bfinal)
	{
		if (s->bp + 2 >= s->insize * 8)
			return (52);
		bfinal = read_bit_from_stream(&s->bp, s->in);
		btype = 1u * read_bit_from_stream(&s->bp, s->in);
		btype += 2u * read_bit_from_stream(&s->bp, s->in);
		if (btype == 3)
			return (20);
		else if (btype == 0)
			error = inflate_no_compression(s);
		else
			error = inflate_huffman_block(s, btype);
		if (error)
			return (error);
	}
	return (error);
}

unsigned int	lodepng_inflatev(t_ucvector *out, const unsigned char *in,
			size_t insize, const t_decompress_settings *settings)
{
	t_inflate_ctx	s;

	(void)settings;
	s = (t_inflate_ctx){out, in, 0, 0, insize};
	return (iv_inflate_loop(&s));
}
