/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:33:01 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

unsigned int	add_chunk_ihdr(t_ucvector *out,
			const t_png_info *info, unsigned int w, unsigned int h)
{
	unsigned int	error;
	t_ucvector		header;

	ucvector_init(&header);
	lodepng_add_32bit_int(&header, w);
	lodepng_add_32bit_int(&header, h);
	ucvector_push_back(&header, (unsigned char)info->color.bitdepth);
	ucvector_push_back(&header, (unsigned char)info->color.colortype);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, 0);
	ucvector_push_back(&header, info->interlace_method);
	error = add_chunk(out, "IHDR", header.data, header.size);
	ucvector_cleanup(&header);
	return (error);
}
