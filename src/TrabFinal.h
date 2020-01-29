
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#define TAM_USER 25
#define TAM_HASH 52
#define TAM_TEXT 282
#define LINHA 400
#define ARQOP 60
#define OP 4

/// ============================== Estrutura dos Dados ==============================

// Lista das operacoes
typedef struct{
    int NumOp;
    char Hashtag[TAM_HASH];
    char CodOp[OP];
}INFO_LISTA_OP;

typedef struct{
    INFO_LISTA_OP InfoOp;
    struct LISTA_OP *prox;
}LISTA_OP;

typedef struct{
    int count;
    char mencao[TAM_HASH];
}INFO_MENCAO;

// Struct de Hashtags
// Obtidas do text da struct TWEET - eh uma AVL
// Org. pela quantidade de vezes que a hashtag foi usada
typedef struct{
    INFO_MENCAO Info_Mencao;
    int FB;
    struct PT_MENCAO *esq;
    struct PT_MENCAO *dir;
}PT_MENCAO;

// Lista de Mencao
typedef struct{
    INFO_MENCAO InfoLista;
    struct PT_LISTA_MENCAO *prox;
}PT_LISTA_MENCAO;

typedef struct{
    int count;  // Quantidade de vezes que foi add essa hashtag na AVL - que aparece nos tweets
    char texto_tweet[TAM_TEXT];
    char hashtag[TAM_HASH]; // Texto da hashtag
}INFO_HASHTAG;
// Struct de Hashtags
// Obtidas do text da struct TWEET - eh uma AVL
// Org. pela quantidade de vezes que a hashtag foi usada
typedef struct{
    INFO_HASHTAG Info_Hashtag;
    int FB;
    struct HASHTAG *esq;
    struct HASHTAG *dir;
}PT_HASHTAG;

typedef struct{
    INFO_HASHTAG InfoLista;
    struct PT_LISTA_HASHTAG *prox;
    struct PT_LISTA_HASHTAG *ant;
}PT_LISTA_HASHTAG;

typedef struct{
    int tweets_count;
    int retweets;
    int mencoes;
    int likes;
    int engajamento;
    char nome[TAM_USER];
}INFO_USER;

// Struct dos usuarios
// Org. pela quantidade de vezes que ela tweetou
typedef struct{
    INFO_USER Info_User;
    int FB;
    struct USER *esq;
    struct USER *dir;
}PT_USER;

struct PT_LISTA_USER{
    INFO_USER InfoLista;
    struct PT_LISTA_USER *prox;
};
typedef struct PT_LISTA_USER PT_LISTA_USER;

typedef struct{
    char text[TAM_TEXT];
    char user[TAM_USER];
    int RT_count;
    int favorite_count;
}INFO_TWEET;
// Struct dos TWEETS
typedef struct{
    INFO_TWEET Info_Tweet;
    int FB;
    struct TWEET *esq;
    struct TWEET *dir;
}PT_TWEET;

struct PT_LISTA_TWEET{
    INFO_TWEET InfoLista;
    struct PT_LISTA_TWEET *prox;
};
typedef struct PT_LISTA_TWEET PT_LISTA_TWEET;

/// ******************************************************************************************
/// =================================| Operacoes dos TWEETS: |================================
/// ******************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_TWEET* RD_Tweet(PT_TWEET* p);
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_TWEET* RE_Tweet(PT_TWEET* p);
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_TWEET* RDD_Tweet(PT_TWEET* p);
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_TWEET* RDE_Tweet(PT_TWEET* p);
// Organiza a Lista de Tweets em ordem alfabetica
PT_LISTA_TWEET* OrganizaListaTweet(PT_LISTA_TWEET *p, INFO_TWEET dado);
// Insere o Tweet em ordem crescente numa lista simplismente encadeada
PT_LISTA_TWEET* InsereListaTweet(PT_LISTA_TWEET *p, INFO_TWEET dado);
// Insere no em uma arvore AVL
PT_TWEET* InsereTweet(PT_TWEET *a, INFO_TWEET dado, int *ok);
// Tratamento de insercao a direita
PT_TWEET* Caso1_Tweet(PT_TWEET *a, int *ok);
// Tratamento de insercao a esquerda
PT_TWEET* Caso2_Tweet(PT_TWEET *a, int *ok);
// Responsavel por retirar o conteudo do arquivo e adiciona-las na estrutura Tweet
// Depois armazena-las na AVL de Tweets
void AtribuiTweet(PT_TWEET **Tweet);
// Imprime AVL de Tweets em ordem decrescente
void CentralDirTweet(PT_TWEET *a);

