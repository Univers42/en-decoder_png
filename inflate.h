/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:15:46 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:15:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFLATE_H
# define INFLATE_H

unsigned int	inflate_no_compression(ucvector *out, const unsigned char *in,
				size_t	*bp, size_t *pos, size_t inlength);
unsigned int	lodepng_inflatev(ucvector *out, const unsigned char *in,
				size_t insize,
				const t_decompress_settings	*settings);
unsigned int	lodepng_inflate(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const t_decompress_settings	*settings);
unsigned int	inflate(unsigned char **out, size_t *outsize,
				const unsigned char *in, size_t insize,
				const t_decompress_settings	*settings);

#endif
