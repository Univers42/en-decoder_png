/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overflow.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:35:47 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:35:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef OVERFLOW_H
#  define OVERFLOW_H

int		lodepng_mulofl(size_t a, size_t b, size_t *result);
int		lodepng_addofl(size_t a, size_t b, size_t *result);
int		lodepng_pixel_overflow(unsigned int w, unsigned int h,
			const t_png_color_mode *pngcolor,
			const t_png_color_mode *rawcolor);

# endif