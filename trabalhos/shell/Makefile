OBJECTS=command_parser.o processes_manager.o shell.o
CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXECUTAVEL=shell

all: $(EXECUTAVEL)

clean:
	rm -f $(OBJECTS) $(EXECUTAVEL)

# $@ : representa o nome do arquivo a ser feito (target)
# $? : representa os nomes das dependências
$(EXECUTAVEL): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $? -o $@
