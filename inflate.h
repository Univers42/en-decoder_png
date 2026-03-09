/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:15:46 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:23:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFLATE_H
# define INFLATE_H

unsigned int	inflate_no_compression(t_inflate_ctx *s);
unsigned int	lodepng_inflatev(t_ucvector *out,
					const unsigned char *in, size_t insize,
					const t_decompress_settings *settings);

#endif
