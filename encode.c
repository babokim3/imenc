//
//  main.c
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include <stdio.h>
#include "pbm.h"
#include "im_comp.h"
#include "dct.h"

int main(int argc, const char * argv[]) {

	unsigned char *data;
	struct image_header ih;
	unsigned long data_size;
	FILE *fp;
	short F[8][8];

	if (argc != 2) {
		printf("Not enough arguments, the program call should as follows: %s <input.pbm>", argv[0]);
		return 0;
	}

	data = readImage(argv[1], &data_size, &ih);

	short row, col, i, j, r;
	unsigned int cur_bnum = 0;
	unsigned char *p;
	unsigned num_blocks = ih.cols * ih.rows / 64;
	unsigned short blocks8x8[num_blocks][64];
	short dctBlocks8x8[num_blocks][64];
	short rDctBlocks8x8[num_blocks][64];
	struct Run3D runs[num_blocks][64];

	for ( row = 0; row < ih.rows; row += 8 ) {
		for ( col = 0; col < ih.cols; col += 8 ) {
			//points to beginning of a 8x8 block
			p = data + (row * ih.cols + col);
			r = 0; //note pointer arithmetic
			for ( i = 0; i < 8; ++i ) {
				for ( j = 0; j < 8; ++j ){
					blocks8x8[cur_bnum][r++] = *p;
					p++;
				}
				p += (ih.cols-8); //points to next row within macroblock
			}
			r = 0;
			cur_bnum++;
		} //for col
	} //for row

	for(cur_bnum = 0; cur_bnum < num_blocks; cur_bnum++)
		dct ( 8, blocks8x8[cur_bnum], dctBlocks8x8[cur_bnum] );

	print_elements ( 8,  dctBlocks8x8[10] );

	quantize_block ( dctBlocks8x8[10] );
	printf("\nQuantized DCT:\n");
	print_elements ( 8,  dctBlocks8x8[10] );

	for (i = 0; i < num_blocks ;i++)
		reorder ( dctBlocks8x8[i], rDctBlocks8x8[i] );

	printf("\n");
	for(i = 0; i<64; i++)
		printf("%d ",rDctBlocks8x8[10][i]);

	printf("\n");
	for(i = 0; i<num_blocks; i++)
		run_block(rDctBlocks8x8[i], &runs[i]);

	printf("\n");
	i = -1;
	do{
		i++;
		printf("%d,%d,%d\n", runs[10][i].run, runs[10][i].level, runs[10][i].last);
	}while(runs[10][i].last == 0);


//	inverse_quantize_block(dctBlocks8x8[10]);
//	printf("\nInverse quantized DCT:\n");
//	print_elements ( 8,  dctBlocks8x8[10] );

//    * Split into 8 x 8 blocks and apply DCT to every block
//    * Quantize DCT coefficients
//    * Apply zigzag reordering
//    * Apply run-level encoding and store the codes in `Run3D  runs[64];`
//    * Print them on the screen, while running the program redirect standard stream to a file i.e.
//        __./encode image.pbm > run3d.code__
    return 0;
}
