// gcc main.c -o photopobre.exe -W -Wall -Wshadow -pedantic
// teste: cat lena.ppm entrada.txt | ./photopobre.exe > saida.ppm

#include <stdio.h>
#include <math.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
} Image;

int max(int a, int b) {
    if (a > b){
        return a;

    } else {
        return b;
    }
}

int min(int a, int b) {
    if (a < b){
        return a;

    } else {
        return b;
    }
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


// Image escala_de_cinza(Image img) {
//     /*for (unsigned int i = 0; i < img.h; ++i) {
//         for (unsigned int j = 0; j < img.w; ++j) {
//             print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
//         }
//     }*/

//     for (unsigned int i = 0; i < img.h; ++i) {
//         for (unsigned int j = 0; j < img.w; ++j) {
//             int media = img.pixel[i][j][0] +
//                         img.pixel[i][j][1] +
//                         img.pixel[i][j][2];
//             media /= 3;
//             img.pixel[i][j][0] = media;
//             img.pixel[i][j][1] = media;
//             img.pixel[i][j][2] = media;
//         }
//     }

//     return img;
// }

void blur(unsigned int h, unsigned int w, Pixel pixel[512][512], int diametro_blur) {
    //Percorre cada pixel da imagem
    for (unsigned int linha = 0; linha < h; ++linha) {
        for (unsigned int coluna = 0; coluna < w; ++coluna) {
            //Variáveis
            Pixel media = {0, 0, 0};
            int h_anterior = h - 1;
            int w_anterior = w - 1;
            int raio_blur = diametro_blur/2;

            int menor_h = min( h_anterior, (linha + raio_blur) ); //Evita passar da borda de baixo da imagem
            int menor_w = min( w_anterior, (coluna + raio_blur) ); //Evita passar da borda direita da imagem

            //Faz as somm das cores dos pixels no primeiro quadrante do pixel base
            for(int y = max(0, linha - raio_blur); y <= menor_h; ++y) {
                for(int x = max(0, coluna - raio_blur); x <= menor_w; ++x) {
                    media.r += pixel[y][x].r;
                    media.g += pixel[y][x].g;
                    media.b += pixel[y][x].b;
                }
            }

            //Define a média das cores na área de blur
            media.r /= pow(diametro_blur,2);
            media.g /= pow(diametro_blur,2);
            media.b /= pow(diametro_blur,2);

            //Define nova cor do pixel como a média das cores na área do blur
            pixel[linha][coluna].r = media.r;
            pixel[linha][coluna].g = media.g;
            pixel[linha][coluna].b = media.b;
        }
    }
}

// Image rotacionar90direita(Image img) {
//     Image rotacionada;

//     rotacionada.w = img.h;
//     rotacionada.h = img.w;

//     for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
//         for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
//             rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
//             rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
//             rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
//         }
//     }

//     return rotacionada;
// }

// void inverter_cores(unsigned short int pixel[512][512][3],
//                     unsigned int w, unsigned int h) {
//     for (unsigned int i = 0; i < h; ++i) {
//         for (unsigned int j = 0; j < w; ++j) {
//             pixel[i][j][0] = 255 - pixel[i][j][0];
//             pixel[i][j][1] = 255 - pixel[i][j][1];
//             pixel[i][j][2] = 255 - pixel[i][j][2];
//         }
//     }
// }

// Image cortar_imagem(Image img, int x, int y, int w, int h) {
//     Image cortada;

//     cortada.w = w;
//     cortada.h = h;

//     for(int i = 0; i < h; ++i) {
//         for(int j = 0; j < w; ++j) {
//             cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
//             cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
//             cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
//         }
//     }

//     return cortada;
// }


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].r,
                                 &img.pixel[i][j].g,
                                 &img.pixel[i][j].b);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                // img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                // for (unsigned int x = 0; x < img.h; ++x) {
                //     for (unsigned int j = 0; j < img.w; ++j) {
                //         unsigned short int pixel[3];
                //         pixel[0] = img.pixel[x][j][0];
                //         pixel[1] = img.pixel[x][j][1];
                //         pixel[2] = img.pixel[x][j][2];

                //         int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                //         int menor_r = (255 >  p) ? p : 255;
                //         img.pixel[x][j][0] = menor_r;

                //         p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                //         menor_r = (255 >  p) ? p : 255;
                //         img.pixel[x][j][1] = menor_r;

                //         p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                //         menor_r = (255 >  p) ? p : 255;
                //         img.pixel[x][j][2] = menor_r;
                //     }
                // }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.h, img.w, img.pixel, tamanho);
                break;
            }
            case 4: { // Rotacao
                // int quantas_vezes = 0;
                // scanf("%d", &quantas_vezes);
                // quantas_vezes %= 4;
                // for (int j = 0; j < quantas_vezes; ++j) {
                //     img = rotacionar90direita(img);
                // }
                break;
            }
            case 5: { // Espelhamento
                // int horizontal = 0;
                // scanf("%d", &horizontal);

                // int w = img.w, h = img.h;

                // if (horizontal == 1) w /= 2;
                // else h /= 2;

                // for (int i2 = 0; i2 < h; ++i2) {
                //     for (int j = 0; j < w; ++j) {
                //         int x = i2, y = j;

                //         if (horizontal == 1) y = img.w - 1 - j;
                //         else x = img.h - 1 - i2;

                //         Pixel aux1;
                //         aux1.r = img.pixel[i2][j][0];
                //         aux1.g = img.pixel[i2][j][1];
                //         aux1.b = img.pixel[i2][j][2];

                //         img.pixel[i2][j][0] = img.pixel[x][y][0];
                //         img.pixel[i2][j][1] = img.pixel[x][y][1];
                //         img.pixel[i2][j][2] = img.pixel[x][y][2];

                //         img.pixel[x][y][0] = aux1.r;
                //         img.pixel[x][y][1] = aux1.g;
                //         img.pixel[x][y][2] = aux1.b;
                //     }
                // }
                break;
            }
            case 6: { // Inversao de Cores
                // inverter_cores(img.pixel, img.w, img.h);
                break;
            }
            case 7: { // Cortar Imagem
                // int x, y;
                // scanf("%d %d", &x, &y);
                // int w, h;
                // scanf("%d %d", &w, &h);

                // img = cortar_imagem(img, x, y, w, h);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].r,
                                   img.pixel[i][j].g,
                                   img.pixel[i][j].b);

        }
        printf("\n");
    }
    return 0;
}
