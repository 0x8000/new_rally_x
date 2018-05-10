#include "tools.h"
#include "bitmap.h"

color_t     color_pallete[ 256 ];
map_entry_t map[ NUM_MAP_ENTRIES ];
int         num_colors;

void colors_to_mem( FILE * f, unsigned long addr )
{
    unsigned int i;

    for( i = 0; i < 255; i++ ) {
        fprintf( f, "\t\t%lu =>\tx\"", addr );

        if( i < num_colors ) {
            fprintf( f, "00%.2X%.2X%.2X", color_pallete[ i ].b, color_pallete[ i ].g, color_pallete[ i ].r );
        } else {
            fprintf( f, "00000000" );
        }

        if( i < num_colors ) {
            fprintf( f, "\", -- R: %d G: %d B: %d\n", color_pallete[ i ].r, color_pallete[ i ].g, color_pallete[ i ].b );
        } else {
            fprintf( f, "\", -- Unused\n" );
        }

        addr++;
    }
}

char * color_to_string( unsigned char r, unsigned char g, unsigned char b )
{
    static char     str[ 3 ];
    unsigned int    i;
    unsigned int    mask;
    unsigned char   bit;
    unsigned char   found;

    memset( str, '?', 2 );

    found = 0;

    for( i = 0; i < num_colors; i++ ) {
        if( color_pallete[ i ].r == r && color_pallete[ i ].g == g && color_pallete[ i ].b == b ) {
            sprintf( str, "%.2X", i );
            found = 1;
            break;
        }
    }

    // Color is not in pallete but there's still free space, so add it
    if( !found && num_colors < 256 ) {
        color_pallete[ num_colors ].r = r;
        color_pallete[ num_colors ].g = g;
        color_pallete[ num_colors ].b = b;

        sprintf( str, "%.2X", num_colors );

        num_colors++;
    } else {
        if( !found ) {
            printf( "Cannot add color: %d %d %d, pallete is full!\n", r, g, b );
        }
    }

    str[ 2 ] = '\0';

    return str;
}

void image_to_mem( FILE * f, unsigned long addr, unsigned char * img, unsigned char type, char * comment )
{
    unsigned char n;
    unsigned char i;
    unsigned char k;
    unsigned char pixel;

    n = ( type == IMG_8x8 ) ? 8 : 16;

    img += ( n * n - 1 ) * 3;

    for( i = 0; i < n; i++ ) {
        img -= ( n - 1 ) * 3;

        for( k = 0; k < n / 4; k++ ) {
            fprintf( f, "\t\t%lu =>\tx\"", addr );

            // 4 color pallete indexes
            for( pixel = 0; pixel < 4; pixel++ ) {
                fprintf( f, color_to_string( img[ 2 ], img[ 1 ], img[ 0 ] ) );
                img += 3;
            }

            if( !i && !k ) {
                fprintf( f, "\", -- %s\n", comment );
            } else {
                fprintf( f, "\",\n" );
            }

            addr++;
        }

        img -= ( n + 1 ) * 3;
    }
}

