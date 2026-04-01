#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 2.1 Dados de Contato
typedef struct {
    char nome[100];
    char telefone[20];
    char email[100];
    char data_captacao[11]; //formato DD/MM/AAAA
} DadosContato;

// 2.2 Endereco - somente para clientes compradores
typedef struct {
    char rua[100];
    int numero;
    char bairro[60];
    char cidade[60];
    char estado[3];
    char cep[10];
} Endereco;

// 2.3 Cliente Comprador
typedef struct {
    DadosContato contato;
    char cpf[15];
    char data_nascimento[11];
    Endereco endereco;
} ClienteComprador;

// 2.4 No da Fila Duplamente Encadeada (aguardando contato)
typedef struct NoAguardando {
    DadosContato contato;
    struct NoAguardando *prox;
    struct NoAguardando *ant;
} NoAguardando;

// 2.5 No da Fila Simples (compradores)
typedef struct NoComprador {
    ClienteComprador dados;
    struct NoComprador *prox;
} NoComprador;

// 2.6 No da Pilha (clientes arquivados)
typedef struct NoPilha {
    DadosContato contato;
    struct NoPilha *prox;
} NoArquivado;

//============FOCO DA TAREFA: OPÇÃO 1 - Cadastrar Cliente ========================
void popularDados(NoAguardando **inicio, NoAguardando **fim) {
    const char *nomes[]    = {"Ana Silva",      "Bruno Costa",    "Carlos Mendes",
                               "Diana Souza",    "Eduardo Lima"};
    const char *fones[]    = {"27999110001",    "27999110002",    "27999110003",
                               "27999110004",    "27999110005"};
    const char *emails[]   = {"ana@email.com",  "bruno@email.com","carlos@email.com",
                               "diana@email.com","eduardo@email.com"};
    const char *datas[]    = {"10/03/2025",     "15/03/2025",     "18/03/2025",
                               "22/03/2025",     "28/03/2025"};

    int total = 5;
    for (int i = 0; i < total; i++) {
        NoAguardando *novo = malloc(sizeof(NoAguardando));
        if (novo == NULL) continue;

        strncpy(novo->contato.nome,           nomes[i],  99);
        strncpy(novo->contato.telefone,        fones[i],  19);
        strncpy(novo->contato.email,           emails[i], 99);
        strncpy(novo->contato.data_captacao,   datas[i],  10);

        novo->prox = NULL;
        novo->ant  = *fim;

        if (*fim != NULL)
            (*fim)->prox = novo;
        else
            *inicio = novo;

        *fim = novo;
    }

    printf("\n[5 clientes de teste carregados com sucesso!]\n");
}

void cadastrarCliente(NoAguardando **inicio, NoAguardando **fim) {
    NoAguardando *novo = malloc(sizeof(NoAguardando));
    if (novo == NULL) {
        printf("\nErro de alocacao de memoria.\n");
        return;
    }

    printf("\n=== CADASTRO DE NOVO CLIENTE ===\n");

    printf("Nome: ");
    fgets(novo->contato.nome, 100, stdin);
    novo->contato.nome[strcspn(novo->contato.nome, "\n")] = '\0';

    printf("Telefone: ");
    fgets(novo->contato.telefone, 20, stdin);
    novo->contato.telefone[strcspn(novo->contato.telefone, "\n")] = '\0';
    
    printf("Email: ");
    fgets(novo->contato.email, 100, stdin);
    novo->contato.email[strcspn(novo->contato.email, "\n")] = '\0';
    
    printf("Data de captcao (DD/MM/AAAA): ");
    fgets(novo->contato.data_captacao, 11, stdin);
    novo->contato.data_captacao[strcspn(novo->contato.data_captacao, "\n")] = '\0';
    getchar(); // consome o \n que sobra após 11 chars

    // ajustando os ponteiros da lista duplamente encadeada
    novo->prox = NULL;
    novo->ant = *fim;

    if (*fim != NULL)
        (*fim)->prox = novo;
    else
        *inicio = novo; // a fila estava vazia

    *fim = novo;

    printf("\nCliente cadastrado com sucesso!\n");
}
//============FOCO DA TAREFA: OPÇÃO 1 - Cadastrar Cliente ========================


//============FOCO DA TAREFA: OPÇÃO 2 - Realizar Contato =========================
void removerAguardando(NoAguardando **inicio, NoAguardando **fim, NoAguardando *atual) {
    if (*inicio == NULL || atual == NULL) return;

    if (*inicio == atual) *inicio = atual->prox;
    if (*fim == atual) *fim = atual->ant;

    if (atual->prox != NULL) atual->prox->ant = atual->ant;
    if (atual->ant != NULL) atual->ant->prox = atual->prox;

    free(atual);
}

