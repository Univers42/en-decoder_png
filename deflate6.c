/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deflate6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:24:44 by marvin            #+#    #+#             */
/*   Updated: 2026/03/09 02:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
** RFC 1951 fixed Huffman literal/length tree (288 symbols):
**   0-143   : 8-bit codes
**   144-255 : 9-bit codes
**   256-279 : 7-bit codes
**   280-287 : 8-bit codes
*/
void	gen_fixed_litlen_tree(t_huffman_tree *tree)
{
	unsigned int	i;
	unsigned int	bl[288];

	i = 0;
	while (i <= 143)
		bl[i++] = 8;
	while (i <= 255)
		bl[i++] = 9;
	while (i <= 279)
		bl[i++] = 7;
	while (i <= 287)
		bl[i++] = 8;
	huffman_tree_make_from_len(tree, bl, 288, 15);
}

void	gen_fixed_dist_tree(t_huffman_tree *tree)
{
	unsigned int	i;
	unsigned int	bl[30];

	i = 0;
	while (i < 30)
		bl[i++] = 5;
	huffman_tree_make_from_len(tree, bl, 30, 15);
}

void	get_tree_inflate_fixed(t_huffman_tree *tree_ll, t_huffman_tree *tree_d)
{
	gen_fixed_litlen_tree(tree_ll);
	gen_fixed_dist_tree(tree_d);
}

unsigned int	dd_emit(t_deflate_work *w, t_dd_ctx *ctx,
		unsigned int bfinal)
{
	dd_write_header(w->out, w->bp, ctx, bfinal);
	dd_write_cls(w->out, w->bp, ctx);
	return (dd_write_data(w, ctx));
}
