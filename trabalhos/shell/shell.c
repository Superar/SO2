#include <signal.h>
#include "command_parser.h"
#include "processes_manager.h"
#include <sys/types.h>
#include <pwd.h>

// Funcoes para tratamento de sinais

// SIGCHLD: Disparado quando um processos filho muda de estado
void sigchild(int signum){
  int status;

  pid_t pid = waitpid(-1, &status, WNOHANG);

  if(pid > 0) {
    if(WIFSTOPPED(status))
    {
      atualiza_status_processo(pid, STOPPED);
    }
    else
    {
      printf("%s\n$ ", "terminou");
      atualiza_status_processo(pid, DONE);
    }
  }
}

// SIGINT: Disparado quando enviado o sinal de termino (^C)
void sigint(int signum) {
  printf("INT init");
  if(fg_proc != NULL)
  {
    if(kill(fg_proc->pid, SIGINT) < 0)
    {
      perror("Erro no envio de sinal ao processo");
    }
  }
}

// SIGTSTP: Disparado quando enviado o sinal de suspensao (^Z)
void sigtstp(int signum) {
  if(fg_proc != NULL)
  {
    if(kill(fg_proc->pid, SIGTSTP) < 0)
    {
      perror("Erro no envio de sinal ao processo");
    }
  }
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
        signal(SIGTSTP, sigtstp);

        while (tcgetpgrp(STDIN_FILENO) != getpgrp())
        {
            kill(-getpgrp(), SIGTTIN);
        }

        // Muda diretorio inicial para a home do usuario
        struct passwd *pw = getpwuid(getuid());
        const char *home_path = pw->pw_dir;
        chdir(home_path);

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
