//
//  im_comp.h
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#ifndef __im_comp__im_comp__
#define __im_comp__im_comp__

#include <stdio.h>

struct Run3D {
    unsigned char run;
    short level;
    char last;
};


void quantize_block ( short *coef );
//inverse quantize one block
void inverse_quantize_block ( short *coef );


//input : Y,  output : Yr
void reorder ( short *Y, short *Yr );
//input : Yr, output : Y
void reverse_reorder ( short *Yr, short *Y );


void run_block ( short *Y, struct Run3D runs[] );
void run_decode (struct Run3D runs[], short *Y );


#endif /* defined(__im_comp__im_comp__) */
