
#include "TrabFinal.h"

int main(int argc, char *argv[])
{
    PT_TWEET *Tweets = NULL;
    PT_USER *UsuariosPorTweet = NULL;
    PT_USER *UsuariosPorEngajamento = NULL;
    PT_USER *UsuariosPorRT = NULL;
    PT_MENCAO *UsuariosPorMencao = NULL;
    PT_HASHTAG *Hashtags = NULL;
    PT_LISTA_HASHTAG *pt_hashtags = NULL;
    LISTA_OP* ListaOp = NULL;

    clock_t T_Inicial, T_Final, T_Total;

    setlocale(LC_ALL, "");

    if(argc != 4){
        printf("Numero incorreto de parametros.\n Para chamar o programa digite: exemplo <arq_entrada> <arq_operacoes> <arq_saida>\n");
        return 1;
    }
    else{
        T_Inicial = clock();
        // Abre o arquivo de entradae e Criar as arvores utilizadas pelas operacoes
        AtribuiStructs(argv[1], &Tweets, &UsuariosPorTweet, &UsuariosPorRT, &UsuariosPorEngajamento, &UsuariosPorMencao, &Hashtags, &pt_hashtags);
        // Abre o arquivo de Operacoes e cria uma LSD com essas operacoes em ordem de insercao
        T_Final = clock();
        T_Total = T_Final - T_Inicial;
        TrataOperacoes(argv[2], &ListaOp);
        // Executa as operacoes da Lista de Operacoes
        // E grava no arquivo de saida as saidas dessas operacoes
        ExecutaOperacoes(argv[3], &ListaOp, &Tweets, &UsuariosPorTweet, &UsuariosPorRT, &UsuariosPorEngajamento, &UsuariosPorMencao, &Hashtags, &pt_hashtags, T_Total);
    }
    return 0;
}


