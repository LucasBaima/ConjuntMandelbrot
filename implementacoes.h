#ifndef IMPLEMENTACOES_H
#define IMPLEMENTACOES_H
 
/*
 * As QUATRO estrategias que preenchem a matriz inteira de pixels.
 * Todas tem a MESMA assinatura de proposito: assim o main consegue
 * tratar as quatro do mesmo jeito (com um unico "atalho de funcao").
 * Todas produzem exatamente a mesma matriz.
 *
 *   img         -> matriz de saida (largura*altura bytes)
 *   largura,    -> dimensoes da imagemaltura
 *   max_iter    -> limite de iteracoes por ponto
 *   num_threads -> n° de threads (ignorado na serial)
 */
 
void mandelbrot_serial   (unsigned char *img, int largura, int altura,int max_iter, int num_threads);
 
void mandelbrot_openmp   (unsigned char *img, int largura, int altura, int max_iter, int num_threads);
 
void mandelbrot_pthreads1(unsigned char *img, int largura, int altura,int max_iter, int num_threads);
 
void mandelbrot_pthreads2(unsigned char *img, int largura, int altura,int max_iter, int num_threads);
 
#endif 
