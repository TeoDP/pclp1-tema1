#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"

void magicfree(int ***m, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(m[i][j]);
        }
        free(m[i]);
    }
    free(m);
}

int main() {
    const int word_size = 100;
    char path[word_size], command[3];
    int N = 0, M = 0;
    int x = 0, y = 0, w = 0, h = 0;
    int index_image = 0;    // total number of images
    int index_filter = 0;   // total number of filters
    int index = 0;          // temporary value
    int index_sec = 0;      // temporary value
    int rows = 0, cols = 0, R = 0, G = 0, B = 0;
    int size = 0;
    int isON = 1;

    int ****image_list = (int ****)malloc(sizeof(int ***));
    int *N_list = (int *)malloc(sizeof(int));
    int *M_list = (int *)malloc(sizeof(int));
    float ***filter_list = NULL;
    int *filter_size = NULL;

    while (isON) {
    // scanning for the command the user has chosen
    // and then running the adequate code for said command
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
            // dynamically adding one more index to the first layer of the 4d array
            // and then creating a 3d array on that index
            image_list = (int ****)realloc(image_list, (index_image+1) * sizeof(int ***));
                image_list[index_image] = (int ***)malloc(N * sizeof(int **));
                for (int i = 0; i < N; i++) {
                    image_list[index_image][i] = (int **)malloc(M * sizeof(int *));
                    for (int j = 0; j < M; j++) {
                        image_list[index_image][i][j] = (int *)malloc(3 * sizeof(int));
                    }
                }
            // using 2 more arrays to store the size of the images
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
            scanf("%d%s", &index, path);
            write_to_bmp(image_list[index], N_list[index], M_list[index], path);
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
            // checking whether a filter already exists
            // and allocating memory apropriately
            if (index_filter == 0) {
                filter_list = NULL;
                filter_list = (float ***)malloc(sizeof(float **));
                filter_size = (int *)malloc(sizeof(float));
            } else {
            filter_list = (float ***)realloc(filter_list, (index_filter + 1) * sizeof(float **));
            }
            filter_list[index_filter] = (float **)malloc(size * sizeof(float *));
            for (int i = 0; i < size; i++) {
                filter_list[index_filter][i] = (float *)malloc(size * sizeof(float));
                for (int j = 0; j < size; j++) {
                    scanf("%f", &filter_list[index_filter][i][j]);
                }
            filter_size = (int *)realloc(filter_size, (index_filter + 1) * sizeof(int));
            }
            filter_size[index_filter] = size;
            index_filter++;
            break;

        // same workaround as for case "a"
        case 'd':

            switch (command[1]) {
                case 'f':
                    // Delete Filter
                    scanf("%d", &index);
                    // checking whether there is more than one filter
                    // and deallocating memory apropriately
                    if (index_filter == 1) {
                        for (int i = 0; i < filter_size[0]; i++) {
                            free(filter_list[0][i]);
                        }
                        free(filter_list[0]);
                        index_filter--;
                    } else {
                        for (int i = 0; i < filter_size[index]; i++) {
                            free(filter_list[index][i]);
                        }
                        free(filter_list[index]);
                        for (int i = index; i < (index_filter-1); i++) {
                            filter_list[i] = filter_list[i+1];
                            filter_size[i] = filter_size[i+1];
                        }
                        index_filter--;
                    }
                    break;

                case 'i':
                    // Delete Image
                    scanf("%d", &index);
                    if (index_image == 1) {
                        for (int i = 0; i < N_list[0]; i++) {
                            for (int j = 0; j < M_list[0]; j++) {
                                free(image_list[0][i][j]);
                            }
                            free(image_list[0][i]);
                        }
                        free(image_list[0]);
                        index_image--;
                    } else {
                        magicfree(image_list[index], N_list[index], M_list[index]);
                        for (int i = index; i < (index_image-1); i++) {
                            image_list[i] = image_list[i+1];
                            N_list[i] = N_list[i+1];
                            M_list[i] = M_list[i+1];
                        }
                        index_image--;
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
    // deallocating all the memory left
    for (int ii = 0; ii < index_image; ii++) {
        magicfree(image_list[ii], N_list[ii], M_list[ii]);
    }
    free(image_list);
    free(N_list);
    free(M_list);

    // ----------
    for (int i = 0; i < index_filter; i ++) {
        for (int j = 0; j < filter_size[i]; j++) {
            free(filter_list[i][j]);
        }
        free(filter_list[i]);
    }
    free(filter_list);
    free(filter_size);
    return 0;
}
