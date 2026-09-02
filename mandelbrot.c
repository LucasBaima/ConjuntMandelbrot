#include "mandelbrot.h"

/* Regiao do plano complexo desenhada (enunciado):
 *   real       [-2,0 ; 1,0]
 *   imaginario [-1,5 ; 1,5]  */
#define REAL_MIN -2.0
#define REAL_MAX  1.0
#define IMAG_MIN -1.5
#define IMAG_MAX  1.5
 


static int escape_time(double cr, double ci, int max_iter) {
    double zr = 0.0, zi = 0.0;   /* z0 = 0 */
    int iter = 0;
 
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter) {
        double zr_novo = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;   
        zr = zr_novo;
        iter++;
    }
    return iter;   /* valor entre 0 e max_iter */
}


unsigned char mandelbrot_pixel(int px, int py, int largura, int altura,int max_iter) {
    
    double div_x = largura;   // divide por W  <- referencia pega no teste
    double div_y = altura;    
 
    /* pixel -> ponto complexo c (uma regra de tres) */
    double cr = REAL_MIN + (REAL_MAX - REAL_MIN) * px / div_x;
    double ci = IMAG_MIN + (IMAG_MAX - IMAG_MIN) * py / div_y;
 
    int iter = escape_time(cr, ci, max_iter);
 
    
    return (unsigned char)(255.0 * iter / max_iter);
}


