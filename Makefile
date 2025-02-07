CC = gcc
CFLAGS = -Wall -g
FLEX = flex
BISON = bison
LIBS = 

OBJS = lex.yy.o fort.tab.o main.o optimization.o codegen.o symtab.o error.o

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS) $(LIBS)

fort.tab.c fort.tab.h: fort.y
	$(BISON) -d fort.y

lex.yy.c: fort.l fort.tab.h
	$(FLEX) fort.l

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f compiler compiler.exe *.o lex.yy.c fort.tab.c fort.tab.h