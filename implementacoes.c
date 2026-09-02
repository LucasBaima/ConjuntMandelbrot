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
 


/*
 * OPENMP - o mesmo trabalho da serial, mas dividido automaticamente.  
 *
 * A diferenca em relacao a serial -> o #pragma. Ele diz ao
 * compilador "divida as repeticoes deste for entre varias threads".
 * A biblioteca cria as threads, reparte as linhas e junta tudo no fim.
*/

void mandelbrot_openmp(unsigned char *img, int largura, int altura, int max_iter, int num_threads) {
    omp_set_num_threads(num_threads);
 
    #pragma omp parallel for schedule(dynamic)
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            img[(long)py * largura + px] =
                mandelbrot_pixel(px, py, largura, altura, max_iter);
        }
    }
}
