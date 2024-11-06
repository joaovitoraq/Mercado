// ALUNOS:
// João Vitor de Oliveira Aquino - 202208821571
// Juan dos Santos de Azevedo - 202309425637

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Fruta;


typedef struct No {
    Fruta fruta;
    struct No *proximo;
} No;


No* carregarFrutas();
void salvarFrutas(No *inicio);
No* cadastrarFruta(No *inicio);
void listarFrutas(No *inicio);
No* buscarFruta(No *inicio, int codigo);
No* alterarFruta(No *inicio, int codigo);
No* excluirFruta(No *inicio, int codigo);
No* venderFruta(No *inicio, int codigo, int quantidade);
void salvarVenda(int codigo, int quantidade, float preco);
void liberarMemoria(No *inicio);

int main() {
    No *inicio = carregarFrutas(); 
    int opcao, codigo, quantidade;

    do {
        printf("\n===== Mercado de Frutas =====\n");
        printf("1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Buscar fruta\n");
        printf("4. Alterar fruta\n");
        printf("5. Excluir fruta\n");
        printf("6. Vender fruta\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                inicio = cadastrarFruta(inicio);
                break;
            case 2:
                listarFrutas(inicio);
                break;
            case 3:
                printf("Digite o codigo da fruta para buscar: ");
                scanf("%d", &codigo);
                No *frutaEncontrada = buscarFruta(inicio, codigo);
                if (frutaEncontrada) {
                    printf("Codigo: %d | Nome: %s | Quantidade: %d | Preco: %.2f\n", frutaEncontrada->fruta.codigo, frutaEncontrada->fruta.nome, frutaEncontrada->fruta.quantidade, frutaEncontrada->fruta.preco);
                } else {
                    printf("Fruta nao encontrada!\n");
                }
                break;
            case 4:
                printf("Digite o codigo da fruta para alterar: ");
                scanf("%d", &codigo);
                inicio = alterarFruta(inicio, codigo);
                break;
            case 5:
                printf("Digite o codigo da fruta para excluir: ");
                scanf("%d", &codigo);
                inicio = excluirFruta(inicio, codigo);
                break;
            case 6:
                printf("Digite o codigo da fruta para vender: ");
                scanf("%d", &codigo);
                printf("Digite a quantidade para vender: ");
                scanf("%d", &quantidade);
                inicio = venderFruta(inicio, codigo, quantidade);
                break;
            case 7:
                salvarFrutas(inicio);  
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 7);

    liberarMemoria(inicio);
    return 0;
}


No* carregarFrutas() {
    FILE *arquivo = fopen("frutas.bin", "rb");
    if (arquivo == NULL) {
        printf("Nenhum dado encontrado, iniciando um novo banco.\n");
        return NULL;
    }

    No *inicio = NULL, *ultimo = NULL;
    Fruta fruta;

    while (fread(&fruta, sizeof(Fruta), 1, arquivo) == 1) {
        No *novoNo = (No*) malloc(sizeof(No));
        if (novoNo == NULL) {
            printf("Erro ao alocar memoria.\n");
            fclose(arquivo);
            return inicio;
        }
        novoNo->fruta = fruta;
        novoNo->proximo = NULL;

        if (inicio == NULL) {
            inicio = novoNo;
        } else {
            ultimo->proximo = novoNo;
        }
        ultimo = novoNo;
    }

    fclose(arquivo);
    return inicio;
}


void salvarFrutas(No *inicio) {
    FILE *arquivo = fopen("frutas.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de frutas para salvar!\n");
        return;
    }

    No *atual = inicio;
    while (atual != NULL) {
        fwrite(&atual->fruta, sizeof(Fruta), 1, arquivo);
        atual = atual->proximo;
    }

    fclose(arquivo);
}


No* cadastrarFruta(No *inicio) {
    Fruta novaFruta;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &novaFruta.codigo);

    if (buscarFruta(inicio, novaFruta.codigo) != NULL) {
        printf("Codigo ja existe!\n");
        return inicio;
    }

    printf("Digite o nome da fruta: ");
    scanf("%s", novaFruta.nome);
    printf("Digite a quantidade em estoque: ");
    scanf("%d", &novaFruta.quantidade);
    printf("Digite o preco da fruta: ");
    scanf("%f", &novaFruta.preco);

    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->fruta = novaFruta;
    novoNo->proximo = inicio;

    printf("Fruta cadastrada com sucesso!\n");
    return novoNo;
}


void listarFrutas(No *inicio) {
    No *atual = inicio;
    printf("\n===== Lista de Frutas =====\n");
    while (atual != NULL) {
        printf("Codigo: %d | Nome: %s | Quantidade: %d | Preco: %.2f\n", atual->fruta.codigo, atual->fruta.nome, atual->fruta.quantidade, atual->fruta.preco);
        atual = atual->proximo;
    }
}


No* buscarFruta(No *inicio, int codigo) {
    No *atual = inicio;
    while (atual != NULL) {
        if (atual->fruta.codigo == codigo)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}


No* alterarFruta(No *inicio, int codigo) {
    No *frutaAlterar = buscarFruta(inicio, codigo);
    if (frutaAlterar) {
        printf("Digite o novo nome: ");
        scanf("%s", frutaAlterar->fruta.nome);
        printf("Digite a nova quantidade: ");
        scanf("%d", &frutaAlterar->fruta.quantidade);
        printf("Digite o novo preco: ");
        scanf("%f", &frutaAlterar->fruta.preco);
        printf("Fruta alterada com sucesso!\n");
    } else {
        printf("Fruta nao encontrada!\n");
    }
    return inicio;
}


No* excluirFruta(No *inicio, int codigo) {
    No *anterior = NULL;
    No *atual = inicio;

    while (atual != NULL && atual->fruta.codigo != codigo) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Fruta nao encontrada!\n");
        return inicio;
    }

    if (atual->fruta.quantidade > 0) {
        printf("Nao e possivel excluir. Estoque ainda disponivel!\n");
        return inicio;
    }

    if (anterior == NULL) {
        inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("Fruta excluida com sucesso!\n");
    return inicio;
}

No* venderFruta(No *inicio, int codigo, int quantidade) {
    No *frutaVender = buscarFruta(inicio, codigo);
    if (frutaVender) {
        if (frutaVender->fruta.quantidade < quantidade) {
            printf("Quantidade indisponivel em estoque!\n");
        } else {
            frutaVender->fruta.quantidade -= quantidade;
            salvarVenda(frutaVender->fruta.codigo, quantidade, frutaVender->fruta.preco);
            printf("Venda realizada com sucesso!\n");
        }
    } else {
        printf("Fruta nao encontrada!\n");
    }
    return inicio;
}

void salvarVenda(int codigo, int quantidade, float preco) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas!\n");
        return;
    }

    fprintf(arquivo, "Codigo: %d | Quantidade: %d | Preco total: %.2f\n", codigo, quantidade, quantidade * preco);
    fclose(arquivo);
}

void liberarMemoria(No *inicio) {
    No *atual = inicio;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}
