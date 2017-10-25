#include "command_parser.h"

Comando* init_comando()
{
    Comando* cmd = malloc(sizeof(Comando));
    cmd->args = malloc(15 * sizeof(char*));
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
    printf("$ ");
    getline(&buffer, &tamanho_buffer, stdin);

    // Substitui o caracter '\n' pelo caracter '\0'
    char* newline = strchr(buffer, '\n');
    if (newline)
    {
        *newline = '\0';
    }

    return buffer;
}

Comando* parse_comando(char *str_comando)
{
    int pos = 0, tamanho_buffer = TAM_BUFFER_COMANDO;
    char **tokens = malloc(tamanho_buffer * sizeof(char*));
    int token;

    if (!tokens)
    {
        fprintf(stderr, "Erro na alocacao do vetor de comandos\n");
        exit(EXIT_FAILURE);
    }

    setup_lexer_string(str_comando);
    while ( (token = yylex()) )
    {
        if (token != SPACE)
        {
            char *aux = NULL;
            aux = malloc(yyleng * sizeof(char));
            strcpy(aux, yytext);
            tokens[pos++] = aux;
        }

        // Caso o vetor de tokens nao seja suficiente
        if (pos == tamanho_buffer-1)
        {
            tamanho_buffer += TAM_BUFFER_COMANDO;
            tokens = realloc(tokens, tamanho_buffer);
        }
    }
    tokens[pos] = NULL; // Token NULL para saber o final

    Comando *comando = init_comando();
    Comando *cur_comando = comando;

    int i;
    cur_comando->args[cur_comando->nro_args] = tokens[0];
    cur_comando->nro_args++;

    for (i = 1; i < pos; i++)
    {
        if(tokens[i] == NULL)
        {
            break;
        }
        else if(!strcmp(tokens[i],"&"))
        {
            cur_comando->bg = 1;
        }
        else if (tokens[i][0] == '>')
        {
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
        else if (tokens[i][0] == '2') {
          if(tokens[i+1] != NULL)
          {
              cur_comando->err = tokens[i+1];
              cur_comando->err_option = strlen(tokens[i]) - 1;
              i++;
          }
        }
        else if (!strcmp(tokens[i],"|"))
        {
            if(tokens[i+1] != NULL)
            {
                cur_comando->pipe = 1;
                cur_comando->next = init_comando();
                cur_comando->next->args[cur_comando->nro_args] = tokens[0];
                cur_comando->next->nro_args++;
                cur_comando = cur_comando->next;
                i++;
            }
        }
        else
        {
            cur_comando->args[cur_comando->nro_args] = tokens[i];
            cur_comando->nro_args++;
        }
    }
    cur_comando->args[cur_comando->nro_args] = NULL;

    return comando;
}

int yywrap() { return 1; }
