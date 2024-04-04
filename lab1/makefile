CC = gcc
CFLAGS = -lfl -ly

FLEX = flex
BISON = bison
FLEXFLAGS =
BISONFLAGS = -d

OBJS = main.c syntax.tab.c
TARGET = parser

all: lex.yy.c $(TARGET) 


lex.yy.c: lexical.l
	$(FLEX) $^

syntax.tab.c syntax.tab.h: syntax.y
	$(BISON) $^ $(BISONFLAGS)

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) -o $@ 

clean:
	@echo clean right now....
	@rm -f $(TARGET) lex.yy.c syntax.tab.c syntax.tab.h
	@echo clean complete!
