#include <signal.h>
#include "command_parser.h"
#include "processes_manager.h"
#include <sys/types.h>

// Funcoes para tratamento de sinais

// SIGCHLD: Disparado quando um processos filho muda de estado
void sigchild(int signum){
  int status;
  //TODO: necessario lidar com os diferentes tipos de mudancas de estado
  int pid = waitpid(-1, &status, WNOHANG);

  if(pid > 0) {
    printf("Filho %d terminou\n", pid);
  }
}

// SIGINT: Disparado quando enviado o sinal de termino (^C)
void sigint(int signum) {
  printf("%d\n$ ", signum);
}

// SIGTSTP: Disparado quando enviado o sinal de suspensao (^Z)
void sigtstp(int signum) {
  printf("%d\n$ ", signum);
}

int main(int argc, char const *argv[])
{
    char *comando_str;
    Comando *comando;
    // Se o programa foi inicializado a partir de um terminal
    if (isatty(STDIN_FILENO))
    {
        // Instalacao das rotinas de tratamento de sinais
        signal(SIGCHLD, sigchild);
        signal(SIGINT, sigint);
        signal(SIGTSTP, sigint);

        while (tcgetpgrp(STDIN_FILENO) != getpgrp())
        {
            kill(-getpgrp(), SIGTTIN);
        }

        while (1)
        {
            comando_str = ler_comando();
            if (strlen(comando_str))
            {
              comando = parse_comando(comando_str);
              executar_comando(comando);
            }
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Programa precisa ser inicializado em um terminal\n");
        return EXIT_FAILURE;
    }
}
