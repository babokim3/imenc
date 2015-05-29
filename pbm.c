//
//  pbm.c
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/16/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "pbm.h"



unsigned char *readImage(const char *file_name, unsigned long *length, struct image_header *ih){
    FILE *pgmFile;
    int i, j;
    int pixel;

    unsigned char *image_data;
    unsigned char *pix;
    
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        fprintf(stderr, "Cannot open file to read!\n");
        fclose(pgmFile);
        return NULL;
    }
    
    fgets(ih->format, sizeof(ih->format), pgmFile);
    if (strcmp(ih->format, "P5") != 0 && strcmp(ih->format, "P6") != 0) {
        fprintf(stderr, "Wrong file type!\n");
        fclose(pgmFile);
        return NULL;
    }
    //read header
    fscanf(pgmFile, "%d", &ih->cols);
    fscanf(pgmFile, "%d", &ih->rows);
    fscanf(pgmFile, "%d", &ih->levels);
    fgetc(pgmFile);
    

    if (strcmp(ih->format, "P5") == 0){
        image_data = (unsigned char *)malloc(ih->rows * ih->cols);
    }else{
        image_data = (unsigned char *)malloc(3 * ih->rows * ih->cols);
    }
    
    
    if (strcmp(ih->format, "P5") == 0){
        for (j = 0; j < ih->rows; ++j)
            for (i = 0; i < ih->cols; ++i) {
                pixel = (char)fgetc(pgmFile);
                image_data[j*ih->cols + i] = pixel;
            }
    }else{
        for (j = 0; j < ih->rows; ++j)
            for (i = 0; i < ih->cols; ++i) {
                pix = &image_data[j*(ih->cols * 3) + i * 3];
                pix[0] = (char)fgetc(pgmFile);
                pix[1] = (char)fgetc(pgmFile);
                pix[2] = (char)fgetc(pgmFile);
            }
    }
    
    
    if (strcmp(ih->format, "P5") == 0){
        *length = ih->rows * ih->cols;
    }else{
        *length = 3 * ih->rows * ih->cols;
    }
    fclose(pgmFile);
    return image_data;
}
//---------------------------------------------------------------------------------------------
void writeImage(const char *filename, const unsigned char *image_data, const struct image_header ih){
    FILE *pgmFile;
    int i, j;
    const unsigned char *pix;
    
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
    
    fprintf(pgmFile, "%s ", ih.format);
    fprintf(pgmFile, "%d %d ", ih.cols, ih.rows);
    fprintf(pgmFile, "%d ", ih.levels);
    
    if(strcmp(ih.format, "P5") == 0){
        for (j = 0; j < ih.rows; ++j)
            for (i = 0; i < ih.cols; ++i) {
                pix = &image_data[j*ih.cols + i];
                fputc(pix[0], pgmFile);
            }
    }else{
        for (j = 0; j < ih.rows; ++j)
            for (i = 0; i < ih.cols; ++i) {
                pix = &image_data[j * ih.cols * 3 + i * 3];
                fputc(pix[0], pgmFile);
                fputc(pix[1], pgmFile);
                fputc(pix[2], pgmFile);
            }
    }
    
    fclose(pgmFile);
}