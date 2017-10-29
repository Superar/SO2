OBJECTS=command_parser.o processes_manager.o shell.o
CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXECUTAVEL=shell
LEXER_SRC=lex.yy.c
LEXER=shell.lex

all: $(EXECUTAVEL)
	./shell

clean:
	rm -f $(OBJECTS) $(EXECUTAVEL) $(LEXER_SRC)

# $@ : representa o nome do arquivo a ser feito (target)
# $? : representa os nomes das dependências que foram alteradas
# $^ : representa os nomes de todas as dependências
$(EXECUTAVEL): $(OBJECTS) $(LEXER_SRC)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@

$(LEXER_SRC): $(LEXER)
	lex $?
