//
// Created by Benjamin DROUARD on 29/06/2018.
//

#ifndef PGSENCODER_YUVRGBTOOLS_H
#define PGSENCODER_YUVRGBTOOLS_H

#define MAX(A,B) ((A)>(B)?(A):(B))
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * (R) + 38470 * (G) + 7471 * (B) ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * ((B) - CLIP((19595 * (R) + 38470 * (G) + 7471 * (B) ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * ((R) - CLIP((19595 * (R) + 38470 * (G) + 7471 * (B) ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( (Y) + ( 91881 * (Cr) >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( (Y) - (( 22544 * (Cb) + 46793 * (Cr) ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( (Y) + (116129 * (Cb) >> 16 ) - 226 )

#define getR(X) ((((X) >> 24) & 0xFF))
#define getG(X) ((((X) >> 16) & 0xFF))
#define getB(X) ((((X) >> 8) & 0xFF))
#define getA(X) (((X) & 0xFF))

#define getRGBA(R, G, B, A) (((R) << 24) | ((G) << 16) | ((B) << 8) | (A))

#define RGBADistance(C1, C2) (MAX((((long)getR(C1) - (long)getR(C2)) ^ 2), (((long)getR(C1) - (long)getR(C2) - ((long)getA(C1) - (long)getA(C2))) ^ 2)) + \
                              MAX((((long)getG(C1) - (long)getG(C2)) ^ 2), (((long)getG(C1) - (long)getG(C2) - ((long)getA(C1) - (long)getA(C2))) ^ 2)) + \
                              MAX((((long)getB(C1) - (long)getB(C2)) ^ 2), (((long)getB(C1) - (long)getB(C2) - ((long)getA(C1) - (long)getA(C2))) ^ 2)))

#endif //PGSENCODER_YUVRGBTOOLS_H
