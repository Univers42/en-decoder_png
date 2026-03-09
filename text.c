/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 23:03:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_text_init(t_png_info *info)
{
	info->text_num = 0;
	info->text_keys = NULL;
	info->text_strings = NULL;
}

void	lodepng_text_cleanup(t_png_info *info)
{
	size_t	i;

	i = 0;
	while (i != info->text_num)
	{
		string_cleanup(&info->text_keys[i]);
		string_cleanup(&info->text_strings[i]);
		i++;
	}
	lodepng_free(info->text_keys);
	lodepng_free(info->text_strings);
}

unsigned int	lodepng_text_copy(t_png_info *dest,
		const t_png_info *source)
{
	size_t			i;
	unsigned int	error;

	dest->text_keys = 0;
	dest->text_strings = 0;
	dest->text_num = 0;
	i = 0;
	while (i != source->text_num)
	{
		error = lodepng_add_text(dest,
				source->text_keys[i], source->text_strings[i]);
		if (error)
			return (error);
		i++;
	}
	return (0);
}

void	lodepng_clear_text(t_png_info *info)
{
	lodepng_text_cleanup(info);
}

unsigned int	lodepng_add_text(t_png_info *info, const char *key,
		const char *str)
{
	char	**new_keys;
	char	**new_strings;

	new_keys = (char **)(lodepng_realloc(info->text_keys,
				sizeof(char *) * (info->text_num + 1)));
	new_strings = (char **)(lodepng_realloc(info->text_strings,
				sizeof(char *) * (info->text_num + 1)));
	if (!new_keys || !new_strings)
	{
		lodepng_free(new_keys);
		lodepng_free(new_strings);
		return (83);
	}
	++info->text_num;
	info->text_keys = new_keys;
	info->text_strings = new_strings;
	info->text_keys[info->text_num - 1] = alloc_string(key);
	info->text_strings[info->text_num - 1] = alloc_string(str);
	return (0);
}
