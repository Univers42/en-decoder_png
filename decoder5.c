/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:11:19 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 19:20:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#ifdef LODEPNG_COMPILE_DISK
unsigned int	lodepng_decode32_file(unsigned char **out, unsigned int *w,
		unsigned int *h, const char *filename)
{
	return (lodepng_decode_file(out, w, h, filename, LCT_RGBA, 8));
}

unsigned int	lodepng_decode24_file(unsigned char **out, unsigned int *w,
		unsigned int *h, const char *filename)
{
	return (lodepng_decode_file(out, w, h, filename, LCT_RGB, 8));
}
#endif
