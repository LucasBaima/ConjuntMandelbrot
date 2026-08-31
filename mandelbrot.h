#ifndef MANDELBROT_H
#define MANDELBROT_H
 
 /*Faz tres coisas:
    1) converte o pixel (px, py) num ponto c do plano complexo
    2) roda o escape-time nesse ponto
    3) normaliza o numero de iteracoes para 0..255
 
  Todas as quatro implementacoes (serial, openmp, pthreads) chamam ESTA
  MESMA funcao para cada pixel. Por isso as quatro saidas sao identicas:
  o que muda entre elas e so QUEM calcula QUAIS pixels, nunca a conta de
  um pixel.*/

  unsigned char mandelbrot_pixel(int px, int py, int largura, int altura, int max_iter);


 
#endif /* MANDELBROT_H */
 
