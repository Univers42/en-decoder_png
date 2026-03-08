/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:57:05 by marvin            #+#    #+#             */
/*   Updated: 2025/12/29 23:57:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PADDING_H
# define PADDING_H

void	remove_padding_bits(unsigned char *out, const unsigned char *in,
			size_t olinebits, size_t ilinebits, unsigned int h);
void	add_padding_bits(unsigned char *out, const unsigned char *in,
			size_t olinebits, size_t ilinebits, unsigned int h);

#endif
