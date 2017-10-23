#include "command_parser.h"

Comando* init_comando()
{
  Comando* cmd = malloc(sizeof(Comando));
  cmd->args = malloc(15*sizeof(char));
  cmd->nro_args = 0;
  cmd->err_option=0;
  cmd->out_option = 0;
  cmd->pipe = 0;
  cmd->bg = 0;
  cmd->next = NULL;

  return cmd;
}

char* ler_comando()
{
  char *buffer = NULL;
  size_t tamanho_buffer = 0;

  getline(&buffer, &tamanho_buffer, stdin);

  // Substitui o caracter '\n' pelo caracter '\0'
  char* newline = strchr(buffer, '\n');
  if (newline)
  {
    *newline = '\0';
  }

  return buffer;
}

char* analisa_delimitador(char *delim) {
  char* operador = malloc(3*sizeof(char));

  if(*delim == '>')
  {
    if(*(delim+1) == '>')
    {
      operador = ">>\0";
    }
    else
    {
      operador = ">\0";
    }
  }
  else if (*delim  == '<')
  {
      operador = "<\0";
  }
  else if (*delim == '&')
  {
    operador = "&\0";
  }
  else if (*delim == '|')
  {
    operador = "|\0";
  } else {
    free(operador);
    operador = NULL;
  }
  return operador;
}

Comando* parse_comando(char *str_comando)
{
  char* copy_comando = strdup(str_comando);

  int pos = 0;

  char **tokens = malloc(TAM_BUFFER_COMANDO * sizeof(char*));
  if (!tokens)
  {
    fprintf(stderr, "Erro na alocacao do vetor de comandos\n");
    exit(EXIT_FAILURE);
  }

  char *token, *token_aux;
  char *restante;
  char *cur_delim;

  token = strtok_r(copy_comando, " ",  &restante); // Primeiro token
  while (token != NULL)
  {
    cur_delim = analisa_delimitador(token);

    if(cur_delim != NULL) {
      tokens[pos++] = cur_delim;
    }

    token_aux = strtok(token, "<>&|");
    while (token_aux != NULL) {
      tokens[pos++] = token_aux;

      cur_delim = str_comando + (strchr(token_aux, '\0') - copy_comando);
      cur_delim = analisa_delimitador(cur_delim);
      if(cur_delim != NULL) {
        tokens[pos++] = cur_delim;
      }

      // tokens[pos++] = analisa_delimitador(cur_delim);
      token_aux = strtok(NULL, "<>&|");
    }

    token = strtok_r(restante, " ",  &restante); // Proximo token
  }

  tokens[pos] = NULL; // Token NULL para saber o final

  Comando *comando = init_comando();
  Comando *cur_comando = comando;

  int i;
  cur_comando->args[cur_comando->nro_args] = tokens[0];
  cur_comando->nro_args++;
  for (i = 1; i < pos; i++) {
    if(tokens[i] == NULL) {
      break;
    }
    else if(!strcmp(tokens[i],"&")){
      cur_comando->bg = 1;
    }
    else if (tokens[i][0] == '>'){
      if(tokens[i+1] != NULL)
      {
        cur_comando->out = tokens[i+1];
        cur_comando->out_option = strlen(tokens[i]);
        i++;
      }
    }
    else if (tokens[i][0] == '<')
    {
      if(tokens[i+1] != NULL)
      {
        cur_comando->in = tokens[i+1];
        i++;
      }
    }
    else if (!strcmp(tokens[i],"|")) {
      if(tokens[i+1] != NULL) {
        cur_comando->pipe = 1;
        cur_comando->next = init_comando();
        cur_comando->next->args[cur_comando->nro_args] = tokens[0];
        cur_comando->next->nro_args++;
        cur_comando = cur_comando->next;
        i++;
      }
    } else {
      cur_comando->args[cur_comando->nro_args] = tokens[i];
      cur_comando->nro_args++;
    }
  }

  return comando;
}
