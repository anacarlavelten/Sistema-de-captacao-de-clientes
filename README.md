# Sistema de Controle de Captação de Clientes

#### Este projeto consiste no desenvolvimento de uma aplicação em linguagem C projetada para gerenciar o fluxo de prospecção e vendas de uma empresa fictícia. O objetivo principal é aplicar conceitos de Estrutura de Dados, como listas encadeadas, filas e pilhas, para organizar o ciclo de vida de um cliente, desde o cadastro inicial até a conversão em venda ou arquivamento de dados.

### Divisão do projeto
```
├── sistemaControle.c --- Onde o está localizado a main com o menu de opções.
├── estruturas_e_funcoes.h --- Onde estão as demais funções, structs e estruturas.
```

## TAD (Tipos abstratos de dados) utilizados:

### **Struct DadosContato**

Bloco base reutilizado por todas as estruturas do sistema. Armazena as informações mínimas de qualquer cliente.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `nome` | `char[100]` | Nome completo do cliente |
| `telefone` | `char[20]` | Número de telefone |
| `email` | `char[100]` | Endereço de e-mail |
| `data_captacao` | `char[11]` | Data de captação no formato `DD/MM/AAAA` |

### **Struct Endereco**

Usada exclusivamente para clientes compradores. Armazena o endereço completo.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `rua` | `char[100]` | Nome da rua |
| `numero` | `int` | Número do imóvel |
| `bairro` | `char[60]` | Bairro |
| `cidade` | `char[60]` | Cidade |
| `estado` | `char[3]` | UF (ex: `ES`) |
| `cep` | `char[10]` | CEP |

### **Struct ClienteComprador**

Agrega DadosContato e Endereco com dados cadastrais adicionais para clientes compradores.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `contato` | `DadosContato` | Dados básicos de contato |
| `cpf` | `char[15]` | CPF do cliente |
| `data_nascimento` | `char[11]` | Data de nascimento (`DD/MM/AAAA`) |
| `endereco` | `Endereco` | Endereço completo |

## Estruturas Encadeadas

### **NoAguardando - Fila Duplamente encadeada**

Representa clientes aguardando contato. Utilizamos esta estrutura pois a fila duplamente encadeada permite navegação nos dois sentidos, essencial para a interface de atendimento. Esta estrutura utiliza a política FIFO.

```
[inicioAguardando]                              [fimAguardando]
       ↓                                               ↓
    [Ana] ⇄ [Bruno] ⇄ [Carlos] ⇄ [Diana] ⇄ [Eduardo]
```
 
| Campo | Tipo | Descrição |
|-------|------|-----------|
| `contato` | `DadosContato` | Dados do cliente |
| `*prox` | `NoAguardando*` | Ponteiro para o próximo nó |
| `*ant` | `NoAguardando*` | Ponteiro para o nó anterior |

---

### **NoComprador -  Fila Simples**

Representa clientes que realizaram a compra. Utilizamos uma Fila simples pois, não há necessidade de navegação reversa. Nesta estrutura foi utilizado a política FIFO.

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `dados` | `ClienteComprador` | Dados completos do comprador |
| `*prox` | `NoComprador*` | Ponteiro para o próximo nó |


### **NoArquivado - Pilha**

Representa clientes que não compraram mas podem ser recontactados. Implementada como pilha. Utilizamos a política LIFO nesta estrutura.

```
topoArquivados
      ↓
  [Eduardo]  ← último arquivado (topo)
      ↓
  [Diana]
      ↓
  [Carlos]   ← primeiro arquivado (fundo)
```
 
| Campo | Tipo | Descrição |
|-------|------|-----------|
| `contato` | `DadosContato` | Dados do cliente arquivado |
| `*prox` | `NoArquivado*` | Ponteiro para o nó abaixo na pilha |
---

## Funções

### **popularDados()**

Insere automaticamente 5 clientes fictícios na fila de aguardando ao iniciar o sistema. Criamos esta função para demonstração durante a apresentação do projeto.

```c
void popularDados(NoAguardando **inicio, NoAguardando **fim)
```

### **cadastrarCliente() - Opção 1 do menu**

Lê os dados do novo cliente via teclado e o insere no fim da fila de aguardando. Realiza:

1. Alocação de memória com `malloc`
2. Leitura dos campos com `fgets` e remoção do `\n` via `strcspn`
3. Ajuste dos ponteiros `ant`, `prox`, `*inicio` e `*fim`

```c
void cadastrarCliente(NoAguardando **inicio, NoAguardando **fim)
```

**removerAguardando()**

Função auxiliar que remove um nó específico da fila duplamente encadeada. Trata os casos de remoção no início, no fim e no meio da fila, reconectando os vizinhos e liberando a memória com free.

```c
void removerAguardando(NoAguardando **inicio, NoAguardando **fim, NoAguardando *atual)
```

### **realizarContato() - Opção 2 do menu**

Abre a interface de navegação e atendimento da fila. Inicia pelo cliente há mais tempo aguardando.

```c
void realizarContato(NoAguardando **inicioAguardando, NoAguardando **fimAguardando,
                     NoComprador **inicioCompradores, NoComprador **fimCompradores,
                     NoArquivado **topoArquivados)
```

Opções após escolher a opção 2 no menu:

| Opção | Ação |
|-------|------|
| `1` | Avança para o próximo cliente (`atual->prox`) |
| `2` | Volta para o cliente anterior (`atual->ant`) |
| `3` | Busca um cliente pelo nome exato |
| `4` | Finaliza o atendimento (ver abaixo) |
| `5` | Sai do módulo |

### **Finalizar Atendimento (opção 4, dentro da função de contato):**

- **Cliente comprou** → coleta CPF, data de nascimento e endereço; enfileira em `NoComprador` (FIFO); remove da fila de aguardando.
- **Cliente não comprou** → pergunta se arquiva ou exclui definitivamente:
  - *Arquivar*: empilha em `NoArquivado` (LIFO).
  - *Excluir*: remove da fila de aguardando sem salvar.

---

### **relatorios() — Opção 3 do menu**

Exibe relatórios sobre a fila de aguardando.

```c
void relatorios(NoAguardando *inicioAguardando)
```

| Sub-opção | Descrição |
|-----------|-----------|
| `1` | Conta e exibe o total de clientes aguardando |
| `2` | Lista todos os clientes com nome, telefone, data de captação e dias em espera |

---

### **calcularDiasEspera() - função dentro da parte de relatórios**

Calcula quantos dias se passaram desde a captação do cliente até hoje. Converte a string `DD/MM/AAAA` em `time_t` via `mktime()` e usa `difftime()` para obter a diferença em segundos, convertendo para dias (`÷ 86400`).
```c
int calcularDiasEspera(const char *data_captacao)
```


### Fluxo Geral do Sistema
 
```
         [CADASTRAR]              [CONTATO]                [ARQUIVAR]
              ↓                      ↓                         ↓
         NoAguardando  ──comprou──▶ NoComprador          NoArquivado
         (Fila Dupla)               (Fila Simples)         (Pilha)
              ↑
         popularDados()
         (dados de teste)
```