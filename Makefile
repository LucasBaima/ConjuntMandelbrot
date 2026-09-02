# Makefile do projeto 
#
#   make          -> compila e gera o executavel ./mandelbrot
#   make clean    -> tira o executavel, e todos .o e os arquivos gerados
#
# Flags:
#   -fopenmp  habilita o OpenMP
#   -pthread  habilita os POSIX threads (usados no proximo passo) -- PENDENTE

CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -fopenmp -pthread
OBJ     = main.o mandelbrot.o implementacoes.o imagem.o
BIN     = mandelbrot


#-----------------------------------------------------------------------------------------


all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

main.o: main.c implementacoes.h imagem.h
	$(CC) $(CFLAGS) -c main.c

mandelbrot.o: mandelbrot.c mandelbrot.h
	$(CC) $(CFLAGS) -c mandelbrot.c

implementacoes.o: implementacoes.c implementacoes.h mandelbrot.h
	$(CC) $(CFLAGS) -c implementacoes.c

imagem.o: imagem.c imagem.h
	$(CC) $(CFLAGS) -c imagem.c

clean:
	rm -f $(BIN) $(OBJ) mandelbrot_*.pgm times.txt

.PHONY: all clean