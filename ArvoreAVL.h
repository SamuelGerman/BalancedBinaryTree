typedef struct NO* ArvAVL;

ArvAVL* cria_ArvAVL();
void libera_ArvAVL(ArvAVL *raiz);
int insere_ArvAVL(ArvAVL *raiz, int valor);
int remove_ArvAVL(ArvAVL *raiz, int valor);
int esta_vazia(ArvAVL *raiz);
int altura_ArvAVL(ArvAVL *raiz);
int total_NOs(ArvAVL *raiz);
int consulta_ArvAVL(ArvAVL *raiz, int valor);
void imprime_preOrdem_ArvAVL(ArvAVL *raiz);
void imprime_emOrdem_ArvAVL(ArvAVL *raiz);
void imprime_posOrdem_ArvAVL(ArvAVL *raiz);