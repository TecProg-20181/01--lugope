// teste: cat lena.ppm entrada.txt | ./Photopobre.exe > saida.ppm
/*
*   Lucas Gomes Pereira
*   13/0013242
*/ 

#include "ppmlib.h"

//Código das ações
enum opcoes {ESCALA_CINZA = 1, FILTRO_SEPIA, BLUR, ROTACAO, ESPELHAMENTO, INVERSAO_CORES, CORTAR_IMAGEM};

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
            case ESCALA_CINZA: { // Escala de Cinza
                escala_de_cinza(&img);

                break;
            }
            case FILTRO_SEPIA: { // Filtro Sepia
                filtro_sepia(&img);

                break;
            }
            case BLUR: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(&img, tamanho);

                break;
            }
            case ROTACAO: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;

                for (int j = 0; j < quantas_vezes; ++j) {
                    rotacionar90direita(&img);
                }

                break;
            }
            case ESPELHAMENTO: { // Espelhamento
                unsigned short int eixo = 0;
                scanf("%hu", &eixo);
                espelhar(&img, eixo);

                break;
            }
            case INVERSAO_CORES: { // Inversao de Cores
                inverter_cores(&img);
                break;
            }
            case CORTAR_IMAGEM: { // Cortar Imagem
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
