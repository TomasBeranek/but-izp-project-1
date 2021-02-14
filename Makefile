# Soubor:       Makefile
# Predmet:      Zaklady programovani (IZP) -- FIT VUT
# Projekt:      Jednoduchy textovy editor
# Autor:        Tomas Beranek (xberan46)
# Email:        xberan46@stud.fit.vutbr.cz
# Datum:        25.10.1018

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror
SRC=proj1.c
OBJ=$(SRC:.c=.o)
BIN=editor

.PHONY: all

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(BIN)
