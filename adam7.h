/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adam7.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:50:26 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:50:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADAM7_H
# define ADAM7_H

# include <stddef.h>

static const unsigned int	g_adam7_ix[7] = {0, 4, 0, 2, 0, 1, 0};
static const unsigned int	g_adam7_iy[7] = {0, 0, 4, 0, 2, 0, 1};
static const unsigned int	g_adam7_dx[7] = {8, 8, 4, 4, 2, 2, 1};
static const unsigned int	g_adam7_dy[7] = {8, 8, 8, 4, 4, 2, 2};

void	adam7_getpassvalues(unsigned int passw[7], unsigned int passh[7],
			size_t filter_passstart[8], size_t padded_passstart[8],
			size_t passstart[8], unsigned int w, unsigned int h, unsigned int bpp);
void	adam7_deinterlace(unsigned char *out, const unsigned char *in,
			unsigned int w, unsigned int h, unsigned int bpp);
void	adam7_interlace(unsigned char *out, const unsigned char *in,
			unsigned int w, unsigned int h, unsigned int bpp);

#endif
