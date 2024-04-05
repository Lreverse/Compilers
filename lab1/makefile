CC = gcc
CFLAGS = -lfl -ly

FLEX = flex
BISON = bison
FLEXFLAGS =
BISONFLAGS = -d

OBJS = main.c Tree.c syntax.tab.c 
TARGET = parser

all: lex.yy.c $(TARGET) 

lex.yy.c: lexical.l
	@echo flex is running...
	@$(FLEX) $^

syntax.tab.c syntax.tab.h: syntax.y
	@echo bison is running...
	@$(BISON) $< $(BISONFLAGS)

$(TARGET): $(OBJS)
	@echo gcc is runing...
	@$(CC) $^ $(CFLAGS) -o $@ 
	@echo complete!

clean:
	@echo clean right now....
	@rm -f $(TARGET) lex.yy.c syntax.tab.c syntax.tab.h
	@echo clean complete!
