CC = clang
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = -lpthread
LIBS = 
SOURCES = sudoku.c sudoku_check.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = sudoku.exe
.PHONY: clean help

sudoku.exe : $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"
