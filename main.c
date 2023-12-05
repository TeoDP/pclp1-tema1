#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"

int main() {

    char *output_file = "./output.bmp";

    char image_path[100];
    int N, M;
    //int x, y, h, w;
    //int rows, cols, new_R, new_G, new_B;
    int filter_size;
    float **filter;

    scanf("%s %d %d", image_path, &N, &M);

    // Allocate memory for the image
    int ***image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    // Read the image
    read_from_bmp(image, N, M, image_path);


    scanf("%d", &filter_size);
    filter = (float **)malloc(filter_size * sizeof(float *));
    for (int i = 0; i < filter_size; i++) {
        filter[i] = (float *)malloc(filter_size * sizeof(float));
        for (int j = 0; j < filter_size; j++) {
            scanf("%f", &filter[i][j]);
        }
    }
    image = apply_filter(image, N, M, filter, filter_size);

    write_to_bmp(image, N, M, output_file);

    // Free memory
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return 0;
}


/*

int main() {

    int N = 789, M = 1024;
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    printf("starting reading file\n");
    read_from_bmp(m, N, M, "./4964064451_25410b9741_b.bmp");
    //read_from_bmp(m, N, M, "./test.bmp");
    printf("file read from\n");
    //int ***flip_h = flip_horizontal(m, N, M);
    //int ***test_m = rotate_left(m, N, M);
    int ***test_m = crop(m, N, M, 100, 100, 300, 400);
    printf("matrice test creata\n");

    write_to_bmp(test_m, 300, 400, "./output.bmp");
    //write_to_bmp(flip_h, N, M, "./output.bmp");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(m[i][j]);
        }
		free(m[i]);
    }
	free(m);

    printf("Done\n");
}

*/
