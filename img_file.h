/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_file.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:54:23 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:12:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMG_FILE_H
# define IMG_FILE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>

long			lodepng_filesize(const char *filename);
unsigned int	lodepng_buffer_file(unsigned char *out,
					size_t size, const char *filename);
unsigned int	lodepng_load_file(unsigned char **out,
					size_t *outsize, const char *filename);
unsigned int	lodepng_save_file(const unsigned char *buffer,
					size_t buffersize, const char *filename);

#endif