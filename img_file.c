/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:54:23 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 18:51:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

long	lodepng_filesize(const char *filename)
{
	FILE	*file;
	long	size;

	file = fopen(filename, "rb");
	if (!file)
		return (-1);
	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		return (-1);
	}
	size = ftell(file);
	if (size == LONG_MAX)
		size = -1;
	fclose(file);
	return (size);
}

unsigned int	lodepng_buffer_file(unsigned char *out,
			size_t size, const char *filename)
{
	FILE	*file;
	size_t	readsize;

	file = fopen(filename, "rb");
	if (!file)
		return (78);
	readsize = fread(out, 1, size, file);
	fclose(file);
	if (readsize != size)
		return (78);
	return (0);
}

unsigned int	lodepng_load_file(unsigned char **out,
			size_t *outsize, const char *filename)
{
	long	size;

	size = lodepng_filesize(filename);
	if (size < 0)
		return (78);
	*outsize = (size_t)size;
	*out = (unsigned char *)lodepng_malloc((size_t)size);
	if (!(*out) && size > 0)
		return (83);
	return (lodepng_buffer_file(*out, (size_t)size, filename));
}

unsigned int	lodepng_save_file(const unsigned char *buffer,
			size_t buffersize, const char *filename)
{
	FILE	*file;

	file = fopen(filename, "wb");
	if (!file)
		return (79);
	fwrite(buffer, 1, buffersize, file);
	fclose(file);
	return (0);
}
