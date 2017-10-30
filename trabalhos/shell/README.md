# Criação de um Shell

### Implementado por:
- Marcio Lima Inácio - 587265

- Felipe Sampaio de Souza - 619523

---

Para executar o shell, é necessário ter o Lex, analisador léxico do Yacc, instalado.
O Lex já vem por padrão em diversas distribuições linux. A implementação deste projeto foi concebida com a utilização do Lex versão 2.6.1.

Mais informações podem ser encontradas em: http://dinosaur.compilertools.net/

O projeto consiste de diversos arquivos. Todos devem estar presentes:

- command_parser.h
- command_parser.c
- Makefile
- processes_manager.h
- processes_manager.c
- shell.c
- shell.lex

Para compilar:
    `make`

Para executar
    `./shell`

O programa deve ser capaz de executar todas as operações listadas abaixo:

- [x] $ prog
- [x] $ prog parâmetros
- [x] $ pwd
- [x] $ exit
- [x] $ prog &
- [x] $ prog parâmetros > arquivo
- [x] $ prog parâmetros < arquivo
- [x] $ prog parâmetros > arq_saída < arq_entrada
- [x] $ prog parâmetros >> arq_saída
- [x] $ prog parâmetros 2> arq_erros
- [x] $ prog parâmetros 2>> arq_erros
- [x] $ prog1 param_1 | prog2 param_2
- [x] $ jobs
- [x] $ fg %num_job
- [x] $ bg %num_job
- [x] $ cd diretório
