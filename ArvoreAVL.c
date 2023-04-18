#include <stdio.h>
#include <stdlib.h>
#include "ArvoreAVL.h"

//struct para representar todos os elementos da arvore
struct NO{
    int num;
    int alt;
    struct NO *esq;
    struct NO *dir;
};

int main(){
    ArvAVL *raiz = cria_ArvAVL();
    insere_ArvAVL(raiz,30);
    insere_ArvAVL(raiz,2);
    insere_ArvAVL(raiz,5);
    insere_ArvAVL(raiz,4);
    insere_ArvAVL(raiz,553);
    insere_ArvAVL(raiz,61);
    insere_ArvAVL(raiz,7);
    insere_ArvAVL(raiz,8);
    insere_ArvAVL(raiz,192);
    insere_ArvAVL(raiz,62);
    insere_ArvAVL(raiz,89);
    remove_ArvAVL(raiz,5);
    remove_ArvAVL(raiz,89);
    remove_ArvAVL(raiz,30);
    remove_ArvAVL(raiz,2);
    remove_ArvAVL(raiz,192);
    consulta_ArvAVL(raiz,4);
    imprime_emOrdem_ArvAVL(raiz);
    return 0;
}

ArvAVL *cria_ArvAVL(){
//se o ponteiro duplo que aponta para a raiz da arvore é diferente de NULL (ou seja, memoria foi alocada), inicializa o conteudo da raiz
// *raiz como null, para indicar que a arvore está vazia.
    ArvAVL *raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz!=NULL)    
        *raiz = NULL;
    return raiz;
}

void libera_NO(struct NO* no){ //endereço de um no
    if(no==NULL)
        return;
    //chama a função recursivamente até chegar em um nó folha, assim ela libera a arvore de baixo para cima.
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;        
}

void libera_ArvAVL(ArvAVL *raiz){
    if(raiz==NULL)
        return;
    libera_NO(*raiz); //libera todos os nos
    free(raiz);//limpa a arvore
}

