/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:25:04 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 01:42:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZLIB_H
# define ZLIB_H

# include "inflate.h"
# include "deflate.h"

unsigned int	zlib_decompress(t_ucvector *out,
					const unsigned char *in, size_t insize,
					const t_decompress_settings *settings);
unsigned int	zlib_compress(t_ucvector *out,
					const unsigned char *in, size_t insize,
					const t_compress_settings *settings);

#endif
