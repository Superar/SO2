#include "processes_manager.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void insere_processo(Processo *p)
{
  if(lista_proc == NULL)
  {
    lista_proc = p;
  }
  else if (busca_proc_pid(p->pid) == NULL)
  {
    Processo* proc_atual = lista_proc;
    while (proc_atual->proximo != NULL)
    {
      proc_atual = proc_atual->proximo;
    }
    proc_atual->proximo = p;
    p->anterior = proc_atual;
  }
}

Processo* busca_proc_pid(pid_t pid)
{
  if(lista_proc == NULL)
  {
    return NULL;
  }
  else
  {
    Processo* proc_atual = lista_proc;
    while ((proc_atual->proximo != NULL)&&(pid != proc_atual->pid))
    {
      proc_atual = proc_atual->proximo;
    }

    if(pid == proc_atual->pid)
    {
      return proc_atual;
    }

    return NULL;
  }
}

Processo* busca_proc_num_job(int num_job)
{
  if(lista_proc == NULL)
  {
    return NULL;
  }
  else
  {
    Processo* proc_atual = lista_proc;
    int i;
    for(i = 1; (i < num_job) && (proc_atual != NULL); i++)
    {
      proc_atual = proc_atual->proximo;
    }
    return proc_atual;
  }
}

void atualiza_status_processo(pid_t pid, int status)
{
  Processo* p = busca_proc_pid(pid);
  if(p != NULL)
  {
    p->status = status;
  }
}

int retira_processo(pid_t pid)
{
  // Acha na lista o processo que desejamos retirar
  Processo* proc_atual = busca_proc_pid(pid);
  if(proc_atual == NULL)
  {
    return -1;
  }
  else
  {
    if(proc_atual->anterior == NULL) // Primeiro processo da lista
    {
      lista_proc = proc_atual->proximo; // Raiz da lista precisa mudar
      if(lista_proc != NULL)
      {
        lista_proc->anterior = NULL;
      }
    }
    else
    {
      proc_atual->anterior->proximo = proc_atual->proximo;

      if(proc_atual->proximo != NULL)
      {
        proc_atual->proximo->anterior = proc_atual->anterior;
      }
    }

    free(proc_atual);

    return 0;
  }
}

void print_args(char** args)
{
  int i;
  char* arg = args[0];

  for(i = 1; arg != NULL; arg = args[i++])
  {
    printf("%s ", arg);
  }
}

// Lista os processos filhos ainda em execução
void jobs()
{
  if(lista_proc != NULL)
  {
    Processo* proc_atual = lista_proc;
    Processo* proximo_proc;
    char status[10];

    int i;
    for(i = 1; proc_atual != NULL; i++)
    {
      if(proc_atual->status == RUNNING)
      {
        strcpy(status, "RUNNING");
      }
      else if (proc_atual->status == STOPPED)
      {
        strcpy(status, "STOPPED");
      }
      else
      {
        strcpy(status, "DONE");
      }

      printf("[%d] %s\t%d\t", i, status, proc_atual->pid);
      print_args(proc_atual->args);
      printf("\n");

      proximo_proc = proc_atual->proximo;

      if(proc_atual->status == DONE)
      {
        retira_processo(proc_atual->pid);
      }
      proc_atual = proximo_proc;
    }
  }
}

// Configura o redirecionamento de estrada e saida
void configura_redir(Processo* p, Comando* comando)
{
  if(comando->out != NULL)
  {
    int flags = O_WRONLY|O_CREAT;

    if(comando->out_option > 1)
    {
      flags |= O_APPEND;
    }

    int fd = open(comando->out, flags, 00644);

    if(fd < 0) {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDOUT_FILENO);
    close(fd);
  }

  if(comando->in != NULL)
  {

    int fd = open(comando->in, O_RDONLY);

    if(fd < 0)
    {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDIN_FILENO);
    close(fd);
  }

  if(comando->err != NULL)
  {
    int flags = O_WRONLY|O_CREAT;

    if(comando->err_option > 1)
    {
      flags |= O_APPEND;
    }

    int fd = open(comando->out, flags, 00644);

    if(fd < 0)
    {
      perror("Impossivel abrir arquivo");
      exit(EXIT_SUCCESS);
    }

    dup2(fd, STDERR_FILENO);
    close(fd);
  }
}

