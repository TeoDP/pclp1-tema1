#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"

//   Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    // alocam o matrice
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    // inversam ordinea liniilor matricei prin salvarea
    // pointerilor catre fiecare linie in sens invers
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            m[i][j] = image[i][M-j-1];
        }
    }
    return m;
}

//   Task 2
int ***rotate_left(int ***image, int N, int M) {
    // alocam o matrice
    int ***m = (int ***)malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        m[i] = (int **)malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    // rescriem matricea originala in noua matrice
    // schimband indicii pentru a putea roti
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            m[i][j] = image[j][M-i-1];
        }
    }

    return m;
}

//   Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    // alocam o matrice
    int ***m = (int ***)malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        m[i] = (int **)malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    // scriem in noua matrice doar
    // elementele necesare din matricea originala
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            m[i][j] = image[y+i][x+j];
        }
    }

    return m;
}

//   Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    // salvam dimensiunea noii matrici in 2 variabile
    // pentru a fi mai usor de lucrat cu ele
    // si pentru mai multa vizibilitate
    int new_N = N + 2*rows;
    int new_M = M + 2*cols;
    int ***m = (int ***)malloc(new_N * sizeof(int **));
    for (int i = 0; i < new_N; i++) {
        m[i] = (int **)malloc(new_M * sizeof(int *));
        for (int j = 0; j < new_M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    // trecem prin fiecare coloana, pe cazuri, pentru a
    // completa in exteriorul matricei initiale cu valorile RGB date
    for (int i = 0; i < new_N; i++) {
        // caz 1: marginea de sus
        if (i < rows) {
            for (int j = 0; j < new_M; j++) {
                m[i][j][0] = new_R;
                m[i][j][1] = new_G;
                m[i][j][2] = new_B;
            }
        } else if (i < (N + rows)) {
            for (int j = 0; j < new_M; j++) {
                if (j < cols) {
                    // caz 2: marginea din stanga
                    m[i][j][0] = new_R;
                    m[i][j][1] = new_G;
                    m[i][j][2] = new_B;
                } else if (j < (M + cols)) {
                    // caz 3: corpul imaginii
                    m[i][j] = image[i-rows][j-cols];
                } else {
                    // caz 4: marginea din dreapta
                    m[i][j][0] = new_R;
                    m[i][j][1] = new_G;
                    m[i][j][2] = new_B;
                }
            }
        } else {
            // caz 5: marginea de jos
            for (int j = 0; j < new_M; j++) {
                m[i][j][0] = new_R;
                m[i][j][1] = new_G;
                m[i][j][2] = new_B;
            }
        }
    }

    return m;
}

//   Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    // alocam o matrice
    for (int i = y; i < N_dst; i++) {
        if (i > (N_src + y - 1)) continue;
        for (int j = x; j < M_dst; j++) {
            if (j > (M_src + x -1)) continue;
            image_dst[i][j][0] = image_src[i-y][j-x][0];
            image_dst[i][j][1] = image_src[i-y][j-x][1];
            image_dst[i][j][2] = image_src[i-y][j-x][2];
        }
    }

    return image_dst;
}

//   Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    const int max_color = 255;
    // alocam o matrice
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    // pt a putea lucra cu valori float mai usor, cream un vector
    // in care sa salvam valorile RGB pt pixelul pe care lucram
    float apply[3] = {0, 0, 0};

    // iterare prin pixelii imaginii initiale
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            apply[0] = 0;
            apply[1] = 0;
            apply[2] = 0;
            // iterare prin fiecare celula a matricei de filtru
            for (int ii = 0; ii < filter_size; ii++) {
                for (int jj = 0; jj < filter_size; jj++) {
                    // verificam ca pixelul cu care lucram sa aiba un indice care exista
                    if (i-filter_size/2+ii < 0)
                        continue;
                    if (j-filter_size/2+jj < 0)
                        continue;
                    if (i-filter_size/2+ii >= N)
                        continue;
                    if (j-filter_size/2+jj >= M)
                        continue;

                    // partea care contine de fapt logica de aplicare a filtrului
                    apply[0] += (float)image[i-filter_size/2+ii][j-filter_size/2+jj][0] * filter[ii][jj];
                    apply[1] += (float)image[i-filter_size/2+ii][j-filter_size/2+jj][1] * filter[ii][jj];
                    apply[2] += filter[ii][jj] * (float)image[i-filter_size/2+ii][j-filter_size/2+jj][2];
                }
            }
            // scriem valorile RGB in pixelul imaginii finale
            for (int k = 0; k < 3; k++) {
                if (apply[k] < 0)
                    m[i][j][k] = 0;
                else if (apply[k] > (float)max_color)
                    m[i][j][k] = max_color;
                else
                    m[i][j][k] = (int)apply[k];
            }
        }
    }
    return m;
}
