#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"

int main() {
    const int word_size = 100;
    char path[word_size], command[3];
    int N = 0, M = 0;
    int x = 0, y = 0, w = 0, h = 0;
    int index_image = 0;
    int index_filter = 0;
    int index = 0;
    int index_sec = 0;
    int rows = 0, cols = 0, R = 0, G = 0, B = 0;
    int size = 0;
    int isON = 1;

    // scanning for the command the user has chosen
    // and then running the adequate code for said command

    int ****image_list = (int ****)malloc(sizeof(int ***));
    int *N_list = (int *)malloc(sizeof(int));
    int *M_list = (int *)malloc(sizeof(int));
    float ***filter_list = (float ***)malloc(sizeof(float**));
    int *filter_size = (int *)malloc(sizeof(int));
    while (isON) {
    // printf("insert command: ");
    scanf("%s", command);
    switch (command[0]) {
        case 'e':
            // exit
            // printf("exiting\n");
            isON = 0;
            break;

        case 'l':
            // load
            // reading command arguments
            scanf("%d%d%s", &N, &M, path);
            image_list = (int ****)realloc(image_list, (index_image+1) * sizeof(int ***));
                image_list[index_image] = (int ***)malloc(N * sizeof(int **));
                for (int i = 0; i < N; i++) {
                    image_list[index_image][i] = (int **)malloc(M * sizeof(int *));
                    for (int j = 0; j < M; j++) {
                        image_list[index_image][i][j] = (int *)malloc(3 * sizeof(int));
                    }
                }
            N_list = (int *)realloc(N_list, (index_image+1) * sizeof(int));
            M_list = (int *)realloc(M_list, (index_image+1) * sizeof(int));
            read_from_bmp(image_list[index_image], N, M, path);
            N_list[index_image] = N;
            M_list[index_image] = M;
            index_image++;
            break;

        case 's':
            // save
            // reading command arguments
            scanf("%d%s", &index_image, path);
            write_to_bmp(image_list[index_image], N_list[index_image], M_list[index_image], path);
            break;

        // because the commands are two letter long and C does not seem to
        // allow usage of switch case with strings
        // i've decided that a sufficient workaround would be to check for the first
        // letter of the command where necessary and then do switch case for the second
        // letter aswell
        // is it a convoluted way to do so? yes. somewhat unreadable? a bit.
        // does it work? i hope so
        case 'a':
            switch (command[1]) {
                case 'h':
                    // Apply Horizontal Flip
                    // reading command arguments
                    scanf("%d", &index);
                    image_list[index] = flip_horizontal(image_list[index], N_list[index], M_list[index]);
                    break;

                case 'r':
                    // Apply Rotate
                    scanf("%d", &index);
                    image_list[index] = rotate_left(image_list[index], N_list[index], M_list[index]);
                    N = N_list[index];
                    N_list[index] = M_list[index];
                    M_list[index] = N;
                    break;

                case 'c':
                    // Apply Crop
                    scanf("%d%d%d%d%d", &index, &x, &y, &w, &h);
                    image_list[index] = crop(image_list[index], N_list[index], M_list[index], x, y, h, w);
                    N_list[index] = h;
                    M_list[index] = w;
                    break;

                case 'e':
                    // Apply Extend
                    scanf("%d%d%d%d%d%d", &index, &rows, &cols, &R, &G, &B);
                    image_list[index] = extend(image_list[index], N_list[index], M_list[index], rows, cols, R, G, B);
                    M_list[index] += 2*cols;
                    N_list[index] += 2*rows;
                    break;

                case 'p':
                    // Apply Paste
                    scanf("%d%d%d%d", &index, &index_sec, &x, &y);
                    image_list[index] = paste(image_list[index], N_list[index], M_list[index],
                                              image_list[index_sec], N_list[index_sec], M_list[index_sec], x, y);
                    break;

                case 'f':
                    // Apply Filter
                    scanf("%d%d", &index, &index_sec);
                    image_list[index] = apply_filter(image_list[index], N_list[index],
                                                     M_list[index], filter_list[index_sec], filter_size[index_sec]);
                    break;

                default: {}
                    // printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
            }
            break;

        case 'c':
            // Create Filter
            scanf("%d", &size);
            filter_list = (float ***)realloc(filter_list, (index_filter + 1) * sizeof(float **));
            filter_list[index_filter] = (float **)malloc(size * sizeof(float *));
            for (int i = 0; i < size; i++) {
                filter_list[index_filter][i] = (float *)malloc(size * sizeof(float));
                for (int j = 0; j < size; j++) {
                    scanf("%f", &filter_list[index_filter][i][j]);
                }
            }
            filter_size = (int *)realloc(filter_size, (index_filter + 1) * sizeof(int));
            filter_size[index_filter] = size;
            index_filter++;
            break;

        // same workaround as for case "a"
        case 'd':

            switch (command[1]) {
                case 'f':
                    // Delete Filter
                    scanf("%d", &index);
                    if (index_filter != 0) {
                    for (int ii = index; ii < index_filter-1; ii++) {
                        for (int i = 0; i < filter_size[ii]; i ++) {
                            for (int j = 0; j < filter_size[ii]; j++) {
                                filter_list[ii][i][j] = filter_list[ii+1][i][j];
                            }
                        }
                    }
                    }
                    for (int i = 0; i < filter_size[index_filter]; i++) {
                        free(filter_list[index_filter][i]);
                    }
                    if (index_filter != 0) {
                        for (int i = index; i < index_filter-1; i++) {
                            filter_size[i] = filter_size[i+1];
                        }
                    }
                    index_filter--;
                    filter_size = (int *)realloc(filter_size, (index_filter + 1) * sizeof(int));
                    break;

                case 'i':
                    // Delete Image
                    scanf("%d", &index);
                    if (index_image !=0) {
                    for (int ii = index; ii < index_image-1; ii++) {
                        image_list[ii] = (int ***)malloc(N_list[ii+1] * sizeof(int **));
                            for (int i = 0; i < N_list[ii+1]; i++) {
                                image_list[ii][i] = (int **)malloc(M_list[ii+1] * sizeof(int *));
                                for (int j = 0; j < M_list[ii+1]; j++) {
                                    image_list[ii][i][j] = (int *)malloc(3 * sizeof(int));
                                }
                            }
                        for (int i = 0; i < N_list[ii+1]; i ++) {
                            for (int j = 0; j < M_list[ii+1]-1; j++) {
                                image_list[ii][i][j][0] = image_list[ii+1][i][j][0];
                                image_list[ii][i][j][1] = image_list[ii+1][i][j][1];
                                image_list[ii][i][j][2] = image_list[ii+1][i][j][2];
                            }
                        }
                    }
                    }
                    for (int i = 0; i < N_list[index_image]-1; i++) {
                        for (int j = 0; j < M_list[index_image]-1; j++) {
                            free(image_list[index_image][i][j]);
                        }
                        free(image_list[index_image][i]);
                    }
                    free(image_list[index_image]);
                    if (index_image != 0) {
                    for (int i = index; i < index_image-1; i++) {
                        N_list[i] = N_list[i+1];
                        M_list[i] = N_list[i+1];
                    }
                        index_image--;
                        N_list = (int *)realloc(N_list, (index_image+1) * sizeof(int));
                        M_list = (int *)realloc(N_list, (index_image+1) * sizeof(int));
                    } else {
                        free(N_list);
                        free(M_list);
                    }

                    break;

                default: {}
                    // printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
            }
            break;

        default: {}
            // printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
    }
    }

    return 0;
}
