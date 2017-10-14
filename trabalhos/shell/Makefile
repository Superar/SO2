OBJECTS=command_parser.o processes_manager.o
CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXECUTAVEL=shell

all: $(EXECUTAVEL)

clean:
	rm -f $(OBJECTS) $(EXECUTAVEL)

# $@ : representa o nome do arquivo a ser feito (target)
# $? : representa os nomes das dependências
$(EXECUTAVEL): shell.c $(OBJECTS)
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

command_parser.o: command_parser.c command_parser.h
	$(CC) $(CFLAGS) -c command_parser.c $(LDFLAGS) -o $@

processes_manager.o: processes_manager.c processes_manager.h
	$(CC) $(CFLAGS) -c processes_manager.c $(LDFLAGS) -o $@
