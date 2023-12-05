#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"
/*
// o functie care sa aloce dinamic o matrice pt o imagine
void image_alloc(int *matrice, int N, int M) {
    int ***matrice = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        matrice[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            matrice[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
}
            int ***image = (int ***)malloc(N * sizeof(int **));
            for (int i = 0; i < N; i++) {
                image[i] = (int **)malloc(M * sizeof(int *));
                for (int j = 0; j < M; j++) {
                    image[i][j] = (int *)malloc(3 * sizeof(int));
                }
            }
*/

int main() {
    const int word_size = 100;
    char path[word_size], command[3];
    int N = 0, M = 0;
    int x = 0, y = 0, w = 0, h = 0;
    int index_image = 0;
    int index_filter = 0;
    int rows = 0, cols = 0;
    int size = 0;
    int isON = 1;

    // scanning for the command the user has chosen
    // and then running the adequate code for said command

    int ****image_list = (int ****)malloc(sizeof(int ***));
    int *N_list = (int *)calloc(sizeof(int));
    int *M_list = (int *)calloc(sizeof(int));
    while (isON) {
    printf("insert command: ");
    scanf("%s", command);
    switch (command[0]) {
        case 'e':
            // exit
            printf("exiting\n");
            isON = 0;
            break;

        case 'l':
            // load
            // reading command arguments
            printf("insert N M and path for loading\n");
            scanf("%d%d%s", &N, &M, path);
            image_list = (int ****)realloc(image_list, (index_image+1) * sizeof(int ****));
            for (int ii = 0; ii < index_image+1; ii++) {
                image_list[ii] = (int ***)realloc(image_list[ii], N * sizeof(int **));
                for (int i = 0; i < N; i++) {
                    image_list[ii][i] = (int **)realloc(image_list[ii][i], M * sizeof(int *));
                    for (int j = 0; j < M; j++) {
                        image_list[ii][i][j] = (int *)realloc(image_list[ii][i][j], 3 * sizeof(int));
                    }
                }
            }
            read_from_bmp(image_list[index_image], N, M, path);

            index_image++;
            break;

        case 's':
            // save
            // reading command arguments
            printf("insert index and path for saving");
            scanf("%d%s", &index_image, path);
            write_to_bmp(image_list[index_image], N, M, path);
            break;

        // because the commands are two letter long and C does not seem to
        // allow usage of switch case with strings
        // i've decided that a sufficient workaround would be to check for the first
        // letter of the command where necessary and then do switch case for the second
        // letter aswell
        // is it a convoluted way to do so? yes. somewhat unreadable? a bit.
        // does it work? i hope so
        case 'a':
            // char command2[] = {command[1]};
            switch (command[1]) {
                case 'h':
                    // Apply Horizontal Flip
                    // reading command arguments
                    scanf("%d" index);
                break;

                case 'r':
                    // Apply Rotate
                    break;

                case 'c':
                    // Apply Crop
                    break;

                case 'e':
                    // Apply Extend
                    break;

                case 'p':
                    // Apply Paste
                    break;

                case 'f':
                    // Apply Filter
                    break;

                default:
                    printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
            }
            break;

        case 'c':
            // Create Filter
            break;

        // same workaround as for case "a"
        case 'd':

            switch (command[1]) {
                case 'f':
                    // Delete Filter
                    break;

                case 'i':
                    // Delete Image
                    break;

                default:
                    printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
            }
            break;

        default:
            printf("~~~ERROR!~~~\nThe command you have entered does not exist.\n~~~~~~~~~\n");
    }
    }

    return 0;
}