/// ******************************************************************************************
/// =================================| Operacoes dos USERS: |=================================
/// ******************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_USER* RD_User(PT_USER* p);
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_USER* RE_User(PT_USER* p);
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_USER* RDD_User(PT_USER* p);
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_USER* RDE_User(PT_USER* p);
//Organiza uma Lista de Usuarios em ordem alfabetica
PT_LISTA_USER* OrganizaListaUser(PT_LISTA_USER *p, INFO_USER dado);
// Insere o User em ordem crescente numa lista duplamente encadeada
PT_LISTA_USER* InsereListaUser(PT_LISTA_USER *p, INFO_USER dado);
// Insere um no (Usuario) em uma arvore AVL organizada por quantidade de Tweets
PT_USER* InsereUserTweet(PT_USER *a, PT_LISTA_USER *Usuarios, int Ordem, int *ok);
// Insere na arvore AVL de Usuarios pela quantidade de RT que seus Tweets tiveram
PT_USER* InsereUserPorRT(PT_USER *a, PT_LISTA_USER *Usuarios, int *ok);
// Imprime AVL de usuarios em ordem decrescente
void CentralDirUser(PT_USER *a);
// Tratamento de insercao a direita da AVL de USER
PT_USER* Caso1_User(PT_USER *a, int *ok);
//Tratamento de insercao a esquerda da AVL de USER
PT_USER* Caso2_User(PT_USER *a, int *ok);
// Funcao responsavel por retirar os Usuarios do arquivo e adiciona-los na Lista de usuarios
// Depois adicionar os usuarios da Lista na arvore
void AtribuiUser(PT_LISTA_USER **ListaUser);
// Insere um no (Usuario) em uma Lista duplamente encadeada
PT_LISTA_USER* InsereListaUser(PT_LISTA_USER *a, INFO_USER info);
// Auxiliar da InsereListaUser
int ConsultaListaUser(PT_LISTA_USER* a, INFO_USER dado);
// Imprime Lista de usuarios
void PrintaListaUser(PT_LISTA_USER *a);
// Impressao pre-fixada a esquerda informando o nivel do elemento
void PreFixadoEsq(PT_USER *a, int nivel);

/// *********************************************************************************************
/// =================================| Operacoes das Hashtags: |=================================
/// *********************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_HASHTAG* RD_Hashtag(PT_HASHTAG* p);
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_HASHTAG* RE_Hashtag(PT_HASHTAG* p);
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_HASHTAG* RDD_Hashtag(PT_HASHTAG* p);
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_HASHTAG* RDE_Hashtag(PT_HASHTAG* p);
// Organiza a Lista de Hastag em ordem alfabetica
PT_LISTA_HASHTAG* OrganizaListaHashtag(PT_LISTA_HASHTAG *p, INFO_HASHTAG dado);
// Cria uma Lista de usuarios mencionados nos Tweets
PT_LISTA_HASHTAG* InsereListaHashtag(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado);
// Cria uma Lista de hashtags utilizadas nos Tweets sem comparar se estao na Lista
// Esta lista sera util na funcao "Operacao G"
PT_LISTA_HASHTAG* InsereListaHashtag2(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado);
// Insere no em uma arvore AVL
PT_HASHTAG* InsereHashtag(PT_HASHTAG *a, PT_LISTA_HASHTAG *Hashtags, int *ok);
// Tratamento de insercao a direita
PT_HASHTAG* Caso1_Hashtag(PT_HASHTAG *a, int *ok);
// Tratamento de insercao a esquerda
PT_HASHTAG* Caso2_Hashtag(PT_HASHTAG *a, int *ok);
// Insere na Lista de Hashtags
PT_LISTA_HASHTAG* InsereListaHashtag(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado);
// Cria uma Lista de hashtags utilizadas nos Tweets sem comparar se estao na Lista
// Esta lista sera util na funcao "Operacao G"
PT_LISTA_HASHTAG* InsereListaHashtag2(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado);
// Consula a lista de hashtags (Auxiliar da InsereListaHashtag
int ConsultaListaHashatg(PT_LISTA_HASHTAG* a, INFO_HASHTAG dado);
// Printa a lista de Hashtags
void PrintaListaHashtag(PT_LISTA_HASHTAG *a);
// Retira as Hashtags do texto do Tweet e adiciona na Lista de Hashtags
PT_LISTA_HASHTAG* PegaHashtag(PT_LISTA_HASHTAG* ListaHashtag, PT_LISTA_HASHTAG** pt_lista_hashtag, char *TextoTweet);
// Imprime AVL de usuarios em ordem decrescente
void CentralDirHashtag(PT_HASHTAG *a);

/// *****************************************************************************************
/// |====================| Operacoes da Lista de Usuarios mencionados: |====================|
/// *****************************************************************************************

