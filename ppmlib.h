/*
*   Lucas Gomes Pereira
*   13/0013242
*/ 

#include <stdio.h>
#include <math.h>
#include "structs.h"

/*
*	Funções de comparação
*/
// Retorna o maior número
int max(int a, int b);

// Retorna o menor número
int min(int a, int b);

// Verifica se os pixels são iguais
int pixel_igual(Pixel p1, Pixel p2);

/*
*	Funções para manipulação das imagens
*/
// Adiciona um filtro preto e branco
void escala_de_cinza(Image* imagem);

// Adiciona um filtro sepia
void filtro_sepia(Image* imagem);

// Adiciona efeito de blur
void blur(Image* imagem, int diametro_blur);

// Rotadciona a imagem 90 graus no sentido horário
void rotacionar90direita(Image* imagem);

// Espelha a imagem no eixo horizontal ou vertical
void espelhar(Image* imagem, unsigned short int eixo);

// Aplica um filtro para mostrar as cores inversas
void inverter_cores(Image* imagem);

// Corta imagem dentro da área determinada
void cortar_imagem(Image* imagem, int x, int y, int nova_largura, int nova_altura);

/*
* Funções para leitura/escrita de imagens PPM
*/
//Carega uma imagem escolhida pelo usuário
void lerImagem(Image* imagem);

//Imprime o arquivo da imagem
void escreverImagem(Image* imagem);
