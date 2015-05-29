//
//  im_comp.c
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "im_comp.h"

int zigzag[] = {
    0, 1, 8,16, 9, 2, 3,10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

int Qstep = 12;

void quantize_block ( short *p_coef ){
	int i,j;
    for ( i = 0; i < 8; i++ )
        for ( j = 0; j < 8; j++ )
        	p_coef[i * 8 + j] = ( short ) round ( (double)p_coef[i * 8 + j] / Qstep );
}
//inverse quantize one block
void inverse_quantize_block ( short *p_coef ){
	int i,j;
    for ( i = 0; i < 8; i++ )
        for ( j = 0; j < 8; j++ )
        	p_coef[i * 8 + j] = (short) (  p_coef[i * 8 + j] * Qstep );
}

//input : Y,  output : Yr
void reorder ( short *Y, short *Yr ){
    int k, i, j, i1, j1;
    k = 0;
    for ( i = 0; i < 8; i++ ){
        for ( j = 0; j < 8; j++ ){
            i1 =  zigzag[k] / 8;
            j1 = zigzag[k] % 8;
            Yr[i * 8 + j] = Y[i1 * 8 + j1];
            k++;
        }
    }
}

//input : Yr, output : Y
void reverse_reorder ( short *Yr, short *Y ){
    int k, i, j, i1, j1;
    k = 0;
    for ( i = 0; i < 8; i++ ){
        for ( j = 0; j < 8; j++ ){
            i1 = zigzag[k] / 8;
            j1 = zigzag[k] % 8;
            Y[i1 * 8 + j1] = Yr[i * 8 + j];
            k++; }
    } }




void run_block ( short *Y, struct Run3D runs[] ){
    unsigned char run_length = 0, k = 0;
    int i,j;
    for ( i = 0; i < 8; i++ ) {
        for ( j = 0; j < 8; j++ ) {
            if(Y[i * 8 + j]==0 ){
                run_length++;
                continue; }
            runs[k].run = run_length;
            runs[k].level = Y[i * 8 + j];
            runs[k].last = 0;
            run_length = 0;
            k++;
        }
    } if ( k > 0 )
        runs[k-1].last = 1;
    else {
        runs[0].run = 64;
        runs[0].level = 0;
        runs[0].last = 1;
    }
}

void run_decode (struct Run3D runs[], short *Y ){
    int i, j, r, k = 0, n = 0;
    while ( n < 64 ) {
        for(r = 0; r < runs[k].run; r++){
            i = n / 8;
            j = n % 8;
            Y[i * 8 + j] = 0;
            n++;
        }
        if ( n < 64 ){
            i = n / 8;
            j = n % 8;
            Y[i * 8 + j] = runs[k].level;
            n++;
        }
        
        if ( runs[k].last != 0 ) break;
        k++;
    }
    //run of 0s to end
    while ( n < 64 ) {
        i = n / 8;
        j = n % 8;
        Y[i * 8 + j] = 0;
        n++;
    }
}