// Tratamento de insercao a esquerda
PT_MENCAO* Caso2_Mencao(PT_MENCAO *a, int *ok);
// Tratamento de insercao a direita
PT_MENCAO* Caso1_Mencao(PT_MENCAO *a, int *ok);
// Retira os usuarios mencionados nos Tweets
PT_LISTA_MENCAO* PegaMencao(PT_LISTA_MENCAO *ListaMencao, char *TextoTweet);
// Organiza a Lista de Usuarios mencionados por qtd de mencao nos Tweets
PT_LISTA_MENCAO* OrganizaListaMencao(PT_LISTA_MENCAO *p, INFO_MENCAO dado);
// Cria uma Lista de usuarios mencionados nos Tweets
PT_LISTA_MENCAO* InsereListaMencao(PT_LISTA_MENCAO *a, INFO_MENCAO dado);
// Auxiliar da funcao AtribuiStructs: Verifica qual usuario consta na Lista de Mencao
// Retorna 1 caso afirmativo, senao 0
int VerificaListaMencao(PT_LISTA_MENCAO *p, char *UsuarioTestado);
// Auxiliar da funcao AtribuiStructs: Verifica qual usuario consta na Lista de Mencao
// Retorna 1 caso afirmativo, senao 0
// Insere na arvore AVL de Usuarios pela quantidade de mencoes
PT_MENCAO* InsereUserPorMencao(PT_MENCAO *a, PT_LISTA_MENCAO *UserMencionados, int *ok);
// Printa lista de mencionados
void PrintListaMencao(PT_LISTA_MENCAO* p);
/// *****************************************************************************************
/// |========================| Funcoes de tratamento dos arquivos: |========================|
/// *****************************************************************************************

// Funcao responsavel por processar os dados (TWEETS) e criar todas as estruturas utilizadas pelas operacoes
// Recebe os Ponteiros das AVL's usadas por referencia e o nome do arquivo de entrada com os dados
void AtribuiStructs(char *ArqEntrada, PT_TWEET **Tweet, PT_USER **UsuariosPorTweets, PT_USER **UsuariosPorRT, PT_USER **UsuariosPorEngajamento, PT_MENCAO **UsuariosPorMencao, PT_HASHTAG **Hashtags, PT_LISTA_HASHTAG **pt_lista_hastags);
// Abre o arquivo de Operacoes e add eles em uma Lista de Operacoes por ordem de leitura do arquivo de Operacoes
void TrataOperacoes(char *ArqOperacoes, LISTA_OP **ListaOp);
// Executa as operacoes contida na Lista de Operacoes;
// Escreve gera oa rquivo de saida da seguinte forma:
// Identifica a operacao � ser realizada, e o numero do elementos que devem ser inseridos no arquivo de saida
// Abre e escreve no arquivo de saida. Fecha o arquivo e identifica a proxima para ser executada.
// (A Lista de Operacoes contem o codigo da funcao a ser executada e o numero de elementos a serem add no arquivo de saida)
void ExecutaOperacoes(char *ArqSaida, LISTA_OP **ListaOp, PT_TWEET **Tweet, PT_USER **UsuariosPorTweets, PT_USER **UsuariosPorRT, PT_USER **UsuariosPorEngajamento, PT_MENCAO **UsuariosPorMencao, PT_HASHTAG **Hashtags, PT_LISTA_HASHTAG **pt_lista_hastags, clock_t TempoGeraStr);
// Auxiliar da TrataOperacoes - Insere na Lista de Operacoes
LISTA_OP* InsereListaOp(LISTA_OP* p, INFO_LISTA_OP Dados);
// Printa a lista de operacoes
void PrintaListaOp(LISTA_OP* p);

/// *****************************************************************************************
/// |================================| Funcoes auxiliares: |================================|
/// *****************************************************************************************

// Remove os acentos de um string
// Usada na funcao AtribuiStructs para retirar os acentos dos Tweets
void RemoveAcentos (char s[]);
// Compara dois Strings passados por parametro
// Se forem iguais retorna -1
// Se retornar 0 = o str1 vem primeiro na ordem alfabetica
// Se retornar 1 = o str2 vem primeiro na ordem alfabetica
int ComparaStrings(char *str1, char *str2);

/// *********************************************************************************************
/// ========================================| OPERACOES: |=======================================
/// *********************************************************************************************

// Operacao A:
void a(PT_HASHTAG* Hashtags, int QuantLista, int *count, FILE* Saida);
// Operacao B:
void b(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida);
// Operacao C:
void c(PT_TWEET* Tweet, int QuantLista, int *count, FILE* Saida);
// Operacao D:
void d(PT_MENCAO* Usuarios, int QuantLista, int *count, FILE* Saida);
// Operacao E:
void e(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida);
// Operacao F:
void f(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida);
// Operacao G:
void g(PT_LISTA_HASHTAG* pt_lista_hashtag, char *Hashtag, PT_HASHTAG **HashtagsAssociadas);
// Procura as Hashtags associada a hashtag passado por parametro pra funcao
// Funcao utlizada pelo funcao g (Operacao G)
PT_LISTA_HASHTAG* achaHashtag(char Hashtag[TAM_HASH], PT_LISTA_HASHTAG* pt_hashtag, PT_LISTA_HASHTAG* nova_lista);
// Funcao auxiliar da Operacao G
// Eh responsavel por gravar o resultado da Operacao G no arquivo de saida
void AuxGravaOperacaoG(PT_HASHTAG* HashtagsAssosciadas, int QuantLista, int *count, FILE* Saida);
