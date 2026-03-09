/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 01:08:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int	dg_validate_chunk(t_png_state *state,
		const unsigned char *chunk, const unsigned char *in,
		size_t insize)
{
	unsigned int	len;

	if ((size_t)((chunk - in) + 12) > insize || chunk < in)
	{
		if (state->decoder.ignore_end)
			return (0);
		state->error = 30;
		return (0);
	}
	len = lodepng_chunk_length(chunk);
	if (len > 2147483647)
	{
		if (state->decoder.ignore_end)
			return (0);
		state->error = 63;
		return (0);
	}
	if ((size_t)((chunk - in) + len + 12) > insize
		|| (chunk + len + 12) < in)
	{
		state->error = 64;
		return (0);
	}
	return (1);
}

static void	dg_post_chunk(t_png_state *state,
		const unsigned char *chunk, unsigned int unknown, unsigned int cpos)
{
	if (!state->decoder.ignore_crc && !unknown)
	{
		if (lodepng_chunk_check_crc(chunk))
			state->error = 57;
	}
	if (unknown && state->decoder.remember_unknown_chunks)
	{
		state->error = lodepng_chunk_append(
				&state->info_png.unknown_chunks_data[cpos - 1],
				&state->info_png.unknown_chunks_size[cpos - 1],
				chunk);
	}
}

void	dg_chunk_loop(t_png_state *state, t_ucvector *idat,
		const unsigned char *in, size_t insize)
{
	unsigned int		out[3];
	const unsigned char	*chunk;

	out[0] = 0;
	out[2] = 1;
	chunk = &in[33];
	while (!out[0] && !state->error)
	{
		if (!dg_validate_chunk(state, chunk, in, insize))
			break ;
		dg_handle_chunk(state, idat, chunk, out);
		if (state->error)
			break ;
		dg_post_chunk(state, chunk, out[1], out[2]);
		if (!out[0])
			chunk = lodepng_chunk_next_const(chunk);
	}
}
