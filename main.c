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
}

void filtro_sepia(Image* imagem) {
    //Percorre todos os pixels da imagem
    for (unsigned int x = 0; x < imagem->h; ++x) {
        for (unsigned int y = 0; y < imagem->w; ++y) {
            //Pixel sepia
            Pixel pixel = imagem->pixel[x][y];

            //Define tom sepia do pixel
            int cor_auxiliar = 0;

            //Modifica vermelho
            cor_auxiliar =  pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
            cor_auxiliar = min(cor_auxiliar, 255);
            imagem->pixel[x][y].r = cor_auxiliar;
            //Modifica verde
            cor_auxiliar =  pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
            cor_auxiliar = min(cor_auxiliar, 255);
            imagem->pixel[x][y].g = cor_auxiliar;
            //Modifica azul
            cor_auxiliar =  pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
            cor_auxiliar = min(cor_auxiliar, 255);
            imagem->pixel[x][y].b = cor_auxiliar;
        }
    }
}

void blur(Image* imagem, int diametro_blur) {
    // //Percorre cada pixel da imagem
    for (unsigned int linha = 0; linha < imagem->h; ++linha) {
        for (unsigned int coluna = 0; coluna < imagem->w; ++coluna) {
            //Variáveis
            Pixel media = {0, 0, 0};
            int h_anterior = imagem->h - 1;
            int w_anterior = imagem->w - 1;
            int raio_blur = diametro_blur/2;

            int menor_h = min( h_anterior, (linha + raio_blur) ); //Evita passar da borda de baixo da imagem
            int menor_w = min( w_anterior, (coluna + raio_blur) ); //Evita passar da borda direita da imagem

            //Faz as somm das cores dos pixels no primeiro quadrante do pixel base
            for(int y = max(0, linha - raio_blur); y <= menor_h; ++y) {
                for(int x = max(0, coluna - raio_blur); x <= menor_w; ++x) {
                    media.r += imagem->pixel[y][x].r;
                    media.g += imagem->pixel[y][x].g;
                    media.b += imagem->pixel[y][x].b;
                }
            }

            //Define a média das cores na área de blur
            media.r /= pow(diametro_blur,2);
            media.g /= pow(diametro_blur,2);
            media.b /= pow(diametro_blur,2);

            //Define nova cor do pixel como a média das cores na área do blur
            imagem->pixel[linha][coluna] = media;
        }
    }
}

void rotacionar90direita(Image* imagem) {
    Image img_auxiliar = *imagem;

    //Troca a largura e a autura
    img_auxiliar.w = imagem->h;
    img_auxiliar.h = imagem->w;

    //Desenha a nova imagem na variável auxiliar
    for (unsigned int i = 0; i < img_auxiliar.h; ++i) {
        for (int j = img_auxiliar.w - 1, x = 0; j >= 0; --j, ++x) {
            img_auxiliar.pixel[i][j].r = imagem->pixel[x][i].r;
            img_auxiliar.pixel[i][j].g = imagem->pixel[x][i].g;
            img_auxiliar.pixel[i][j].b = imagem->pixel[x][i].b;
        }
    }

    //Transfere a nova imagem
    *imagem = img_auxiliar;
}

void espelhar(Image* imagem, unsigned short int eixo) {
    int w = imagem->w, h = imagem->h;

    if (eixo == 1) { //Vertical
        w /= 2;

    } else { //Horizontal
        h /= 2;
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int x = i, y = j;

            if (eixo == 1){ //Vertical
                y = imagem->w - j;
                y -= 1;

            } else { //Horizontal
                x = imagem->h - i;
                x -= 1;
            }

            //Faz a troca de pixel
            Pixel pixel_auxiliar;

            pixel_auxiliar = imagem->pixel[i][j];
            imagem->pixel[i][j] = imagem->pixel[x][y];
            imagem->pixel[x][y] = pixel_auxiliar;
        }
    }
}

void inverter_cores(Image* imagem) {
    for (unsigned int i = 0; i < imagem->h; ++i) {
        for (unsigned int j = 0; j < imagem->w; ++j) {
            imagem->pixel[i][j].r = 255 - imagem->pixel[i][j].r;
            imagem->pixel[i][j].g = 255 - imagem->pixel[i][j].g;
            imagem->pixel[i][j].b = 255 - imagem->pixel[i][j].b;
        }
    }
}

void cortar_imagem(Image* imagem, int x, int y, int nova_largura, int nova_altura) {
    Image imagem_cortada;

    imagem_cortada.w = nova_largura;
    imagem_cortada.h = nova_altura;

    //Desenha nova imagem dentro dos parâmetros de corte
    for(int i = 0; i < nova_altura; ++i) {
        for(int j = 0; j < nova_largura; ++j) {
            imagem_cortada.pixel[i][j] = imagem->pixel[i + y][j + x];
        }
    }

    //Define nova imagem
    *imagem = imagem_cortada;
}

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
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(&img, tamanho);

                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;

                for (int j = 0; j < quantas_vezes; ++j) {
                    rotacionar90direita(&img);
                }

                break;
            }
            case 5: { // Espelhamento
                unsigned short int eixo = 0;
                scanf("%hu", &eixo);
                espelhar(&img, eixo);

                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(&img);
                break;
            }
            case 7: { // Cortar Imagem
                int posicao_x, posicao_y;
                scanf("%d %d", &posicao_x, &posicao_y);
                int nova_largura, nova_altura;
                scanf("%d %d", &nova_largura, &nova_altura);

                cortar_imagem(&img, posicao_x, posicao_y, nova_largura, nova_altura);
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
