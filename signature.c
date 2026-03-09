/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signature.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:02:33 by marvin            #+#    #+#             */
/*   Updated: 2026/03/08 22:23:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void	write_signature(t_ucvector *out)
{
	ucvector_push_back(out, 137);
	ucvector_push_back(out, 80);
	ucvector_push_back(out, 78);
	ucvector_push_back(out, 71);
	ucvector_push_back(out, 13);
	ucvector_push_back(out, 10);
	ucvector_push_back(out, 26);
	ucvector_push_back(out, 10);
}
