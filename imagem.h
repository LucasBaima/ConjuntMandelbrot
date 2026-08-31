#ifndef IMAGEM_H
#define IMAGEM_H
 

 
/* Aloca uma matriz de largura*altura bytes (um byte por pixel).
 * Retorna o ponteiro, ou NULL se faltar memoria. */
unsigned char *criar_matriz(int largura, int altura);
 
/* Libera a matriz criada por criar_matriz. */
void liberar_matriz(unsigned char *matriz);
 
/* Grava a matriz num arquivo texto, SEM cabecalho de formato:
 * um valor por pixel, separados por espaco, uma linha do arquivo
 * por linha da imagem.
 * Retorna 0 em sucesso, ou -1 se nao conseguiu criar/gravar o arquivo. */
int gravar_pgm(const char *caminho,
               const unsigned char *matriz,
               int largura, int altura);

 
#endif 
