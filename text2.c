/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:23:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	LodePNGIText_init(LodePNGInfo *info)
{
	info->itext_num = 0;
	info->itext_keys = NULL;
	info->itext_langtags = NULL;
	info->itext_transkeys = NULL;
	info->itext_strings = NULL;
}

void	LodePNGIText_cleanup(LodePNGInfo *info)
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

unsigned int	LodePNGIText_copy(LodePNGInfo *dest,
		const LodePNGInfo *source)
{
	size_t	i;

	dest->itext_keys = 0;
	dest->itext_langtags = 0;
	dest->itext_transkeys = 0;
	dest->itext_strings = 0;
	dest->itext_num = 0;
	i = 0;
	while (i != source->itext_num)
	{
		CERROR_TRY_RETURN(lodepng_add_itext(dest,
				source->itext_keys[i], source->itext_langtags[i],
				source->itext_transkeys[i], source->itext_strings[i]));
		i++;
	}
	return (0);
}

void	lodepng_clear_itext(LodePNGInfo *info)
{
	LodePNGIText_cleanup(info);
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

unsigned int	lodepng_add_itext(LodePNGInfo *info, const char *key,
		const char *langtag, const char *transkey, const char *str)
{
	char	**nk;
	char	**nl;
	char	**nt;
	char	**ns;

	nk = (char **)lodepng_realloc(info->itext_keys,
			sizeof(char *) * (info->itext_num + 1));
	nl = (char **)lodepng_realloc(info->itext_langtags,
			sizeof(char *) * (info->itext_num + 1));
	nt = (char **)lodepng_realloc(info->itext_transkeys,
			sizeof(char *) * (info->itext_num + 1));
	ns = (char **)lodepng_realloc(info->itext_strings,
			sizeof(char *) * (info->itext_num + 1));
	if (itext_alloc_fail(nk, nl, nt, ns))
		return (83);
	++info->itext_num;
	info->itext_keys = nk;
	info->itext_langtags = nl;
	info->itext_transkeys = nt;
	info->itext_strings = ns;
	info->itext_keys[info->itext_num - 1] = alloc_string(key);
	info->itext_langtags[info->itext_num - 1] = alloc_string(langtag);
	info->itext_transkeys[info->itext_num - 1] = alloc_string(transkey);
	info->itext_strings[info->itext_num - 1] = alloc_string(str);
	return (0);
}
