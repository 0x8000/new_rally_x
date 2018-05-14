#ifndef _IMAGES_H
#define _IMAGES_H

#define _CRT_SECURE_NO_WARNINGS 

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COLOR_PALLETE_BASE_ADDR 0x0000
#define IMAGE_8x8_BASE_ADDR     0x00FF
#define IMAGE_16x16_BASE_ADDR     0x0100

#define MAP_ROW_SIZE 80
#define MAP_COL_SIZE 100
#define MAP_AREA_SIZE (MAP_ROW_SIZE * MAP_COL_SIZE)

#define NUM_MAP_ENTRIES         ( 160 * 30 )

// TODO: Broj sprajtova saznati u programu i dinamicki zauzeti memoriju
#define NUMBER_OF_SPRITES (32 + 1)
#define LINE_SIZE 50

typedef enum {
	IMG_8x8,
	IMG_16x16
} image_type_t;

typedef struct {
	unsigned char   r;
	unsigned char   g;
	unsigned char   b;
} color_t;

typedef struct {
	unsigned char   z;
	unsigned char   rot;
	unsigned short  ptr;
} map_entry_t;

typedef struct {
	char sprite_name[LINE_SIZE];  // file name
	unsigned long address;
	char id;
} Sprite;

extern color_t  color_pallete[256];
extern int      num_colors;

void    colors_to_mem(FILE * f, unsigned long addr);
char *  color_to_string(unsigned char r, unsigned char g, unsigned char b);
void    image_to_mem(FILE * f, unsigned long addr, unsigned char * img, unsigned char type, char * comment);
void    process_images(const char * dir, FILE * mem_file, FILE * def_file, unsigned long * base_addr, unsigned char type);
void    create_test_map();
void    map_to_mem(FILE * mem_file, FILE * def_file, FILE * hdr_file, unsigned long * base_addr);
void create_our_map();
void merge_names_and_ids();

#endif // _IMAGES_H