// Inicia um novo processo
void inicia_processo(Processo** p, Comando* comando)
{
  Processo* proc_atual;
  Comando* comando_atual = comando;
  int pipefd[2];
  // variavel de controle do arquivo de entrada do processo
  int stdin = (int) STDIN_FILENO;
  // variavel de controle do arquivo de entrada do processo
  int stdout;

  while(comando_atual != NULL)
  {
    proc_atual = malloc(sizeof(Processo));
    //Se foi indicado um pipe, realizamos as trocas dos arquivos de entrada e saida
    if (comando_atual->pipe)
    {
      if(pipe(pipefd) < 0)
      {
        perror("Erro pipe");
        exit(EXIT_SUCCESS);
      }
      stdout = pipefd[1];
    }
    else
    {
      stdout = (int) STDOUT_FILENO;
    }

    // configuramos o processo para ser executado
    proc_atual->args = comando_atual->args;
    proc_atual->status = RUNNING;

    proc_atual->pid = fork();

    if (proc_atual->pid < 0)
    {
      fprintf(stderr, "Erro na execução do fork\n");
      exit(EXIT_FAILURE);
    }
    else if (proc_atual->pid == 0) // filho
    {
      // Caso o arquivo de entrada nao for o padrao quer dizer que ha a presenca do pipe
      if(stdin != (int) STDIN_FILENO)
      {
        // Copia para o novo arquivo para o arquivo padrao de entrada
        if(dup2(stdin,STDIN_FILENO) < 0)
        {
          perror("erro in");
          exit(EXIT_FAILURE);
        }
        // fecha-se a ponta do pipe que foi copiada
        close(stdin);
      }
      // Mesmo procedimento no arquivo de saida
      if(stdout != (int) STDOUT_FILENO)
      {
        if(dup2(stdout, STDOUT_FILENO) < 0)
        {
          perror("erro out");
          exit(EXIT_FAILURE);
        }
        close(stdout);
      }
      // Configura o resultante do processo antes de mudar sua imagem
      configura_redir(proc_atual,comando_atual);
      execvp(proc_atual->args[0], proc_atual->args);
      fprintf(stderr, "%s: comando não encontrado\n", comando_atual->args[0]);
      exit(EXIT_FAILURE);
    }

    // No processo pai
    if (comando_atual->pipe)
    {
      stdin = pipefd[0];
      close(stdout);
    }
    else if(stdin != (int) STDIN_FILENO)
    {
      close(stdin);
    }

    comando_atual = comando_atual->next;
  }
  // Retornamos o ultimo processo criado via parametro
  *p = proc_atual;
}

void coloca_background(Processo* p)
{
  // Para colocar um processo em bg, basta mandar um sinal de continuacao
  // e atualizar em nossa lista de processos
  if(!kill(p->pid, SIGCONT))
  {
    p->status = RUNNING;
  }
}

void coloca_foreground(Processo* p)
{
  int status;
  // Apontamos nosso ponteiro de fg para o processo que se tornara fg
  fg_proc = p;
  // Atualizamos seu estado
  fg_proc->status = RUNNING;
  // Manda sinal para que o processo continue
  if(kill(p->pid, SIGCONT) == 0)
  {
    // Como esta em background, o shell deve espera-lo mudar de estado (terminar ou parar)
    waitpid(fg_proc->pid, &status, WUNTRACED);
    // Filho mudou de estado
    // Se parou, devemos atualizar o seu estado e inserir em lista_proc
    if(WIFSTOPPED(status))
    {
      fg_proc->status = STOPPED;
      insere_processo(fg_proc);
    }
    // Caso contrario, foi terminado e devemos liberar a estrutura processo
    else
    {
      if(retira_processo(fg_proc->pid) < 0)
      {
        free(fg_proc);
      }
    }

    fg_proc = NULL;
  }
  else
  {
    perror("Erro no envio de sinal");
  }
}

int executar_comando(Comando* comando)
{
  if (!verifica_builtins(comando))
  {
    Processo* p = NULL;

    inicia_processo(&p, comando);

    if(!comando->bg)
    {
      coloca_foreground(p);
    }
    else
    {
      insere_processo(p);
    }
  }
  return 0;
}

int verifica_builtins(Comando* comando)
{
  // builtin 'exit' sai do shell
  if (!strcmp(comando->args[0], "exit"))
  {
    exit(EXIT_SUCCESS);
  }
  // exibe o estado dos processos em execucao
  else if(!strcmp(comando->args[0], "jobs"))
  {
    jobs();
  }
  // builtin 'cd' muda o diretorio corrente de trabalho
  else if (!strcmp(comando->args[0], "cd"))
  {
    if (comando->args[1])
    {
      // Possui um caminho indicado
      if(chdir(comando->args[1]) == -1)
      {
        // Erro
        fprintf(stderr, "cd: %s: %s\n", comando->args[1], strerror(errno));
      }
    }
    else
    {
      // Muda o diretorio para o diretorio home do usuario
      struct passwd *pw = getpwuid(getuid());
      chdir(pw->pw_dir);
    }
  }
  // continua um processo (identificado por seu pid ou numero de job) em background
  else if (!strcmp(comando->args[0], "bg"))
  {
    if(comando->args[1] != NULL)
    {
      int nro;
      Processo* p = NULL;
      // % indica a passagem do numero do job em seguida
      if(comando->args[1][0] == '%')
      {
        nro = atoi(comando->args[1] + sizeof(char));
        p = busca_proc_num_job(nro);
      }
      // Se apenas numeros, indica o numero pid do processo
      else
      {
        nro = atoi(comando->args[1]);
        p = busca_proc_pid(nro);
      }

      if(p == NULL)
      {
        printf("%s\n", "Job inexistente");
      }
      else
      {
        print_args(p->args);
        printf("\n");
        coloca_background(p);
      }
    }
  }
  // continua um processo (identificado por seu pid ou numero de job) em foreground
  else if (!strcmp(comando->args[0], "fg"))
  {
    if(comando->args[1] != NULL)
    {
      int nro;
      Processo* p = NULL;

      if(comando->args[1][0] == '%')
      {
        nro = atoi(comando->args[1] + sizeof(char));
        p = busca_proc_num_job(nro);
      }
      else
      {
        nro = atoi(comando->args[1]);
        p = busca_proc_pid(nro);
      }

      if(p == NULL)
      {
        printf("%s\n", "Job inexistente");
      }
      else
      {
        print_args(p->args);
        printf("\n");
        coloca_foreground(p);
      }
    }
  }
  else
  {
    return 0;
  }
  return 1;
}