void process_images( const char * dir, FILE * mem_file, FILE * def_file, unsigned long * base_addr, unsigned char type )
{
    char            search_dir[ MAX_PATH ];
    char            file_path[ MAX_PATH ];
    char            def_name[ 128 ];
    unsigned char * img;
    WIN32_FIND_DATA find_data;
    HANDLE          find;

    sprintf( search_dir, ( type == IMG_16x16 ) ? "%s\\16x16\\*.bmp" : "%s\\8x8\\*.bmp", dir );

    if( !( find = FindFirstFile( search_dir, &find_data ) ) ) {
        printf( "FindFirstFile failed.\n" );
        return;
    }

    do {
        if( !( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
            sprintf( file_path, ( type == IMG_16x16 ) ? "%s\\16x16\\%s" : "%s\\8x8\\%s", dir, find_data.cFileName );

            if( !( img = load_bitmap( file_path ) ) ) {
                printf( "Failed to open: %s\n", file_path );
                continue;
            }

            sprintf( def_name, ( type == IMG_16x16 ) ? "IMG_16x16_%s" : "IMG_8x8_%s", find_data.cFileName );

            // Remove .bmp extension
            def_name[ strlen( def_name ) - 4 ] = '\0';

            fprintf( def_file, "#define %s\t\t\t0x%.4X\n", def_name, *base_addr );

            image_to_mem( mem_file, *base_addr, img, type, def_name );

            // Each image row gets split into 4 byte parts in order to fit memory size.
            *base_addr += ( type == IMG_16x16 ) ? (16 * 4) : (8 * 2);

            free( img );
        }
    } while( FindNextFile( find, &find_data ) );

    FindClose( find );
}

// Using 0-9, a-z and A-Z 
void create_our_map() {
	FILE *f;

	if (!(f = fopen("bin\\empty_rally_map.map", "w"))) {
		printf("Couldn't open 'empty_rally_map.map' file!\n");
		return;
	}

	for (int i = 0; i < MAP_ROW_SIZE; i++) {
		for (int j = 0; j < MAP_COL_SIZE - 1; j++) {
			fprintf(f, "0");
		}
		fprintf(f, "0\n");
	}

	fclose(f);
}

void create_test_map( )
{
    unsigned int    x;
    unsigned int    y;
    char            tmp;
    FILE *          f;

    if( !( f = fopen( "bin\\mapa.map", "r" ) ) ) {
        printf( "Couldn't open 'mapa.map' file!\n" );
        return;
    }

    x = 0;

    while( ( tmp = fgetc( f ) ) != EOF ) {
        if( tmp >= '0' && tmp <= '9' ) {
            map[ x ].rot = 0;
			
            if( tmp == '0' ) {
                map[ x ].z = 0;
                map[ x++ ].ptr = 0x017F; // crno
            } else if( tmp == '1' ) {
                map[ x ].z = 1;
				map[ x++ ].ptr = 0x01FF; // mario
            } else if( tmp == '2' ) {
                map[ x ].z = 2; 
				map[ x++ ].ptr = 0x00FF; // cigla
            } else if( tmp == '3' ) {
                map[ x ].z = 3;
				map[ x++ ].ptr = 0x023F; // plava cigla
            } else if( tmp == '4' ) {
                map[ x ].z = 4; 
				map[ x++ ].ptr = 0x01BF; // enemi
            } else if( tmp == '5' ) {
                map[ x ].z = 5;
				map[ x++ ].ptr = 0x013F; // coin
			}else{
				 map[ x ].z = 0;
				 map[ x++ ].ptr = 0x0000; // null
			}
        }
    }

    fclose( f );
}

void map_to_mem( FILE * mem_file, FILE * def_file, FILE * hdr_file, unsigned long * base_addr )
{
    unsigned int i, j = 0;

    fprintf( def_file, "#define MAP_BASE_ADDRESS\t\t\t0x%.4X", *base_addr );
    for( i = 0; i < MAP_AREA_SIZE; i++ ) {

		//matrica[30][160]

		fprintf( hdr_file, "#ifndef _MAP_H_\n", *base_addr );

        fprintf( hdr_file, "unsigned char  map1[30][160] = {\n" );

		for( i = 0; i < MAP_AREA_SIZE; i++ ) {
			//fprintf( mem_file, "%d\n", map[ i ].z );
			fprintf( mem_file, "\t\t%lu =>\tx\"%.2X%.2X%.4X\", -- z: %d rot: %d ptr: %d\n", *base_addr,
                                                                                         map[ i ].z,
                                                                                         map[ i ].rot,
                                                                                         map[ i ].ptr,
                                                                                         map[ i ].z,
                                                                                         map[ i ].rot,
                                                                                         map[ i ].ptr );
			if(j == 0)
			{
				fprintf( hdr_file, "{ " );
			}
			fprintf( hdr_file, ( i == MAP_AREA_SIZE - 1 ) ? "%d, "
															: "%d, ", map[ i ].z);
			if(j >= 159)
				fprintf( hdr_file, "\n{ " );
			}
			fprintf( hdr_file, ( i == MAP_AREA_SIZE - 1 ) ? "%d, "
															: "%d, ", map[ i ].z);
			if(j >= 79)
			{
				fprintf( hdr_file, " },\n");
				j = 0;
			}
			else
			{
				j++;
			}

			*base_addr += 1;
		}

		fprintf( hdr_file, "\n};\n" );
}
