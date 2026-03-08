/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:15:00 by marvin            #+#    #+#             */
/*   Updated: 2026/01/03 17:43:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdlib.h>

/* PNG color types */
typedef enum e_png_color_type
{
	LCT_GREY = 0,
	LCT_RGB = 2,
	LCT_PALETTE = 3,
	LCT_GREY_ALPHA = 4,
	LCT_RGBA = 6
} t_png_color_type;

/* Color mode structure */
typedef struct s_png_color_mode
{
	t_png_color_type colortype;
	unsigned int bitdepth;
	unsigned char *palette;
	size_t palettesize;
	unsigned int key_defined;
	unsigned int key_r;
	unsigned int key_g;
	unsigned int key_b;
} t_png_color_mode;

/* Color tree node for palette optimization */
typedef struct s_color_tree
{
	struct s_color_tree *children[16];
	int index;
} t_color_tree;

/* Color profile for analyzing image statistics */
typedef struct s_png_color_profile
{
	unsigned int colored;
	unsigned int alpha;
	unsigned int key;
	unsigned int key_r;
	unsigned int key_g;
	unsigned int key_b;
	unsigned int bits;
	size_t numpixels;
	unsigned int numcolors;
	unsigned char palette[256 * 4];
} t_png_color_profile;

/* Time structure for tIME chunk */
typedef struct s_png_time
{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
} t_png_time;

/* Info structure for PNG metadata */
typedef struct s_png_info
{
	unsigned int compression_method;
	unsigned int filter_method;
	unsigned interlace_method;
	t_png_color_mode color;
	unsigned int background_defined;
	unsigned int background_r;
	unsigned int background_g;
	unsigned int background_b;
	size_t text_num;
	char **text_keys;
	char **text_strings;
	size_t itext_num;
	char **itext_keys;
	char **itext_langtags;
	char **itext_transkeys;
	char **itext_strings;
	unsigned int time_defined;
	t_png_time time;
	unsigned int phys_x;
	unsigned int phys_y;
	unsigned int phys_unit;
	unsigned int gama_defined;
	unsigned int gama_gamma;
	unsigned int chrm_defined;
	unsigned int chrm_white_x;
	unsigned int chrm_white_y;
	unsigned int chrm_red_x;
	unsigned int chrm_red_y;
	unsigned int chrm_green_x;
	unsigned int chrm_green_y;
	unsigned int chrm_blue_x;
	unsigned int chrm_blue_y;
	unsigned int srgb_defined;
	unsigned int srgb_intent;
	unsigned int iccp_defined;
	char *iccp_name;
	unsigned char *iccp_profile;
	unsigned int iccp_profile_size;
	unsigned char *unknown_chunks_data[3];
	size_t unknown_chunks_size[3];
} t_png_info;

/* Compress settings */
typedef struct s_compress_settings
{
	unsigned int btype;
	unsigned int use_lz77;
	unsigned int windowsize;
	unsigned int minmatch;
	unsigned int nicematch;
	unsigned int lazymatching;
	// optional custom hooks
	unsigned int (*custom_zlib)(unsigned char **, size_t *, const unsigned char *, size_t, const struct s_compress_settings *);
	unsigned int (*custom_deflate)(unsigned char **, size_t *, const unsigned char *, size_t, const struct s_compress_settings *);
	const void *custom_context;
} t_compress_settings;

typedef struct s_decompress_settings
{
	unsigned int ignore_adler32;
	unsigned int (*custom_zlib)(unsigned char **, size_t *, const unsigned char *, size_t, const struct s_decompress_settings *);
	unsigned int (*custom_inflate)(unsigned char **, size_t *, const unsigned char *, size_t, const struct s_decompress_settings *);
	const void *custom_context;
} t_decompress_settings;

/* Filter strategy enum - DEFINED BEFORE USE */
typedef enum e_lodepng_filter_strategy
{
	LFS_ZERO = 0,
	LFS_MINSUM,
	LFS_ENTROPY,
	LFS_PREDEFINED,
	LFS_BRUTE_FORCE
} t_filter_strategy;

/* Encoder settings */
typedef struct s_encoder_settings
{
	t_compress_settings zlibsettings;
	unsigned int filter_palette_zero;
	t_filter_strategy filter_strategy;
	unsigned int auto_convert;
	unsigned int force_palette;
	unsigned int text_compression;
	unsigned int add_id;
	const unsigned char *predefined_filters;
} t_encoder_settings;

/* Decoder settings */
typedef struct s_decoder_settings
{
	t_decompress_settings zlibsettings;
	unsigned int color_convert;
	unsigned int read_text_chunks;
	unsigned int remember_unknown_chunks;
	unsigned int ignore_crc;
	unsigned int ignore_critical;
	unsigned int ignore_end;
} t_decoder_settings;

/* Main PNG state */
typedef struct s_png_state
{
	t_decoder_settings decoder;
	t_encoder_settings encoder;
	t_png_color_mode info_raw;
	t_png_info info_png;
	unsigned int error;
} t_png_state;

/* Vector types */
// guard vector structs for re-includes
#ifndef UCVECTOR_DEFINED
#define UCVECTOR_DEFINED
typedef struct s_ucvector
{
	unsigned char *data;
	size_t size;
	size_t allocsize;
} ucvector;
#endif

#ifndef UIVECTOR_DEFINED
#define UIVECTOR_DEFINED
typedef struct s_uivector
{
	unsigned int *data;
	size_t size;
	size_t allocsize;
} uivector;
#endif

/* Huffman tree */
#ifndef HUFFMAN_TREE_DEFINED
#define HUFFMAN_TREE_DEFINED
typedef struct s_huffman_tree
{
	unsigned int *tree2d;
	unsigned int *tree1d;
	unsigned int *lengths;
	unsigned int max_bit_len;
	unsigned int numcodes;
} t_huffman_tree;
#endif

/* t_hash table for LZ77 */
#ifndef HASH_STRUCT_DEFINED
#define HASH_STRUCT_DEFINED
typedef struct s_hash
{
	int *head;
	unsigned short *chain;
	int *val;
	int *headz;
	unsigned short *chainz;
	unsigned short *zeros;
} t_hash;
#endif

#ifndef HASH_CONSTANTS_DEFINED
#define HASH_CONSTANTS_DEFINED
#define HASH_NUM_VALUES 65536
#define HASH_BIT_MASK 65535
#define MAX_SUPPORTED_DEFLATE_LENGTH 258
#endif

#endif
