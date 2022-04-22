#include <math.h>
#include <stdlib.h>
#include "PIC.H"


void PicFilterRoberts(PIC *PDest, PIC *PSrc, double Alpha)
{
  int x, y, c, A, B, C, D;

   for (y = 0; y < PSrc->H; y++)
    for (x = 0; x < PSrc->W; x++)
      for (c = 0; c < 3; c++)
      {
        A = PicGet(PSrc, x, y, c);
        B = PicGet(PSrc, x + 1, y, c);
        C = PicGet(PSrc, x, y + 1, c);
        D = PicGet(PSrc, x + 1, y + 1, c);
        PDest->Pixels[y * PDest->W + x][c] = Clamp(Alpha * (double)(abs(A - D) + abs(B - C)), 0, 255);
      }
}

void PicFilterSobel( PIC *PDest, PIC *PSrc, double Alpha )
{
  int x, y, c, A, B, C, D, E, F, G, H, f, g;

  for (y = 0; y < PSrc->H; y++)
   for (x = 0; x < PSrc->W; x++)
     for (c = 0; c < 3; c++)
     {
       A = PicGet(PSrc, x - 1, y + 1, c);
       B = PicGet(PSrc, x, y + 1, c);
       C = PicGet(PSrc, x + 1, y + 1, c);
       D = PicGet(PSrc, x - 1, y, c);
       E = PicGet(PSrc, x + 1, y, c);
       F = PicGet(PSrc, x - 1, y - 1, c);
       G = PicGet(PSrc, x, y - 1, c);
       H = PicGet(PSrc, x + 1, y - 1, c);
       f = (A + 2 * B + C) - (F + 2 * G + H);
       g = (A + 2 * D + F) - (C + 2 * E + H);
       PDest->Pixels[y * PDest->W + x][c] =  Clamp(Alpha * sqrt(f * f + g * g), 0, 255);
     }
}

void PicFilterMedian( PIC *PDest, PIC *PSrc, int N )
{
  int k, i, j, x, y, c;
  byte *A;

  if ((A = malloc(N * N)) == NULL)
    return;

  for (y = 0; y < PSrc->H; y++)
   for (x = 0; x < PSrc->W; x++)
     for (c = 0; c < 3; c++)
     {
       k = 0;
       for (i = 0; i < N; i++)
         for (j = 0; j < N; j++)
           A[k++] = PicGet(PSrc, x - N / 2 + j, y - N / 2 + i, c);
       QuickSort(A, N * N);
       PDest->Pixels[y * PDest->W + x][c] = A[N * N / 2];
     }
  free(A);
}