void realizarContato(NoAguardando **inicioAguardando, NoAguardando **fimAguardando,
                     NoComprador **inicioCompradores, NoComprador **fimCompradores,
                     NoArquivado **topoArquivados) {

    // faz uma validação inicial para ver se não existe ninguém na fila
    if (*inicioAguardando == NULL) {
        printf("\nNenhum cliente aguardando contato.\n");
        return;
    }

    // inicia mostrando o cliente cadastrado a mais tempo
    NoAguardando *atual = *inicioAguardando;
    int opcao;
    char buscaNome[100];

    while (atual != NULL) {
        printf("\n--- INTERFACE DE CONTATO ---");
        printf("\nCliente: %s", atual->contato.nome);
        printf("\nTelefone: %s | Email: %s", atual->contato.telefone, atual->contato.email);
        printf("\nData de Captacao: %s", atual->contato.data_captacao);
        printf("\n----------------------------");
        printf("\n1. Proximo Cliente");    
        printf("\n2. Cliente Anterior"); 
        printf("\n3. Buscar por Nome"); 
        printf("\n4. FINALIZAR ATENDIMENTO");
        printf("\n5. Sair do Modulo");
        printf("\nEscolha: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer

        switch (opcao) {
            case 1: // exibir próximo
                if (atual->prox != NULL) atual = atual->prox;
                else printf("\nFim da fila.\n");
                break;

            case 2: // exibir anterior
                if (atual->ant != NULL) atual = atual->ant;
                else printf("\nInicio da fila.\n");
                break;

            case 3: // busca por nome
                printf("Digite o nome completo: ");
                fgets(buscaNome, 100, stdin);
                buscaNome[strcspn(buscaNome, "\n")] = '\0';

                NoAguardando *temp = *inicioAguardando;
                int achou = 0;
                while (temp != NULL) {
                    if (strcmp(temp->contato.nome, buscaNome) == 0) {
                        atual = temp;
                        achou = 1;
                        break;
                    }
                    temp = temp->prox;
                }
                if (!achou) printf("\nCliente nao encontrado. Mantendo atual.\n");
                break;

            //============FOCO DA TAREFA: OPÇÃO 2 - Finalizar Atendimento =======================
            case 4: {
                int comprou;
                printf("\n--- FINALIZAR ATENDIMENTO ---");
                printf("\nO cliente %s comprou o produto?", atual->contato.nome);
                printf("\n1. Sim");
                printf("\n2. Nao");
                printf("\nEscolha: ");
                scanf("%d", &comprou);
                getchar();

                if (comprou == 1) {
                    //alocando o nó para a fila de compradores
                    NoComprador *novo = malloc(sizeof(NoComprador));
                    if (novo == NULL) {
                        printf("\nErro de alocacao de memoria.\n");
                        break;
                    }

                    //copiando os dados de contatos que ja existem
                    novo->dados.contato = atual->contato;
                    novo->prox = NULL;

                    //coletando os dados adicionais
                    printf("\n--- DADOS ADICIONAIS ---\n");

                    printf("CPF: ");
                    fgets(novo->dados.cpf, 15, stdin);
                    novo->dados.cpf[strcspn(novo->dados.cpf, "\n")] = '\0';

                    printf("Data de nascimento (DD/MM/AAAA): ");
                    fgets(novo->dados.data_nascimento, 11, stdin);
                    novo->dados.data_nascimento[strcspn(novo->dados.data_nascimento, "\n")] = '\0';
                    getchar(); // consome o '\n' que sobra após 11 chars

                    printf("Rua: ");
                    fgets(novo->dados.endereco.rua, 100, stdin);
                    novo->dados.endereco.rua[strcspn(novo->dados.endereco.rua, "\n")] = '\0';

                    printf("Numero: ");
                    scanf("%d", &novo->dados.endereco.numero);
                    getchar();

                    printf("Bairro: ");
                    fgets(novo->dados.endereco.bairro, 60, stdin);
                    novo->dados.endereco.bairro[strcspn(novo->dados.endereco.bairro, "\n")] = '\0';

                    printf("Cidade: ");
                    fgets(novo->dados.endereco.cidade, 60, stdin);
                    novo->dados.endereco.cidade[strcspn(novo->dados.endereco.cidade, "\n")] = '\0';

                    printf("Estado (UF): ");
                    fgets(novo->dados.endereco.estado, 3, stdin);
                    novo->dados.endereco.estado[strcspn(novo->dados.endereco.estado, "\n")] = '\0';
                    getchar();

                    printf("CEP: ");
                    fgets(novo->dados.endereco.cep, 10, stdin);
                    novo->dados.endereco.cep[strcspn(novo->dados.endereco.cep, "\n")] = '\0';
                    getchar();

                    //enfileira na fila de compradores
                    if (*fimCompradores == NULL) {
                        *inicioCompradores = novo;
                        *fimCompradores = novo;
                    } else {
                        (*fimCompradores)->prox = novo;
                        *fimCompradores = novo;
                    }

                    printf("\nCliente movido para fila de compradores com sucesso!\n");

                    // remove da fila de aguardando e avança o ponteiro
                    NoAguardando *proximo = atual->prox;
                    removerAguardando(inicioAguardando, fimAguardando, atual);
                    atual = proximo;

                } else {
                    // caso o cliente não tenha comprado
                    int arquivar;
                    printf("\nDeseja arquivar os dados do cliente para contato futuro?");
                    printf("\n1. Arquivar");
                    printf("\n2. Excluir definitivamente");
                    printf("\nEscolha: ");
                    scanf("%d", &arquivar);
                    getchar();

                    if (arquivar == 1) {
                        // empilhando na pilha de arquivados
                        NoArquivado *noArq = malloc(sizeof(NoArquivado));
                        if (noArq == NULL) {
                            printf("\nErro de alocacao de memoria.\n");
                            break;
                        }
                        noArq->contato = atual->contato;
                        noArq->prox = *topoArquivados;
                        *topoArquivados = noArq;
                        printf("\nCliente arquivado com sucesso!\n");
                    } else {
                        printf("\nCliente removido definitivamente do sistema.\n");
                    }

                    //removendo da fila de aguardando em qualquer um dos casos
                    NoAguardando *proximo = atual->prox;
                    removerAguardando(inicioAguardando, fimAguardando, atual);
                    atual = proximo;
                }

                if (atual == NULL) return; //Saindo do modulo apos a fila esvaziar
                break;
            }
            case 5:
                return;
            default:
                printf("\nOpcao invalida.\n");
        }
    }
}
//============FOCO DA TAREFA: OPÇÃO 3 - Relatórios =======================
int calcularDiasEspera(const char *data_captacao) {

    int dia, mes, ano;
    if (sscanf(data_captacao, "%d/%d/%d", &dia, &mes, &ano) != 3)
        return -1;
    
    // struct tm para a data de captação
    struct tm t_captacao = {0};
    t_captacao.tm_mday = dia;
    t_captacao.tm_mon  = mes - 1;
    t_captacao.tm_year = ano - 1900;

    time_t tempo_captacao = mktime(&t_captacao);
    time_t tempo_hoje     = time(NULL);

    if (tempo_captacao == (time_t)-1 || tempo_hoje == (time_t)-1)
        return -1;

    double diferenca = difftime(tempo_hoje, tempo_captacao);
    return (int)(diferenca / 86400); // conversão de segundos para dia
}
void relatorios(NoAguardando *inicioAguardando) {
    int opcao;

    printf("\n=== RELATÓRIOS ===");
    printf("\n1. Número de clientes aguardando contato");
    printf("\n2. Lista de clientes aguardando com dias em espera");
    printf("\n3. Voltar");
    printf("\nEscolha: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {

        case 1: {
            int contador = 0;
            NoAguardando *atual = inicioAguardando;
            while (atual != NULL) {
                contador++;
                atual = atual->prox;
            }
            printf("\nTotal de clientes aguardando contato: %d\n", contador);
            break;
        }

        case 2: {
            if (inicioAguardando == NULL) {
                printf("\nNenhum cliente aguardando contato.\n");
                break;
            }

            printf("\n%-30s %-15s %-12s %s\n",
                   "Nome", "Telefone", "Captacao", "Dias aguardando");
            printf("----------------------------------------------------------------------\n");

            NoAguardando *atual = inicioAguardando;
            while (atual != NULL) {
                int dias = calcularDiasEspera(atual->contato.data_captacao);
                if (dias < 0)
                    printf("%-30s %-15s %-12s %s\n",
                           atual->contato.nome,
                           atual->contato.telefone,
                           atual->contato.data_captacao,
                           "Data invalida");
                else
                    printf("%-30s %-15s %-12s %d dia(s)\n",
                           atual->contato.nome,
                           atual->contato.telefone,
                           atual->contato.data_captacao,
                           dias);
                atual = atual->prox;
            }
            break;
        }

        case 3:
            return;

        default:
            printf("\nOpcao invalida.\n");
    }
}
//============FOCO DA TAREFA: OPÇÃO 3 - Relatórios =======================
