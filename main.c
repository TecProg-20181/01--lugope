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


void escala_de_cinza(Image* imagem) {
    //Percorre todos os pixels da imagem
    for (unsigned int i = 0; i < imagem->h; ++i) {
        for (unsigned int j = 0; j < imagem->w; ++j) {
            int media = imagem->pixel[i][j].r +
                        imagem->pixel[i][j].g +
                        imagem->pixel[i][j].b;
            media /= 3;

            imagem->pixel[i][j].r = media;
            imagem->pixel[i][j].g = media;
            imagem->pixel[i][j].b = media;
        }
    }

    // return image_cinza;

    // Image test;
    // for (unsigned int i = 0; i < img.h; ++i) {
    //     for (unsigned int j = 0; j < img.w; ++j) {
    //         img.pixel[i][j].r = img.pixel[1][j].r;
    //         img.pixel[i][j].g = img.pixel[1][j].g;
    //         img.pixel[i][j].b = img.pixel[1][j].b;
    //     }
    // }

    // Image imp = image_cinza;

    // return image_cinza;
}

void filtro_sepia(Image* imagem) {
    //Percorre todos os pixels da imagem
    for (unsigned int x = 0; x < imagem->h; ++x) {
        for (unsigned int y = 0; y < imagem->w; ++y) {
            //Pixel sepia
            Pixel pixel = imagem->pixel[x][y];

            //Defini tom sepia do pixel
            int p =  pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
            int menor_r = (255 >  p) ? p : 255;
            imagem->pixel[x][y].r = menor_r;

            p =  pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
            menor_r = (255 >  p) ? p : 255;
            imagem->pixel[x][y].g = menor_r;

            p =  pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
            menor_r = (255 >  p) ? p : 255;
            imagem->pixel[x][y].b = menor_r;
        }
    }
}

Image blur(Image imagem_blur, int diametro_blur) {
    // //Percorre cada pixel da imagem
    for (unsigned int linha = 0; linha < imagem_blur.h; ++linha) {
        for (unsigned int coluna = 0; coluna < imagem_blur.w; ++coluna) {
            //Variáveis
            Pixel media = {0, 0, 0};
            int h_anterior = imagem_blur.h - 1;
            int w_anterior = imagem_blur.w - 1;
            int raio_blur = diametro_blur/2;

            int menor_h = min( h_anterior, (linha + raio_blur) ); //Evita passar da borda de baixo da imagem
            int menor_w = min( w_anterior, (coluna + raio_blur) ); //Evita passar da borda direita da imagem

            //Faz as somm das cores dos pixels no primeiro quadrante do pixel base
            for(int y = max(0, linha - raio_blur); y <= menor_h; ++y) {
                for(int x = max(0, coluna - raio_blur); x <= menor_w; ++x) {
                    media.r += imagem_blur.pixel[y][x].r;
                    media.g += imagem_blur.pixel[y][x].g;
                    media.b += imagem_blur.pixel[y][x].b;
                }
            }

            //Define a média das cores na área de blur
            media.r /= pow(diametro_blur,2);
            media.g /= pow(diametro_blur,2);
            media.b /= pow(diametro_blur,2);

            //Define nova cor do pixel como a média das cores na área do blur
            imagem_blur.pixel[linha][coluna].r = media.r;
            imagem_blur.pixel[linha][coluna].g = media.g;
            imagem_blur.pixel[linha][coluna].b = media.b;
        }
    }

    return imagem_blur;

}

Image rotacionar90direita(Image img) {
    // Image* nova_imagem = NULL;

    // nova_imagem.w = img.h;
    // nova_imagem.h = img.w;

    // for (unsigned int i = 0, y = 0; i < nova_imagem.h; ++i, ++y) {
    //     for (int j = nova_imagem.w - 1, x = 0; j >= 0; --j, ++x) {
    //         nova_imagem.pixel[i][j].r = img.pixel[x][y].r;
    //         nova_imagem.pixel[i][j].g = img.pixel[x][y].g;
    //         nova_imagem.pixel[i][j].b = img.pixel[x][y].b;
    //     }
    // }

    return img;
}

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
                escala_de_cinza(&img);

                break;
            }
            case 2: { // Filtro Sepia
                filtro_sepia(&img);

                break;
            }
            case 3: { // Blur
                // int tamanho = 0;
                // scanf("%d", &tamanho);
                // img = blur(img, tamanho);

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
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;

                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }

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
            default: {
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
