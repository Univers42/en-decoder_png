/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/08 18:22:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	string_cleanup(char **out)
{
	if (out && *out)
	{
		free(*out);
		*out = NULL;
	}
}

char	*alloc_string(const char *in)
{
	size_t	len;
	char	*s;

	if (!in)
		return (NULL);
	len = strlen(in) + 1;
	s = (char *)malloc(len);
	if (!s)
		return (NULL);
	memcpy(s, in, len);
	return (s);
}
