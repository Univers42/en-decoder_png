/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 12:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:12:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_color_mode_init(t_png_color_mode *info)
{
	info->key_defined = 0;
	info->key_r = 0;
	info->key_g = 0;
	info->colortype = LCT_RGBA;
	info->bitdepth = 8;
	info->palette = 0;
	info->palettesize = 0;
}
