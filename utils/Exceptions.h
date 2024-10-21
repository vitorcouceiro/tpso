#define ERRO_HEADER "[ERRO]"
#define INFO_HEADER "[INFO]"
//Info

//Error
#define INVALID_ARGS_FEED ERRO_HEADER " Numero de argumentos invalidos. Sintaxe: ./feed <nome>\n" 
#define INVALID_ARGS_MANA ERRO_HEADER " Numero de argumentos invalidos. Sintaxe: ./manager\n" 
#define MAX_USERS_REACHED ERRO_HEADER " Numero maximo de utilizadores atingido.\n"
#define SYNTAX_ERROR_TOPICS ERRO_HEADER " Erro de sintaxe. Sintaxe: topics\n"
#define SYNTAX_ERROR_MSG ERRO_HEADER " Erro de sintaxe. Sintaxe: msg <topico> <duracao> <mensagem>\n"
#define SYNTAX_ERROR_SUBCRIBE ERRO_HEADER " Erro de sintaxe. Sintaxe: subscribe <topico>\n"
#define SYNTAX_ERROR_UNSUBCRIBE ERRO_HEADER " Erro de sintaxe. Sintaxe: unsubscribe <topico>\n"
#define SYNTAX_ERROR_HELP ERRO_HEADER " Erro de sintaxe. Sintaxe: help\n"
#define TOPIC_LENGTH_ERROR ERRO_HEADER " O topico nao pode ter mais de 20 caracteres.\n"
#define MESSAGE_LENGTH_ERROR ERRO_HEADER " A mensagem nao pode ter mais de 300 caracteres.\n"
#define INVALID_COMMAND ERRO_HEADER " Comando invalido.\n"
