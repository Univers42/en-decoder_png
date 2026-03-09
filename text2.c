/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/09 00:12:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	lodepng_itext_init(t_png_info *info)
{
	info->itext_num = 0;
	info->itext_keys = NULL;
	info->itext_langtags = NULL;
	info->itext_transkeys = NULL;
	info->itext_strings = NULL;
}

void	lodepng_itext_cleanup(t_png_info *info)
{
	size_t	i;

	i = 0;
	while (i != info->itext_num)
	{
		string_cleanup(&info->itext_keys[i]);
		string_cleanup(&info->itext_langtags[i]);
		string_cleanup(&info->itext_transkeys[i]);
		string_cleanup(&info->itext_strings[i]);
		i++;
	}
	lodepng_free(info->itext_keys);
	lodepng_free(info->itext_langtags);
	lodepng_free(info->itext_transkeys);
	lodepng_free(info->itext_strings);
}

static int	itext_alloc_fail(char **nk, char **nl,
		char **nt, char **ns)
{
	if (!nk || !nl || !nt || !ns)
	{
		lodepng_free(nk);
		lodepng_free(nl);
		lodepng_free(nt);
		lodepng_free(ns);
		return (1);
	}
	return (0);
}

static unsigned int	itext_copy_entry(t_png_info *dest,
		const t_png_info *src, size_t i)
{
	char	**nk;
	char	**nl;
	char	**nt;
	char	**ns;

	nk = (char **)lodepng_realloc(dest->itext_keys,
			sizeof(char *) * (dest->itext_num + 1));
	nl = (char **)lodepng_realloc(dest->itext_langtags,
			sizeof(char *) * (dest->itext_num + 1));
	nt = (char **)lodepng_realloc(dest->itext_transkeys,
			sizeof(char *) * (dest->itext_num + 1));
	ns = (char **)lodepng_realloc(dest->itext_strings,
			sizeof(char *) * (dest->itext_num + 1));
	if (itext_alloc_fail(nk, nl, nt, ns))
		return (83);
	dest->itext_keys = nk;
	dest->itext_langtags = nl;
	dest->itext_transkeys = nt;
	dest->itext_strings = ns;
	nk[dest->itext_num] = alloc_string(src->itext_keys[i]);
	nl[dest->itext_num] = alloc_string(src->itext_langtags[i]);
	nt[dest->itext_num] = alloc_string(src->itext_transkeys[i]);
	ns[dest->itext_num] = alloc_string(src->itext_strings[i]);
	++dest->itext_num;
	return (0);
}

unsigned int	lodepng_itext_copy(t_png_info *dest,
		const t_png_info *source)
{
	size_t			i;
	unsigned int	error;

	dest->itext_keys = 0;
	dest->itext_langtags = 0;
	dest->itext_transkeys = 0;
	dest->itext_strings = 0;
	dest->itext_num = 0;
	i = 0;
	while (i != source->itext_num)
	{
		error = itext_copy_entry(dest, source, i);
		if (error)
			return (error);
		i++;
	}
	return (0);
}
