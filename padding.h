/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:05 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 00:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PADDING_H
# define PADDING_H

void	remove_padding_bits(unsigned char *out,
			const unsigned char *in,
			const size_t *linebits, unsigned int h);
void	add_padding_bits(unsigned char *out,
			const unsigned char *in,
			const size_t *linebits, unsigned int h);

#endif
