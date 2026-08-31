#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
 
unsigned char *criar_matriz(int largura, int altura) {
    return malloc((size_t)largura * (size_t)altura);
}
 

void liberar_matriz(unsigned char *matriz) {
    free(matriz);
}
 


int gravar_pgm(const char *caminho, const unsigned char *matriz, int largura, int altura) {
    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        return -1;   /* main vai tratar isso como erro de criacao de arquivo <--- lembrar*/
    }
 
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            /* a matriz e 1D; o pixel (px,py) fica na posicao py*largura + px */
            fprintf(f, "%d", matriz[py * largura + px]);
 
            if (px < largura - 1) {
                fputc(' ', f);   /* espaco ENTRE os valores, nunca no fim da linha */
            }
        }
        fputc('\n', f);          /* uma linha do arquivo por linha da imagem */
    }
 
    fclose(f);
    return 0;
}
