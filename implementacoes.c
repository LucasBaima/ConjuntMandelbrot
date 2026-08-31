#include "implementacoes.h"
#include "mandelbrot.h"
 

void mandelbrot_serial(unsigned char *img, int largura, int altura, int max_iter, int num_threads) {
    (void)num_threads;   /* a serial nao usa threads */
 
    long total = (long)largura * altura;   /* tamanho da fila de pixels */
 
    for (long i = 0; i < total; i++) {
        int px = (int)(i % largura);
        int py = (int)(i / largura);
        img[i] = mandelbrot_pixel(px, py, largura, altura, max_iter);
    }
}
 
