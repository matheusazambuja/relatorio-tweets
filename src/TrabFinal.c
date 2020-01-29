#include "TrabFinal.h"

/// *******************************************************************************************
/// ================================== Operacoes dos TWEETS: ==================================
/// *******************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_TWEET* RD_Tweet(PT_TWEET* p){
    PT_TWEET *u;

    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->FB = 0;
    p = u;
    return p;
}
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_TWEET* RE_Tweet(PT_TWEET* p){
    PT_TWEET *z;

    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->FB = 0;
    p = z;
    return p;
}
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_TWEET* RDD_Tweet(PT_TWEET* p){
    PT_TWEET *u, *v;

    u = p->esq;
    v = u->dir;

    if(!u || !v)
        return p;

    u->dir = v->esq;
    v->esq = u;
    p->esq = v->dir;
    v->dir = p;
    if(v->FB == 1)
        p->FB = -1;
    else
        p->FB = 0;
    if(v->FB == -1)
        u->FB = 1;
    else
        u->FB = 0;
    p = v;
    return p;
}
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_TWEET* RDE_Tweet(PT_TWEET* p){
    PT_TWEET *z, *y;

    z = p->dir;
    y = z->esq;

    if(!z || !y)
        return p;

    z->esq = y->dir;
    y->dir = z;
    p->dir = y->esq;
    y->esq = p;
    if(y->FB == -1)
        p->FB = 1;
    else
        p->FB = 0;
    if(y->FB == 1)
        z->FB = -1;
    else
        z->FB = 0;
    p = y;
    return p;
}
// Organiza a Lista de Tweets em ordem alfabetica
PT_LISTA_TWEET* OrganizaListaTweet(PT_LISTA_TWEET *p, INFO_TWEET dado){
    PT_LISTA_TWEET* Aux = p;
    PT_LISTA_TWEET* Anterior = NULL;
    PT_LISTA_TWEET* novo;
    int CmpString;

    novo = (PT_LISTA_TWEET*)malloc(sizeof(PT_LISTA_TWEET));
    novo->InfoLista = dado;

    if(Aux == NULL)
    {
        p = novo;
        novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL && ComparaStrings(Aux->InfoLista.text, novo->InfoLista.text) == 0){
            Anterior = Aux;
            Aux = Aux->prox;
        }

        if(Aux == NULL)
        {
            Anterior->prox = novo;
            novo->prox = NULL;
        }
        else
        {
            CmpString = ComparaStrings(Aux->InfoLista.text, novo->InfoLista.text);
            if(Anterior == NULL && CmpString != 0)
            {
                novo->prox = p;
                p = novo;
            }
            else if(Anterior != NULL)
            {
                Anterior->prox = novo;
                novo->prox = Aux;
            }
        }
    }
    return p;
}
// Insere o Tweet em ordem crescente numa lista simplismente encadeada
PT_LISTA_TWEET* InsereListaTweet(PT_LISTA_TWEET *p, INFO_TWEET dado){
    PT_LISTA_TWEET* Aux = p;
    PT_LISTA_TWEET* Anterior = NULL;
    PT_LISTA_TWEET* Novo;

    Novo = (PT_LISTA_TWEET*)malloc(sizeof(PT_LISTA_TWEET));
    Novo->InfoLista = dado;

    if(Aux == NULL)
    {
        p = Novo;
        Novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL && strcmp(Aux->InfoLista.text, Novo->InfoLista.text) != 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL)
        {
            Anterior->prox = Novo;
            Novo->prox = NULL;
        }
        else
        {
            Aux->InfoLista.favorite_count += dado.favorite_count;
            Aux->InfoLista.RT_count += dado.RT_count;
        }
    }
    return p;
}
// Insere no em uma arvore AVL
PT_TWEET* InsereTweet(PT_TWEET *a, INFO_TWEET dado, int *ok){
    if(a == NULL){
        a = (PT_TWEET*)malloc(sizeof(PT_TWEET));
        a->Info_Tweet = dado;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if(dado.RT_count < a->Info_Tweet.RT_count){
            a->esq = InsereTweet(a->esq, dado, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_Tweet(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereTweet(a->dir, dado, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_Tweet(a, ok);
                        break;
                }
            }
        }
        return a;
}
// Tratamento de insercao a direita
PT_TWEET* Caso1_Tweet(PT_TWEET *a, int *ok){
    PT_TWEET *z;

    z = a->esq;
    if(z->FB == 1)
        a = RD_Tweet(a);
    else
        a = RDD_Tweet(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Tratamento de insercao a esquerda
PT_TWEET* Caso2_Tweet(PT_TWEET *a, int *ok){
    PT_TWEET* z;

    z = a->dir;
    if(z->FB == -1)
        a = RE_Tweet(a);
    else
        a = RDE_Tweet(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Printa a lista de operacoes
void PrintaListaTweet(PT_LISTA_TWEET* p){
    PT_LISTA_TWEET* Aux = p;
    while(Aux != NULL)
    {
        printf("Tweet: %s, RT: %d, Like: %d, User: %s\n", Aux->InfoLista.text,
               Aux->InfoLista.RT_count, Aux->InfoLista.favorite_count, Aux->InfoLista.user);
        Aux = Aux->prox;
    }
}
// Imprime os Tweets em ordem decrescente por RT
void CentralDirTweet(PT_TWEET *a){
    if(a != NULL)
    {
        CentralDirTweet(a->dir);
        printf("Tweet: %s | RT: %d \n", a->Info_Tweet.text, a->Info_Tweet.RT_count);
        CentralDirTweet(a->esq);
    }
}
// Impressao pre-fixada a esquerda informando o nivel do elemento
void PreFixadoEsqTweet(PT_TWEET *a, int nivel){
    int cont = 0;
    if(a != NULL)
    {
        while(cont++ < nivel) //Calcula o nivel do elemento
            printf("=");
        printf("%s, RT: %d\n", a->Info_Tweet.text, a->Info_Tweet.RT_count);
        PreFixadoEsq(a->esq, nivel+1);
        PreFixadoEsq(a->dir, nivel+1);
    }
}

/// ******************************************************************************************
/// ================================== Operacoes dos USERS: ==================================
/// ******************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_USER* RD_User(PT_USER* p){
    PT_USER *u;

    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->FB = 0;
    p = u;
    return p;
}
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_USER* RE_User(PT_USER* p){
    PT_USER *z;

    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->FB = 0;
    p = z;
    return p;
}
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_USER* RDD_User(PT_USER* p){
    PT_USER *u, *v;

    u = p->esq;
    v = u->dir;

    if (!u || !v)
        return p;

    u->dir = v->esq;
    v->esq = u;
    p->esq = v->dir;
    v->dir = p;
    if(v->FB == 1)
        p->FB = -1;
    else
        p->FB = 0;
    if(v->FB == -1)
        u->FB = 1;
    else
        u->FB = 0;
    p = v;
    return p;
}
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_USER* RDE_User(PT_USER* p){
    PT_USER *z, *y;

    z = p->dir;
    y = z->esq;

    if (!z || !y)
        return p;

    z->esq = y->dir;
    y->dir = z;
    p->dir = y->esq;
    y->esq = p;
    if(y->FB == -1)
        p->FB = 1;
    else
        p->FB = 0;
    if(y->FB == 1)
        z->FB = -1;
    else
        z->FB = 0;
    p = y;
    return p;
}
//Organiza uma Lista de Usuarios em ordem alfabetica
PT_LISTA_USER* OrganizaListaUser(PT_LISTA_USER *p, INFO_USER dado){
    PT_LISTA_USER* Aux = p;
    PT_LISTA_USER* Anterior = NULL;
    PT_LISTA_USER* novo;
    int CmpString;

    novo = (PT_LISTA_USER*)malloc(sizeof(PT_LISTA_USER));
    novo->InfoLista = dado;

    if(p == NULL)
    {
        p = novo;
        novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL && ComparaStrings(Aux->InfoLista.nome, novo->InfoLista.nome) == 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL)
        {
            Anterior->prox = novo;
            novo->prox = NULL;
        }
        else
        {
            CmpString = ComparaStrings(Aux->InfoLista.nome, novo->InfoLista.nome);
            if(Anterior == NULL && CmpString != 0)
            {
                novo->prox = p;
                p = novo;
            }
            else
            {
                Anterior->prox = novo;
                novo->prox = Aux;
            }
        }
    }
    return p;
}
// Insere o User em ordem crescente numa lista duplamente encadeada
PT_LISTA_USER* InsereListaUser(PT_LISTA_USER *p, INFO_USER dado){
    PT_LISTA_USER* Aux = p;
    PT_LISTA_USER* Anterior = NULL;
    PT_LISTA_USER* Novo;

    Novo = malloc(sizeof(PT_LISTA_USER));
    Novo->InfoLista = dado;

    if(Aux == NULL)
    {
        Novo->InfoLista.tweets_count = 1;
        Novo->InfoLista.engajamento = 0;
        Novo->InfoLista.mencoes = 0;
        p = Novo;
        Novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL && strcmp(Aux->InfoLista.nome, Novo->InfoLista.nome) != 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL)
        {
            Anterior->prox = Novo;
            Novo->prox = NULL;
        }
        else
        {
            Aux->InfoLista.tweets_count++;
            Aux->InfoLista.likes += dado.likes;
            Aux->InfoLista.retweets += dado.retweets;
        }
    }
    return p;
}
// Printa a lista de usuarios
void PrintaListaUser(PT_LISTA_USER *a){
    while(a != NULL)
    {
        printf("Usuario: %s, Tweets: %d, Retweets: %d, Likes: %d, Mencoes: %d, Engajamento: %d \n",a->InfoLista.nome,
               a->InfoLista.tweets_count, a->InfoLista.retweets, a->InfoLista.likes, a->InfoLista.mencoes, a->InfoLista.engajamento);
        a = a->prox;
    }
}
// Insere na arvore AVL de Usuarios pela quantidade de Tweets feitos
PT_USER* InsereUserPorTweet(PT_USER *a, PT_LISTA_USER *Usuarios, int *ok){
    if(a == NULL){
        a = (PT_USER*)malloc(sizeof(PT_USER));
        a->Info_User = Usuarios->InfoLista;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if((Usuarios->InfoLista.tweets_count < a->Info_User.tweets_count)){
            a->esq = InsereUserPorTweet(a->esq, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_User(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereUserPorTweet(a->dir, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_User(a, ok);
                        break;
                }
            }
        }
    return a;
}
// Insere na arvore AVL de Usuarios pela quantidade de Engajamento que tiveram na rede
PT_USER* InsereUserPorRT(PT_USER *a, PT_LISTA_USER *Usuarios, int *ok){
    if(a == NULL){
        a = (PT_USER*)malloc(sizeof(PT_USER));
        a->Info_User = Usuarios->InfoLista;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if((Usuarios->InfoLista.retweets < a->Info_User.retweets)){
            a->esq = InsereUserPorRT(a->esq, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_User(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereUserPorRT(a->dir, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_User(a, ok);
                        break;
                    }
                }
            }
    return a;
}
// Insere na arvore AVL de Usuarios pela quantidade de Engajamento que tiveram na rede
PT_USER* InsereUserPorEngajamento(PT_USER *a, PT_LISTA_USER *Usuarios, int *ok){
    if(a == NULL){
        a = (PT_USER*)malloc(sizeof(PT_USER));
        a->Info_User = Usuarios->InfoLista;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if((Usuarios->InfoLista.engajamento < a->Info_User.engajamento)){
            a->esq = InsereUserPorEngajamento(a->esq, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_User(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereUserPorEngajamento(a->dir, Usuarios, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_User(a, ok);
                        break;
                }
            }
        }
    return a;
}
// Tratamento de insercao a direita
PT_USER* Caso1_User(PT_USER *a, int *ok){
    PT_USER *z;

    z = a->esq;
    if(z->FB == 1)
        a = RD_User(a);
    else
        a = RDD_User(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Tratamento de insercao a esquerda
PT_USER* Caso2_User(PT_USER *a, int *ok){
    PT_USER* z;

    z = a->dir;
    if(z->FB == -1)
        a = RE_User(a);
    else
        a = RDE_User(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Impressao central a direita
void CentralDirUser(PT_USER *a){
    if(a != NULL)
    {
        CentralDirUser(a->dir);
        printf("Usuario: %s, Tweets: %d, Retweets: %d, Likes: %d, Mencoes: %d, Engajamento: %d\n",a->Info_User.nome,
            a->Info_User.tweets_count, a->Info_User.retweets, a->Info_User.likes, a->Info_User.mencoes, a->Info_User.engajamento);
        CentralDirUser(a->esq);
    }
}

/// *****************************************************************************************
/// |===============================| Operacoes de HASHTAG: |===============================|
/// *****************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_HASHTAG* RD_Hashtag(PT_HASHTAG* p){
    PT_HASHTAG *u;

    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->FB = 0;
    p = u;
    return p;
}
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_HASHTAG* RE_Hashtag(PT_HASHTAG* p){
    PT_HASHTAG *z;

    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->FB = 0;
    p = z;
    return p;
}
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_HASHTAG* RDD_Hashtag(PT_HASHTAG* p){
    PT_HASHTAG *u, *v;

    u = p->esq;
    v = u->dir;
    u->dir = v->esq;
    v->esq = u;
    p->esq = v->dir;
    v->dir = p;
    if(v->FB == 1)
        p->FB = -1;
    else
        p->FB = 0;
    if(v->FB == -1)
        u->FB = 1;
    else
        u->FB = 0;
    p = v;
    return p;
}
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_HASHTAG* RDE_Hashtag(PT_HASHTAG* p){
    PT_HASHTAG *z, *y;

    z = p->dir;
    y = z->esq;
    z->esq = y->dir;
    y->dir = z;
    p->dir = y->esq;
    y->esq = p;
    if(y->FB == -1)
        p->FB = 1;
    else
        p->FB = 0;
    if(y->FB == 1)
        z->FB = -1;
    else
        z->FB = 0;
    p = y;
    return p;
}
// Organiza a uma LSE de Hastag em ordem alfabetica
PT_LISTA_HASHTAG* OrganizaListaHashtag(PT_LISTA_HASHTAG *p, INFO_HASHTAG dado){
    PT_LISTA_HASHTAG* Aux = p;
    PT_LISTA_HASHTAG* Anterior = NULL;
    PT_LISTA_HASHTAG* novo;
    int CmpString;

    novo = (PT_LISTA_HASHTAG*)malloc(sizeof(PT_LISTA_HASHTAG));
    novo->InfoLista = dado;

    if(Aux == NULL)
    {
        p = novo;
        novo->prox = NULL;
    }
    else
    {
        // CmpString = ComparaStrings(Aux->InfoLista.nome, novo->InfoLista.nome);
        while(Aux != NULL && ComparaStrings(Aux->InfoLista.hashtag, novo->InfoLista.hashtag) == 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL) // Ultimo elemento
        {
            Anterior->prox = novo;
            novo->prox = NULL;
        }
        else
        {
            CmpString = ComparaStrings(Aux->InfoLista.hashtag, novo->InfoLista.hashtag);
            if(Anterior == NULL && (CmpString == 1 || CmpString == -1)) // Primeiro elemento
            {
                novo->prox = p;
                p = novo;
            }
            else// Elemento no meio da lista
            {
                Anterior->prox = novo;
                novo->prox = Aux;
            }
        }
    }
    return p;
}
// Passa a Lista de Hashtag para add no final dela
int PassaListaHashtag(PT_LISTA_HASHTAG* a){
    if(a == NULL)
        return 0;
    else
        PassaListaHashtag(a->ant);
}
// Cria uma Lista de hashtags utilizadas nos Tweets sem comparar se estao na Lista
// Esta lista sera util na funcao "Operacao G"
PT_LISTA_HASHTAG* InsereListaHashtag2(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado){
    PT_LISTA_HASHTAG* novo;

    novo = (PT_LISTA_HASHTAG*)malloc(sizeof(PT_LISTA_HASHTAG));
    novo->InfoLista = dado;

    PT_LISTA_HASHTAG* anterior = a;
    PT_LISTA_HASHTAG *comeco_lista = novo;

    if(a == NULL){
        novo->InfoLista.count = 1;
        novo->ant = NULL;
        novo->prox = NULL;
        comeco_lista = novo;
    }
    else{
        if(PassaListaHashtag(anterior) == 0){
            anterior->prox = novo;
            novo->ant = anterior;
            novo->prox = NULL;
        }
        else
            comeco_lista = a;
    }
    return comeco_lista;
}
// Cria uma Lista de usuarios mencionados nos Tweets
PT_LISTA_HASHTAG* InsereListaHashtag(PT_LISTA_HASHTAG *a, INFO_HASHTAG dado){
    PT_LISTA_HASHTAG* novo;

    novo = (PT_LISTA_HASHTAG*)malloc(sizeof(PT_LISTA_HASHTAG));
    novo->InfoLista = dado;

    PT_LISTA_HASHTAG* anterior = a;
    PT_LISTA_HASHTAG *comeco_lista = novo;

    if(a == NULL){
        novo->InfoLista.count = 1;
        novo->ant = NULL;
        novo->prox = NULL;
        comeco_lista = novo;
    }
    else{
        if(ConsultaListaHashtag(anterior, dado) == 0){
            anterior->prox = novo;
            novo->ant = anterior;
            novo->prox = NULL;
        }
        else
            comeco_lista = a;
    }
    return comeco_lista;
}
// Auxiliar da funcao InsereListaMencao: Verifica se o usuario ja connta na Lista de Mencao
// Retorna 1 caso afirmativo, senao 0
int ConsultaListaHashtag(PT_LISTA_HASHTAG* a, INFO_HASHTAG dado){
    if(a == NULL)
        return 0;
    else{
        if(strcmp(a->InfoLista.hashtag, dado.hashtag) == 0){
            a->InfoLista.count++;
            return 1;
        }
        else
            ConsultaListaHashtag(a->ant, dado);
    }
}
// Printa a lista de Hashtags
void PrintaListaHashtag(PT_LISTA_HASHTAG *a){
    while(a != NULL){
        printf("Hashtag: %s, Count: %d\n", a->InfoLista.hashtag, a->InfoLista.count);
        a = a->ant;
    }
}
// Insere na arvore AVL de Usuarios
PT_HASHTAG* InsereHashtag(PT_HASHTAG *a, PT_LISTA_HASHTAG *Hashtags, int *ok){
    INFO_HASHTAG InfoAux;
    int i;
    if(a == NULL){
        a = (PT_HASHTAG*)malloc(sizeof(PT_HASHTAG));
        a->Info_Hashtag = Hashtags->InfoLista;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if((Hashtags->InfoLista.count < a->Info_Hashtag.count)){
            a->esq = InsereHashtag(a->esq, Hashtags, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_Hashtag(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereHashtag(a->dir, Hashtags, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_Hashtag(a, ok);
                        break;
                }
            }
        }
        return a;
}
// Tratamento de insercao a direita
PT_HASHTAG* Caso1_Hashtag(PT_HASHTAG *a, int *ok){
    PT_HASHTAG *z;

    z = a->esq;
    if(z->FB == 1)
        a = RD_Hashtag(a);
    else
        a = RDD_Hashtag(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Tratamento de insercao a esquerda
PT_HASHTAG* Caso2_Hashtag(PT_HASHTAG *a, int *ok){
    PT_HASHTAG* z;

    z = a->dir;
    if(z->FB == -1)
        a = RE_Hashtag(a);
    else
        a = RDE_Hashtag(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Retira as Hashtags utilizadas nos Tweets
PT_LISTA_HASHTAG* PegaHashtag(PT_LISTA_HASHTAG* ListaHashtag, PT_LISTA_HASHTAG** pt_lista_hashtag, char *TextoTweet){
    INFO_HASHTAG Novo;
    char Aux[TAM_TEXT];
    int i = 0, j = 0;

    strcpy(Aux, TextoTweet);

    while(Aux[i] != '\0')
    {
        if(Aux[i] == '#')
        {  // pq preciso testar o '\0'?
            while(Aux[i] != ' ' && Aux[i] != '/' && Aux[i] != ';' && Aux[i] != '"' && Aux[i] != '!' && Aux[i] != '?' && Aux[i] != '.'  && Aux[i] != ',' && Aux[i] != '\0')
            {
                Novo.hashtag[j] = Aux[i];
                j++;
                i++;
            }
            Novo.hashtag[j] = '\0';
            Novo.count = 1;
            strcpy(Novo.texto_tweet, TextoTweet);
            ListaHashtag = InsereListaHashtag(ListaHashtag, Novo);
            *pt_lista_hashtag = InsereListaHashtag2(*pt_lista_hashtag, Novo);

            for(j; j>=0; j--)
                Novo.hashtag[j] = 0;

            j = 0;
        }
        if(Aux[i] == '\0')
            i--;
        i++;
    }
    return ListaHashtag;
}
// Printa os Hashtags em ordem decrescente por quantidade de citacoes
void CentralDirHashtag(PT_HASHTAG *a){
    if(a != NULL)
    {
        CentralDirHashtag(a->dir);
        printf("Hashtag: %s, Count: %d\n",a->Info_Hashtag.hashtag, a->Info_Hashtag.count);
        CentralDirHashtag(a->esq);
    }
}
// Retira os usuarios mencionados dos Tweets
// Impressao pre-fixada a esquerda informando o nivel do elemento
void PreFixadoEsq(PT_USER *a, int nivel){
    int cont = 0;
    if(a != NULL)
    {
        while(cont++ < nivel) //Calcula o nivel do elemento
            printf("=");
        printf("%s, %d\n", a->Info_User.nome, a->Info_User.engajamento);
        PreFixadoEsq(a->esq, nivel+1);
        PreFixadoEsq(a->dir, nivel+1);
    }
}

/// *****************************************************************************************
/// |====================| Operacoes da Lista de Usuarios mencionados: |====================|
/// *****************************************************************************************

// Subarvore com fator positivo e a sua subarvore esquerda tambem tem fator positivo > |1|
PT_MENCAO* RD_Mencao(PT_MENCAO* p){
    PT_MENCAO *u;

    u = p->esq;
    p->esq = u->dir;
    u->dir = p;
    p->FB = 0;
    p = u;
    return p;
}
// Subarvore com fator negativo e a sua subarvore direita tambem tem fator negativo > |1|
PT_MENCAO* RE_Mencao(PT_MENCAO* p){
    PT_MENCAO *z;

    z = p->dir;
    p->dir = z->esq;
    z->esq = p;
    p->FB = 0;
    p = z;
    return p;
}
// Sempre que uma subarvore ficar com um fator positivo:
//  * E sua subarvore esquerda tem fator negativo;
PT_MENCAO* RDD_Mencao(PT_MENCAO* p){
    PT_MENCAO *u, *v;

    u = p->esq;
    v = u->dir;
    u->dir = v->esq;
    v->esq = u;
    p->esq = v->dir;
    v->dir = p;
    if(v->FB == 1)
        p->FB = -1;
    else
        p->FB = 0;
    if(v->FB == -1)
        u->FB = 1;
    else
        u->FB = 0;
    p = v;
    return p;
}
// Sempre que um subarvore ficar com um fator negativo:
//  E sua subarvore direita tem um fator positivo;
PT_MENCAO* RDE_Mencao(PT_MENCAO* p){
    PT_MENCAO *z, *y;

    z = p->dir;
    y = z->esq;
    z->esq = y->dir;
    y->dir = z;
    p->dir = y->esq;
    y->esq = p;
    if(y->FB == -1)
        p->FB = 1;
    else
        p->FB = 0;
    if(y->FB == 1)
        z->FB = -1;
    else
        z->FB = 0;
    p = y;
    return p;
}
// Organiza a Lista de Usuarios mencionados por qtd de mencao nos Tweets
PT_LISTA_MENCAO* OrganizaListaMencao(PT_LISTA_MENCAO *p, INFO_MENCAO dado){
    PT_LISTA_MENCAO* Aux = p;
    PT_LISTA_MENCAO* Anterior = NULL;
    PT_LISTA_MENCAO* novo;
    int CmpString;

    novo = malloc(sizeof(PT_LISTA_MENCAO));
    novo->InfoLista = dado;

    if(Aux == NULL)
    {
        p = novo;
        novo->prox = NULL;
    }
    else
    {
        // CmpString = ComparaStrings(Aux->InfoLista.nome, novo->InfoLista.nome);
        while(Aux != NULL && ComparaStrings(Aux->InfoLista.mencao, novo->InfoLista.mencao) == 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL) // Ultimo elemento
        {
            Anterior->prox = novo;
            novo->prox = NULL;
        }
        else
        {
            CmpString = ComparaStrings(Aux->InfoLista.mencao, novo->InfoLista.mencao);
            if(Anterior == NULL && (CmpString == 1 || CmpString == -1)) // Primeiro elemento
            {
                novo->prox = p;
                p = novo;
            }
            else// Elemento no meio da lista
            {
                Anterior->prox = novo;
                novo->prox = Aux;
            }
        }
    }
    return p;
}
// Retira os usuarios mencionados nos Tweets
PT_LISTA_MENCAO* PegaMencao(PT_LISTA_MENCAO *ListaMencao, char *TextoTweet){
    INFO_MENCAO Novo;
    char Aux[TAM_TEXT];
    int i = 0, j = 0;

    strcpy(Aux, TextoTweet);

    while(Aux[i] != '\0')
    {
        if(Aux[i] == '@')
        {
            while(Aux[i] != ' ' && Aux[i] != '/' && Aux[i] != ';' && Aux[i] != '"' && Aux[i] != '?' && Aux[i] != '!' && Aux[i] != '.' && Aux[i] != ',' && Aux[i] != '\0')
            {
                Novo.mencao[j] = Aux[i];
                j++;
                i++;
            }
            Novo.mencao[j] = '\0';
            Novo.count = 1;
            ListaMencao = InsereListaMencao(ListaMencao, Novo);

            for(j; j>=0; j--)
                Novo.mencao[j] = 0;
            j = 0;
        }
        if(Aux[i] == '\0')
            i--;
        i++;
    }
    return ListaMencao;
}
// Cria uma Lista de usuarios mencionados nos Tweets
PT_LISTA_MENCAO* InsereListaMencao(PT_LISTA_MENCAO *p, INFO_MENCAO dado){
    PT_LISTA_MENCAO* Aux = p;
    PT_LISTA_MENCAO* Anterior = NULL;
    PT_LISTA_MENCAO* Novo;

    Novo = malloc(sizeof(PT_LISTA_MENCAO));
    Novo->InfoLista = dado;

    if(Aux == NULL)
    {
        p = Novo;
        Novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL && strcmp(Aux->InfoLista.mencao, Novo->InfoLista.mencao) != 0)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        if(Aux == NULL)
        {
            Anterior->prox = Novo;
            Novo->prox = NULL;
        }
        else
            Aux->InfoLista.count++;
    }
    return p;
}
// Auxiliar da funcao AtribuiStructs: Verifica qual usuario consta na Lista de Mencao
// Retorna 1 caso afirmativo, senao 0
// Insere na arvore AVL de Usuarios pela quantidade de mencoes
PT_MENCAO* InsereUserPorMencao(PT_MENCAO *a, PT_LISTA_MENCAO *UserMencionados, int *ok){
    if(a == NULL){
        a = (PT_MENCAO*)malloc(sizeof(PT_MENCAO));
        a->Info_Mencao = UserMencionados->InfoLista;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        ok = 1;
    }
    else if((UserMencionados->InfoLista.count < a->Info_Mencao.count)){
            a->esq = InsereUserPorMencao(a->esq, UserMencionados, ok);
            if(ok){
                switch(a->FB){
                    case -1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = 1;
                        break;
                    case 1:
                        a = Caso1_Mencao(a, ok);
                        break;
                }
            }
        }
        else{
            a->dir = InsereUserPorMencao(a->dir, UserMencionados, ok);
            if(ok){
                switch(a->FB){
                    case 1:
                        a->FB = 0;
                        ok = 0;
                        break;
                    case 0:
                        a->FB = -1;
                        break;
                    case -1:
                        a = Caso2_Mencao(a, ok);
                        break;
                }
            }
        }
    return a;
}
// Tratamento de insercao a direita
PT_MENCAO* Caso1_Mencao(PT_MENCAO *a, int *ok){
    PT_MENCAO *z;

    z = a->esq;
    if(z->FB == 1)
        a = RD_Mencao(a);
    else
        a = RDD_Mencao(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Tratamento de insercao a esquerda
PT_MENCAO* Caso2_Mencao(PT_MENCAO *a, int *ok){
    PT_MENCAO* z;

    z = a->dir;
    if(z->FB == -1)
        a = RE_Mencao(a);
    else
        a = RDE_Mencao(a);

    a->FB = 0;
    ok = 0;
    return a;
}
// Retira as Hashtags utilizadas nos Tweets
int VerificaListaMencao(PT_LISTA_MENCAO *p, char *UsuarioTestado){
    PT_LISTA_MENCAO *Aux = p;
    if(Aux == NULL)
        return 0;
    else
    {
        while(Aux != NULL){
            if(strcmp(Aux->InfoLista.mencao, UsuarioTestado) == 0)
                return Aux->InfoLista.count;
            Aux = Aux->prox;
        }
        if(Aux == NULL)
            return 0;
    }
}
// Printa lista de mencionados
void PrintListaMencao(PT_LISTA_MENCAO* p){
    PT_LISTA_MENCAO* Aux = p;
    while(Aux != NULL)
    {
        printf("User: %s, qtd: %d \n", Aux->InfoLista.mencao, Aux->InfoLista.count);
        Aux = Aux->prox;
    }
}

/// *****************************************************************************************
/// |========================| Funcoes de Tratamento dos Arquivos: |========================|
/// *****************************************************************************************

// Funcao responsavel por processar os dados (TWEETS) e criar todas as estruturas utilizadas pelas operacoes
// Recebe os Ponteiros das AVL's usadas por referencia e o nome do arquivo de entrada com os dados
void AtribuiStructs(char *ArqEntrada, PT_TWEET **Tweet, PT_USER **UsuariosPorTweets, PT_USER **UsuariosPorRT, PT_USER **UsuariosPorEngajamento, PT_MENCAO **UsuariosPorMencao, PT_HASHTAG **Hashtags, PT_LISTA_HASHTAG **pt_lista_hastags){
    char Linha[LINHA];
    char Usuario[TAM_USER];
    char TextoTweet[TAM_TEXT];
    char RT[15];
    char Like[15];
    int *ok = 1; // Utilizado para fazer o balanceamento das AVL
    int RetornoMencao; // Utilizado pela funcao que atualiza as mencoes de usuarios que Tweetaram

    // Listas e Estuturas de informacoes utilizadas pela funcao
    INFO_USER InfoUser;
    INFO_TWEET InfoTweet;
    INFO_HASHTAG InfoHashtag;

    PT_LISTA_USER* ListaUser = NULL;
    PT_LISTA_USER* ListaUserInsere = NULL;
    PT_LISTA_USER* ListaUserAlf = NULL;

    PT_LISTA_HASHTAG *ListaHashtag = NULL;
    PT_LISTA_HASHTAG* ListaHashtagAlf = NULL;

    PT_LISTA_MENCAO* ListaMencao = NULL;
    PT_LISTA_MENCAO* ListaMencaoInsere = NULL;
    PT_LISTA_MENCAO* ListaMencaoAlf = NULL;

    PT_LISTA_TWEET* ListaTweet = NULL;
    PT_LISTA_TWEET* ListaTweetAlf = NULL;
    PT_TWEET *TweetAux = NULL;

    FILE *arquivo = fopen(ArqEntrada, "r");

    if(!arquivo)
        printf("erro na abertura do arquivo para atribuicao das estruturas");
    else{
        while(!feof(arquivo)){ // Trata o arquivo de entrada dos Tweets
            fgets(Linha, LINHA, arquivo);
            strcpy(Usuario, strtok(Linha, ";"));

            strcpy(InfoUser.nome, Usuario);
            strcpy(InfoTweet.user, Usuario);

            strcpy(TextoTweet, strtok(NULL, ";"));
            RemoveAcentos(TextoTweet);

            strcpy(InfoTweet.text, TextoTweet);
            strcpy(InfoHashtag.texto_tweet, TextoTweet);

            // Funcoes responsaveis por contabilizar o numero de mencoes e hashtags dos Tweets
            // Adiciona eles em uma Lista de Hashtag e Usuarios Mencionados
            ListaHashtag = PegaHashtag(ListaHashtag, *(&pt_lista_hastags), TextoTweet);
            ListaMencao = PegaMencao(ListaMencao, TextoTweet);

            strcpy(RT, strtok(NULL, ";"));
            InfoUser.retweets = atoi(RT);
            InfoTweet.RT_count = atoi(RT);

            strcpy(Like, strtok(NULL, ";"));
            InfoUser.likes = atoi(Like);
            InfoTweet.favorite_count = atoi(Like);

            InfoUser.tweets_count = 1;
            InfoUser.mencoes = 0;
            ListaUser = InsereListaUser(ListaUser, InfoUser);
            ListaTweet = InsereListaTweet(ListaTweet, InfoTweet);
        }
        fclose(arquivo); // Fecha o arquivo de entrada dos Tweets
        /// ===================== Realiza att das Listas =====================
        /// Ajustas as ultimas informacoes das Listas para adiciona-las nas arvores AVL

        // Contabiliza a qtd de mencoes que o usuario teve
        ListaMencaoInsere = ListaMencao;
        ListaUserInsere = ListaUser;
        while(ListaUserInsere != NULL)
        {
            RetornoMencao = VerificaListaMencao(ListaMencaoInsere, ListaUserInsere->InfoLista.nome);
            if(RetornoMencao != 0)
                ListaUserInsere->InfoLista.mencoes = RetornoMencao;
            ListaUserInsere = ListaUserInsere->prox;
        }
        ListaUserInsere = ListaUser;
        // Contabiliza o engajamento do usuario
        while(ListaUserInsere != NULL)
        {
            ListaUserInsere->InfoLista.engajamento = ListaUserInsere->InfoLista.likes + ListaUserInsere->InfoLista.retweets + ListaUserInsere->InfoLista.mencoes;
            ListaUserInsere = ListaUserInsere->prox;
        }
        ListaUserInsere = ListaUser;
        while(ListaUserInsere != NULL)
        {
            ListaUserAlf = OrganizaListaUser(ListaUserAlf, ListaUserInsere->InfoLista);
            ListaUserInsere = ListaUserInsere->prox;
        }
        /// ===================== Estrutura dos Usuarios Mencionados =====================
        /// Adicionas as listas auxiliares da Struct de Usuarios mencionado em sua arvore AVL

        // Ordena a Lista de Usuarios mencionados na arvoreAVL por qtd de mencoes
        while(ListaMencao != NULL)
        {
            ListaMencaoAlf = OrganizaListaMencao(ListaMencaoAlf, ListaMencao->InfoLista);
            ListaMencao = ListaMencao->prox;
        }
        ok = 1;
        // Insere a Lista de Usuarios mencionados em sua arvore AVL
        while(ListaMencaoAlf != NULL)
        {
            *UsuariosPorMencao = InsereUserPorMencao(*UsuariosPorMencao, ListaMencaoAlf, ok);
            ListaMencaoAlf = ListaMencaoAlf->prox;
        }
        /// ===================== Estrutura dos Usuarios =====================
        /// Adicionas as listas auxiliares da Struct de Usuarios em suas respectivas arvores AVL

        // Insere a Lista de Usuarios na arvore por quantidade de tweets realzados
        ListaUserInsere = ListaUserAlf;
        ok = 1;
        while(ListaUserInsere != NULL)
        {
            *UsuariosPorTweets = InsereUserPorTweet(*UsuariosPorTweets, ListaUserInsere, ok);
            ListaUserInsere = ListaUserInsere->prox;
        }
        // Insere a Lista de Usuarios na arvore por qtd de RT que os tweets do usuario tiveram
        ListaUserInsere = ListaUserAlf;
        ok = 1;
        while(ListaUserInsere != NULL)
        {
            *UsuariosPorRT = InsereUserPorRT(*UsuariosPorRT, ListaUserInsere, ok);
            ListaUserInsere = ListaUserInsere->prox;
        }
        // Insere a Lista de Usuarios na arvore pelo valor de engajamento
        ListaUserInsere = ListaUserAlf;
        ok = 1;
        while(ListaUserInsere != NULL)
        {
            *UsuariosPorEngajamento = InsereUserPorEngajamento(*UsuariosPorEngajamento, ListaUserInsere, ok);
            ListaUserInsere = ListaUserInsere->prox;
        }
        // Organiza a Lista de Hashtag por ordem alfabetica
        while(ListaHashtag != NULL)
        {
            ListaHashtagAlf = OrganizaListaHashtag(ListaHashtagAlf, ListaHashtag->InfoLista);
            ListaHashtag = ListaHashtag->ant;
        }
        ok = 1;
        // Insere a Lista de Hashtags na arvore pela qtd de vezes que ela foi utilizada
        while(ListaHashtagAlf != NULL)
        {
            *Hashtags = InsereHashtag(*Hashtags, ListaHashtagAlf, ok);
            ListaHashtagAlf = ListaHashtagAlf->prox;
        }

        /// *****************************************************************
        /// ===================== Estrutura dos Tweets =====================
        /// Adiciona as listas auxiliares da Struct de Tweets sua arvore

        // Organiza os elementos da Lista de Tweets por ordem alfabetica
        while(ListaTweet != NULL)
        {
            ListaTweetAlf = OrganizaListaTweet(ListaTweetAlf, ListaTweet->InfoLista);
            ListaTweet = ListaTweet->prox;
        }
        // Insere a Lista de Tweets em ordem alfabetica em uma arvore AVL organizada pelo numero de RT
        ok = 1;
        while(ListaTweetAlf != NULL)
        {
            *Tweet = InsereTweet(*Tweet, ListaTweetAlf->InfoLista, ok);
            ListaTweetAlf = ListaTweetAlf->prox;
        }
    }
}
// Abre o arquivo de Operacoes e adiciona eles em uma Lista de Operacoes por ordem de leitura do arquivo de Operacoes
void TrataOperacoes(char *ArqOperacoes, LISTA_OP **ListaOp){
    int i, j;
    char Linha[ARQOP];
    char NumOp[3];
    char AuxHashtag[TAM_HASH];
    INFO_LISTA_OP InfoOp;
    FILE* arquivo = fopen(ArqOperacoes, "r");

    if(!arquivo)
        printf("Erro na abertura do arquivo de operacoes\n");
    else
    {
        while(!feof(arquivo))
        {
            fgets(Linha, ARQOP, arquivo);
            strcpy(InfoOp.CodOp, strtok(Linha, ";"));

            if(InfoOp.CodOp[0] != 'g')
            {
                strcpy(NumOp, strtok(NULL, ";"));
                InfoOp.NumOp = atoi(NumOp);
                InfoOp.Hashtag[0] = '\0';
            }
            else
            {
                strcpy(AuxHashtag, strtok(NULL, ";"));
                i = 0;
                j = 0;
                while(AuxHashtag[i] != '#')
                    i++;
                while(AuxHashtag[i] != '\0' && AuxHashtag[i] != ' ' && AuxHashtag[i] != '\n'){
                    InfoOp.Hashtag[j] = AuxHashtag[i];
                    i++;
                    j++;
                }
                InfoOp.Hashtag[j] = '\0';
                strcpy(NumOp, strtok(NULL, ";"));
                InfoOp.NumOp = atoi(NumOp);
            }
            *ListaOp = InsereListaOp(*ListaOp, InfoOp);
        }
        fclose(arquivo);
    }
}
// Auxiliar da TrataOperacoes - Insere na Lista de Operacoes
LISTA_OP* InsereListaOp(LISTA_OP* p, INFO_LISTA_OP Dados){
    LISTA_OP* Aux = p;
    LISTA_OP* Anterior = NULL;
    LISTA_OP* Novo;

    Novo = malloc(sizeof(LISTA_OP));
    Novo->InfoOp = Dados;

    if(Aux == NULL)
    {
        p = Novo;
        Novo->prox = NULL;
    }
    else
    {
        while(Aux != NULL)
        {
            Anterior = Aux;
            Aux = Aux->prox;
        }
        Anterior->prox = Novo;
        Novo->prox = NULL;
    }
    return p;
}
// Printa a lista de operacoes
void PrintaListaOp(LISTA_OP* p){
    LISTA_OP* Aux = p;
    while(Aux != NULL)
    {
        printf("Op: %s, Num: %d, Hashtag: %s\n", Aux->InfoOp.CodOp, Aux->InfoOp.NumOp, Aux->InfoOp.Hashtag);
        Aux = Aux->prox;
    }
}
// Executa as operacoes contida na Lista de Operacoes;
// Escreve gera oa rquivo de saida da seguinte forma:
// Identifica a operacao � ser realizada, e o numero do elementos que devem ser inseridos no arquivo de saida
// Abre e escreve no arquivo de saida. Fecha o arquivo e identifica a proxima para ser executada.
// (A Lista de Operacoes contem o codigo da funcao a ser executada e o numero de elementos a serem add no arquivo de saida)
void ExecutaOperacoes(char *ArqSaida, LISTA_OP **ListaOp, PT_TWEET **Tweet, PT_USER **UsuariosPorTweets, PT_USER **UsuariosPorRT, PT_USER **UsuariosPorEngajamento, PT_MENCAO **UsuariosPorMencao, PT_HASHTAG **Hashtags, PT_LISTA_HASHTAG **pt_lista_hastags, clock_t TempoGeraStrc){
    int count;
    clock_t T_Inicial, T_Final, TempoGeraSaida;
    PT_HASHTAG* ArvoreOperacaoG = NULL;
    T_Inicial = clock();
    FILE *arquivo = fopen(ArqSaida, "w");
    if(!ArqSaida)
        printf("Erro na abertura do arquivo de saida\n");
    else
    {
        while(*ListaOp != NULL)
        {
            switch((*ListaOp)->InfoOp.CodOp[0])
            {
                case 'a':
                    count = 0;
                    fprintf(arquivo, "---OP A\n");
                    a(*Hashtags, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'b':
                    count = 0;
                    fprintf(arquivo, "---OP B\n");
                    b(*UsuariosPorTweets, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'c':
                    count = 0;
                    fprintf(arquivo, "---OP C\n");
                    c(*Tweet, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'd':
                    count = 0;
                    fprintf(arquivo, "---OP D\n");
                    d(*UsuariosPorMencao, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'e':
                    count = 0;
                    fprintf(arquivo, "---OP E\n");
                    e(*UsuariosPorRT, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'f':
                    count = 0;
                    fprintf(arquivo, "---OP F\n");
                    f(*UsuariosPorEngajamento, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    break;
                case 'g':
                    count = 0;
                    fprintf(arquivo, "---OP G - %s\n", (*ListaOp)->InfoOp.Hashtag);
                    g(*pt_lista_hastags, (*ListaOp)->InfoOp.Hashtag, &ArvoreOperacaoG);
                    AuxGravaOperacaoG(ArvoreOperacaoG, (*ListaOp)->InfoOp.NumOp, &count, arquivo);
                    ArvoreOperacaoG = NULL;
                    break;
            }
            *ListaOp = (*ListaOp)->prox;
        }
        T_Final = clock();
        TempoGeraSaida = T_Final - T_Inicial;
        fprintf(arquivo, "\nTempo de geracao das estruturas: %d (em milissegundos)\n", TempoGeraStrc);
        fprintf(arquivo, "Tempo de geracao do arquivo de saida: %d (em milissegundos)", TempoGeraSaida);
        fclose(arquivo);
    }
}

/// *****************************************************************************************
/// |=====================================| OPERACOES: |====================================|
/// *****************************************************************************************

// Operacao A:
void a(PT_HASHTAG* Hashtags, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Hashtags != NULL && *count <= QuantLista)
        {
            a(Hashtags->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", Hashtags->Info_Hashtag.hashtag, Hashtags->Info_Hashtag.count);
            a(Hashtags->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, nao sao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Hashtags != NULL)
        {
            a(Hashtags->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Hashtags->Info_Hashtag.hashtag, Hashtags->Info_Hashtag.count);
            a(Hashtags->esq, QuantLista, count, Saida);
        }
    }
}
// Operacao B:
void b(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Usuarios != NULL && *count <= QuantLista)
        {
            b(Usuarios->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.tweets_count);
            b(Usuarios->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Usuarios != NULL)
        {
            b(Usuarios->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.tweets_count);
            b(Usuarios->esq, QuantLista, count, Saida);
        }
    }
}
// Operacao C:
void c(PT_TWEET* Tweet, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Tweet != NULL && *count <= QuantLista)
        {
            c(Tweet->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", Tweet->Info_Tweet.text, Tweet->Info_Tweet.RT_count);
            c(Tweet->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Tweet != NULL)
        {
            c(Tweet->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Tweet->Info_Tweet.text, Tweet->Info_Tweet.RT_count);
            c(Tweet->esq, QuantLista, count, Saida);
        }
    }
}
// Operacao D:
void d(PT_MENCAO* Usuarios, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Usuarios != NULL && *count <= QuantLista)
        {
            d(Usuarios->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista && Usuarios->Info_Mencao.count > 0)
                fprintf(Saida, "%s - %d \n", Usuarios->Info_Mencao.mencao, Usuarios->Info_Mencao.count);
            d(Usuarios->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Usuarios != NULL)
        {
            d(Usuarios->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Usuarios->Info_Mencao.mencao, Usuarios->Info_Mencao.count);
            d(Usuarios->esq, QuantLista, count, Saida);
        }
    }
}
// Operacao E:
void e(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Usuarios != NULL && *count <= QuantLista)
        {
            e(Usuarios->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.retweets);
            e(Usuarios->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Usuarios != NULL)
        {
            e(Usuarios->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.retweets);
            e(Usuarios->esq, QuantLista, count, Saida);
        }
    }
}
// Operacao F:
void f(PT_USER* Usuarios, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(Usuarios != NULL && *count <= QuantLista)
        {
            f(Usuarios->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.engajamento);
            f(Usuarios->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(Usuarios != NULL)
        {
            f(Usuarios->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", Usuarios->Info_User.nome, Usuarios->Info_User.engajamento);
            f(Usuarios->esq, QuantLista, count, Saida);
        }
    }
}
// Procura as Hashtags associada a hashtag passado por parametro pra funcao
// Funcao utlizada pelo funcao g (Operacao G)
PT_LISTA_HASHTAG* achaHashtag(char Hashtag[TAM_HASH], PT_LISTA_HASHTAG* pt_hashtag, PT_LISTA_HASHTAG* nova_lista){
    int i, j = 0;
    INFO_HASHTAG Novo;

   for(i=0; pt_hashtag->InfoLista.texto_tweet[i] != '\0'; i++)
    {
        if(pt_hashtag->InfoLista.texto_tweet[i] == '#')
        {
            while(pt_hashtag->InfoLista.texto_tweet[i] != ' ' && pt_hashtag->InfoLista.texto_tweet[i] != ';' &&
                pt_hashtag->InfoLista.texto_tweet[i] != '\0' && pt_hashtag->InfoLista.texto_tweet[i] != '"' &&
                pt_hashtag->InfoLista.texto_tweet[i] != '.' && pt_hashtag->InfoLista.texto_tweet[i] != ',' &&
                pt_hashtag->InfoLista.texto_tweet[i] != '!' && pt_hashtag->InfoLista.texto_tweet[i] != '?')
            {
                Novo.hashtag[j] = pt_hashtag->InfoLista.texto_tweet[i];
                j++;
                i++;
            }
            Novo.hashtag[j] = '\0';
            Novo.count = 1;
            j = 0;
            if(strcmp(Hashtag, Novo.hashtag) != 0)
                nova_lista = InsereListaHashtag(nova_lista, Novo);
        }
        if(pt_hashtag->InfoLista.texto_tweet[i] == '\0')
            i--;
    }
    return nova_lista;
}
// Operacao G:
void g(PT_LISTA_HASHTAG* pt_hashtag, char *Hashtag, PT_HASHTAG **HashtagsAssociadas){
    PT_LISTA_HASHTAG* HashtagsAlf = NULL;
    PT_LISTA_HASHTAG* nova_lista = NULL;
    int *ok = 1;

    while(pt_hashtag != NULL){
        if(strcmp(Hashtag, pt_hashtag->InfoLista.hashtag) == 0){
            nova_lista = achaHashtag(Hashtag, pt_hashtag, nova_lista);
            pt_hashtag = pt_hashtag->ant;
        }
        else
            pt_hashtag = pt_hashtag->ant;
    }
    while(nova_lista != NULL) // Organiza a Lista de Hashtags Associadas por ordem alfabetica
    {
        HashtagsAlf = OrganizaListaHashtag(HashtagsAlf, nova_lista->InfoLista);
        nova_lista = nova_lista->ant;
    }
    // Adiciona a Lista de hashtags associadas em uma arvore AVL
    // A arvore eh organizada pela qtd de vezes que ela eh usada assosicada a Hashtag pesquisa
    while(HashtagsAlf != NULL)
    {
        *HashtagsAssociadas = InsereHashtag(*HashtagsAssociadas, HashtagsAlf, ok);
        HashtagsAlf = HashtagsAlf->prox;
    }
}
// Funcao auxiliar da Operacao G
// Eh responsavel por gravar o resultado da Operacao G no arquivo de saida
void AuxGravaOperacaoG(PT_HASHTAG* HashtagsAssociadas, int QuantLista, int *count, FILE* Saida){
    if(QuantLista > 0)
    {
        if(HashtagsAssociadas != NULL && *count <= QuantLista)
        {
            AuxGravaOperacaoG(HashtagsAssociadas->dir, QuantLista, count, Saida);
            (*count)++;

            if(*count <= QuantLista)
                fprintf(Saida, "%s - %d \n", HashtagsAssociadas->Info_Hashtag.hashtag, HashtagsAssociadas->Info_Hashtag.count);
            AuxGravaOperacaoG(HashtagsAssociadas->esq, QuantLista, count, Saida);
        }
    }
    else if(QuantLista == 0)
    { // Note que aqui, naosao usadas todas a variveis: Count nao eh usada e QuantLista eh usada apenas no else if inicial
        if(HashtagsAssociadas != NULL)
        {
            AuxGravaOperacaoG(HashtagsAssociadas->dir, QuantLista, count, Saida);
            fprintf(Saida, "%s - %d \n", HashtagsAssociadas->Info_Hashtag.hashtag, HashtagsAssociadas->Info_Hashtag.count);
            AuxGravaOperacaoG(HashtagsAssociadas->esq, QuantLista, count, Saida);
        }
    }
}

/// *****************************************************************************************
/// |================================| Funcoes auxiliares: |================================|
/// *****************************************************************************************

// Remove os acentos de um string
void RemoveAcentos (char s[]){
    int i;
    i=strlen(s)-1;

    while (i>=0)
    {
        switch (s[i])
        {
        case 'â':
            s[i]='a';
            break;
        case 'à':
            s[i]='a';
            break;
        case 'á':
            s[i]='a';
            break;
        case 'ã':
            s[i]='a';
            break;
        case 'ô':
            s[i]='o';
            break;
        case 'ò':
            s[i]='o';
            break;
        case 'ó':
            s[i]='o';
            break;
        case 'è':
            s[i]='e';
            break;
        case 'é':
            s[i]='e';
            break;
        case 'ê':
            s[i]='e';
            break;
        case 'ù':
            s[i]='u';
            break;
        case 'ú':
            s[i]='u';
            break;
        case 'í':
            s[i]='i';
            break;
        case 'ç':
            s[i]='c';
            break;
        case 'ñ':
            s[i]='n';
            break;
        case 'Á':
            s[i]='A';
            break;
        case  'À':
            s[i]='A';
            break;
        case  'Â':
            s[i]='A';
            break;
        case  'É':
            s[i]='E';
            break;
        case  'È':
            s[i]='E';
            break;
        case  'Í':
            s[i]='I';
            break;
        case  'Ô':
            s[i]='O';
            break;
        case  'Ò':
            s[i]='O';
            break;
        case  'Ó':
            s[i]='O';
            break;
        case  'Ù':
            s[i]='U';
            break;
        case  'Ú':
            s[i]='U';
            break;
        case  'Ç':
            s[i]='C';
            break;
        case  'Ñ':
            s[i]='N';
            break;
        }
        i--;
    }
}
// Compara dois Strings passados por parametro
// Se forem iguais retorna -1
// Se retornar 0 = o str1 vem primeiro na ordem alfabetica
// Se retornar 1 = o str2 vem primeiro na ordem alfabetica
int ComparaStrings(char *str1, char *str2){
    int i = 0;
    while(str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
    {
        i++;
    }
    if(str1[i] == '\0' && str2[i] == '\0')
        return -1;
    else if(str1[i] > str2[i])
            return 0;
        else
            return 1;
}
