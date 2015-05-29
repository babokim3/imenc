#include "dct.h"



//input: f, N; output: F
short dct_direct( short N, double *f, double *F ){
    double a[32], sum, coef;
    short i, j, u, v;
    if ( N > 32 || N <= 0 ) {
        printf ("\ninappropriate N\n");
        return -1;
    }
    a[0] = sqrt ( 1.0 / N );
    for ( i = 1; i < N; ++i ) {
        a[i] = sqrt ( 2.0 / N );
    }
    for ( u = 0; u < N; ++u ) {
        for ( v = 0; v < N; ++v ) {
            sum = 0.0;
            for ( i = 0; i < N; ++i ) {
                for ( j = 0; j < N; ++j ) {
                    coef = cos( (2*i+1) * u * PI/(2*N) ) * cos( (2*j+1) * v * PI/(2*N) );
                    sum += *(f+i*N+j) * coef;
                } //for j
                *(F+u*N+v) = a[u] * a[v] * sum;
            } //for i
        } //for u
    } //for v
    return 1; }

//f[i][j] * coef
//input: N, F; output f
short idct_direct( short N, double *F, double *f ){
    double a[32], sum, coef;
    short i, j, u, v;
    if ( N > 32 || N <= 0 ) {
        printf ("\ninappropriate N\n");
        return -1;
    }
    a[0] = sqrt ( 1.0 / N );
    for ( i = 1; i < N; ++i )
        a[i] = sqrt ( 2.0 / N );

    for ( i = 0; i < N; ++i ) {
        for ( j = 0; j < N; ++j ) {
            sum = 0.0;
            for ( u = 0; u < N; ++u ) {
                for ( v = 0; v < N; ++v ) {
                    coef =  cos( (2*j+1) * v * PI/(2*N) )  *   cos( (2*i+1) * u * PI/(2*N) );
                    sum += a[u]*a[v]*(*(F+u*N+v))*coef;//a[u]*a[v]*F[u][v]*coef
                } //for j
                *(f+i*N+j) =  sum;
            } //for i
        } //for u
    } //for v
    return 1; }

// change values from short to double and vice versa.
short dct ( short N, short *f, short *F ){
    double  tempx[1024], tempy[1024];
    int  total, i;
    if ( N > 32 || N <= 0 ) {
        printf ("\ninappropriate N\n");
        return -1;
    }
    total = N * N;
    for ( i = 0; i < total; ++i )
        tempx[i] = (double) *(f+i);

    dct_direct ( N, tempx, tempy );
    for ( i = 0; i < total; ++i )
        //DCT operation
        *(F+i) = (short ) ( floor (tempy[i]+0.5) ); //rounding
    return 1;}

// change values from short to doulbe, and vice versa.
short idct ( short N, short *F, short *f ){
    double  tempx[1024], tempy[1024];
    int  total, i;
    if ( N > 32 || N <= 0 ) {
        printf ("\ninappropriate N\n");
        return -1;
    }
    total = N * N;
    for ( i = 0; i < total; ++i )
        tempy[i] = (double) *(F+i);

    idct_direct ( N, tempy, tempx );
    for ( i = 0; i < total; ++i )
        //IDCT operation
        *(f+i) = (short ) ( floor (tempx[i]+0.5) ); //rounding
    return 1; }

void print_elements ( short N,  short *f ){
    short i, j;
    for ( i = 0; i < N; ++i ){
        printf("\n");
        for ( j = 0; j < N; ++j )
            printf ("%4d, ", *(f+N*i+j) );
    }
}