int esta_vazia(ArvAVL *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

int total_NOs(ArvAVL *raiz){
    if(esta_vazia(raiz))
        return 0;
    /*
    Chama a função recursivamente para cada subarvore do no analisado no momento.
    Nao se assuste com a forma de passar o argumento da função: &(*raiz)->esq nada mais é q o endereço de memoria do conteúdo da raiz da
    subarvore a esquerda do no analisado em cada chamada. Em outras palavras, (*raiz)->esq é o endereço do filho a esquerda da raiz e 
    equivale a um ponteiro do tipo struct no** ou ArvAVL *
    */
    int total_esq = total_NOs(&((*raiz)->esq));
    int total_dir = total_NOs(&((*raiz)->dir));
    return total_dir + total_esq + 1;
}

void imprime_preOrdem_ArvAVL(ArvAVL *raiz){
    if(esta_vazia(raiz))
        return;
    printf("%d\n",(*raiz)->num);
    imprime_emOrdem_ArvAVL(&((*raiz)->esq));
    imprime_emOrdem_ArvAVL(&((*raiz)->dir));
}

void imprime_posOrdem_ArvAVL(ArvAVL *raiz){
    if(esta_vazia(raiz))
        return;
    imprime_emOrdem_ArvAVL(&((*raiz)->esq));
    imprime_emOrdem_ArvAVL(&((*raiz)->dir));
    printf("%d\n",(*raiz)->num);
}

void imprime_emOrdem_ArvAVL(ArvAVL *raiz){
    if(esta_vazia(raiz))
        return;
    imprime_emOrdem_ArvAVL(&((*raiz)->esq));
    printf("%d\n",(*raiz)->num);
    imprime_emOrdem_ArvAVL(&((*raiz)->dir));
}

int consulta_ArvAVL(ArvAVL *raiz, int valor){
    if(esta_vazia(raiz))
        return 0;
    if(valor == (*raiz)->num)
        return 1;
    if(valor <(*raiz)->num)
        return consulta_ArvAVL(&((*raiz)->esq),valor);
    else
        return consulta_ArvAVL(&((*raiz)->dir),valor);
}

//apenas as funções de inserção e remoção sao diferentes das funções de ABB

//funções auxiliares
//retorna altura de uma arvore
int alt_NO(struct NO *no){
    if(no == NULL)
        return -1;
    else
        return no->alt;
}

//retorna o fator de balanceamente de um NO (hesq - hdir)
int fator_balanceamento_no(struct NO *no){
    int res = (alt_NO(no->esq) - alt_NO(no->dir));
    if(res == 2 || res == -2)
        return res;
    else
        return 1;
}

//retorna o maior entre 2 valores
int maior(int x, int y){
    if(x>y)
        return x;
    else
        return y;
}

/*Rotações: existem dois tipos de rotações, simples ou duplas.
Rotação Simples: quando Pai, filho e neto estão no mesmo sentido de inclinação
        Rotação LL ou rotação simples para a direita: Quando o novo nó é inserido à esquerda do filho.
        Rotação RR ou rotação para simples a esquerda: QUando o novo nó é inserido à direita do filho.
Rotação Dupla: quando Pai e filho estao inclinados no sentido inverso do Neto.
        Rotação LR ou rotação dupla à direita, quando um nó é inserido a direita do filho a esquerda de A
*/

//A rotação é aplicada no nó ancestral mais proximo do nó inserido que tem o fator de balanceamento de +-2

/*A é um ponteiro para ponteiro, por isso quando queremos acessar ou manipular algum valor do NO, é necessário acessar com (*A)
Diferentemente de B que é um ponteiro simples*/

int rotacaoLL(ArvAVL *A){ // A = raizDesbalanceada
    struct NO *B;
    B = (*A)->esq; // B = filho a esquerda de A
    (*A)->esq = B->dir; //rotação é realizada nessa linha e na de baixo.
    B->dir = (*A);      // raiz vai para a direita e o filho da esquerda vira a raiz
    //recalcula as alturas
    (*A)->alt=maior(alt_NO((*A)->esq),alt_NO((*A)->dir))+1;
    B->alt=maior(alt_NO(B->esq),alt_NO(B->dir))+1;
    *A = B; // raiz agora aponta para B, mudou a Raiz da arvore. (Ponteiro para ponteiro)
    return 1;
}

int rotacaoRR(ArvAVL *A){
    struct NO *B;
    B=(*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->alt=maior(alt_NO((*A)->esq),alt_NO((*A)->dir))+1;
    B->alt=maior(alt_NO(B->esq),alt_NO(B->dir))+1;
    *A = B; // raiz agora aponta para B, mudou a Raiz da arvore. (Ponteiro para ponteiro)
    return 1;
}

/*A rotação LR pode ser realizada atraves de duas rotações simples na ordem RR-LL*/
int rotacaoLR(ArvAVL *A){ //elemento inserido na direita do filho a esquerda , C tem que virar pai de A e B

//passa o filho esquerdo de A como raíz da subarvore que deverá ser rotacionada de forma simples a direita
    rotacaoRR(&(*A)->esq); //endereço de memoria do ponteiro esq, ou seja, ponteiro para ponteiro

// depois de fazer uma rotação simples a esquerda no filho esquerdo de Aagora a arvore está desbalanceada no caso de rotação simples LL
    rotacaoLL(A);
    return 1;
}

int rotacaoRL(ArvAVL *A){
    rotacaoLL(&(*A)->dir);
    rotacaoRR(A);
    return 1;
}
/*Como saber o tipo de rotação:
Se A e B tem o mesmo sinal de fator de balanceamento a rotação é simples. Se os sinais forem negativos ela é para a esquerda RR
se forem positivos a rotação é para a direita LL.
Se A e B tem sinais opostos a rotação é dupla. A+2 e B-1 = Dupla para a direita LR e A-2 e B+1 = Dupla para esquerda RL
*/

int insere_ArvAVL(ArvAVL *raiz,int valor){    
    int res; //result
    if(*raiz == NULL){//quando chegar um nó depois da folha
        struct NO* novo = (struct NO *) malloc(sizeof(struct NO));
        if (novo == NULL)
            return 0;
        
        novo->alt = 0;
        novo->num = valor;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo; //lembrando que raiz até entao era NULL.
        return 1;
    }
    struct NO *atual = *raiz;
    if(valor < atual->num){
        if((res = insere_ArvAVL(&(atual->esq),valor)) == 1){
            if(fator_balanceamento_no(atual)>=2){
                if(valor<(*raiz)->esq->num) //foi inserido na esquerda do filho a esquerda
                    rotacaoLL(raiz);
                else
                    rotacaoLR(raiz);
            }
        }
    }else{
        if(valor > atual->num){
            if((res = insere_ArvAVL(&(atual->dir),valor))==1){
                if(fator_balanceamento_no(atual)>=2){
                    if((*raiz)->dir->num < valor) //inserido na direita do filho a direita
                        rotacaoRR(raiz);
                    else
                        rotacaoRL(raiz);
                }
            }
        }else //valor repetido
            return 0;
    }
    atual->alt = maior(alt_NO(atual->esq),alt_NO(atual->dir))+1;
    return res;
}

struct NO* procuraMenor(struct NO* atual){
    struct NO *no1 = atual, *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

int remove_ArvAVL(ArvAVL *raiz, int valor){
    if(*raiz == NULL)//valor nao existe
        return 0;
    int res;
    if(valor < (*raiz)->num){
        if((res = remove_ArvAVL(&(*raiz)->esq,valor))==1){
            if(fator_balanceamento_no((*raiz))>=2){
                if(alt_NO((*raiz)->dir->esq) <= alt_NO((*raiz)->dir->dir))
                    rotacaoRR(raiz);
                else
                    rotacaoRL(raiz);
            }
        } 
    }
    if(valor > (*raiz)->num){
        if((res = remove_ArvAVL(&(*raiz)->dir,valor))==1){
            if(fator_balanceamento_no(*raiz)>=2){
                if(alt_NO((*raiz)->esq->dir)<= alt_NO((*raiz)->esq->esq))
                    rotacaoLL(raiz);
                else
                    rotacaoLR(raiz);
            }
        }
    }
    if((*raiz)->num == valor){ //achou o que deve ser removido.
        if(((*raiz)->esq == NULL || (*raiz)->dir ==NULL)){
            //tem 1 ou 0 filhos
            struct NO* aux = (*raiz);
            if((*raiz)->esq != NULL)
                (*raiz) = (*raiz)->esq;
            else
                (*raiz) = (*raiz)->dir; //ou um dos filhos vai "subir" na arvore ou ela ficara como NULL e perderá o no pois ela nao tem filhos
            free(aux);
        }else{//no tem dois filhos, devo substituir o no a ser removido pelo no mais a esquerda de sua subarvore a direita
            struct NO *temp = procuraMenor((*raiz)->dir);
            (*raiz)->num = temp->num;
            remove_ArvAVL(&(*raiz)->dir, (*raiz)->num);
            if(fator_balanceamento_no(*raiz) >= 2){
                if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq))
                    rotacaoLL(raiz);
                else
                    rotacaoLR(raiz);
            }
        }
            if(*raiz != NULL)
                (*raiz)->alt = maior(alt_NO((*raiz)->esq),alt_NO((*raiz)->dir))+1;
        return 1;
    }
    (*raiz)->alt = maior(alt_NO((*raiz)->esq),alt_NO((*raiz)->dir))+1;
    return res;
}

//linha 148 -> se a chamada da função remoç~çao foi feita para subárvore a esquerda devemos escolher entre a rotação RR e a rotação RL
/*
Se a altura da arvore esquerda do fiho direito for menor ou igual a da arvore direita do filho direito : RR, caso o contrario, RL
Se a chamada da função foi feita para sibárvore a direita devemos escolher entre rotação LL e rotação LR.
Se a altura da arvore direita do filho esquerdo foir menor ou igual a da arvore esquerda do filho esquerdo : rotação ll, caso o contrario: LR

*/