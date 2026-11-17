# Sistema de Registro de Compra de Uvas por Romaneio

## 1. Descrição do Sistema

O projeto consiste em um sistema de gerenciamento para intermediários de uvas, desenvolvido em linguagem C. O modelo de dados é baseado em um romaneio manual real (Figura 01), utilizado por um comprador de uva, que reúne em um único documento numerado (Destino, Data e Nº do romaneio) uma tabela com vários itens, cada linha trazendo Quantidade, Peso, Produto, Produtor, Preço Unitário e Preço Total, seguida de totais gerais, Comissão, Frete e Valor líquido a receber.

<p align="center">
Figura 01 – Romaneio manual
</p>
<p align="center">
  <img width="500" alt="Image" src="https://github.com/user-attachments/assets/25ef3f5d-1abd-4595-ab58-9d876b91de02](https://github.com/user-attachments/assets/78ba6bd2-ee89-44a3-8840-3f7d1ae20462" />
</p>

O sistema permite registrar romaneios com vários itens, informando o produtor e o produto (variedade da uva) diretamente em cada item. Ele realiza automaticamente os cálculos da operação (preço total de cada item, quantidade de volumes, peso total, valor dos produtos, comissão e valor final) e grava tudo em arquivo binário, mantendo um histórico que pode ser consultado depois e no qual é possível alterar a situação do pagamento.

A proposta busca solucionar problemas do preenchimento manual dos romaneios, como erros de cálculo, dificuldade para localizar informações, perda de registros e necessidade de refazer contas de valores e totais. Dessa forma, o sistema torna o gerenciamento das compras de uvas mais organizado, rápido e confiável, mantendo no formato digital as principais informações do romaneio utilizado atualmente.

___


## 2. Fluxo de Utilização Esperado para o Sistema

### 2.1 Menu Principal
Ao iniciar o programa, o usuário visualiza um menu principal com as opções:
   * `1. Registrar Romaneio`
   * `2. Historico de Romaneios`
   * `3. Sair`

### 2.2 Registrar Romaneio

Ao selecionar a opção `1`, o sistema solicita os dados gerais do romaneio:

* Número do romaneio (não pode repetir);
* Data (DD/MM/AAAA);
* Destino.

Em seguida, o usuário insere um ou mais itens (até 10), em um laço de repetição. Para cada item informa:

* Produtor (nome digitado);
* Produto, ou seja, a variedade da uva (nome digitado);
* Quantidade de volumes;
* Peso de cada volume (kg);
* Preço por kg (R$).

O sistema calcula automaticamente o preço total de cada item:

`Preço Total = Volumes × Peso de cada volume × Preço por kg`

Após os itens, o usuário informa o frete e o sistema calcula os totais do romaneio:

* Quantidade total de volumes;
* Peso total (soma de volumes × peso de cada item);
* Valor total dos produtos;
* Comissão (15% do valor dos produtos);
* Valor final = valor dos produtos + comissão + frete.

Exemplo: 15 volumes × 8 kg × R$ 56,00 = R$ 6.720,00; comissão de R$ 1.008,00; com frete de R$ 200,00, o valor final é R$ 7.928,00.

Todo romaneio novo é gravado no arquivo `romaneios.dat` com a situação `PENDENTE`.

### 2.3 Histórico de Romaneios
Na opção `2`, o usuário escolhe como consultar os romaneios registrados:

* Por número do romaneio;
* Por produtor (nome ou parte do nome);
* Por produto (nome ou parte do nome);
* Por data;
* Listar todos.

O sistema mostra os romaneios encontrados em uma lista resumida (número, data, destino, valor final e situação). O usuário pode então digitar o número de um romaneio para ver todos os dados. Se a situação for `PENDENTE`, o sistema pergunta se deseja marcar como `PAGO`, e o arquivo `romaneios.dat` é atualizado.

### 2.4 Encerramento
Ao selecionar a opção `3`, o sistema encerra a execução.

### 2.5 Tratamento de erros
O sistema exibe mensagens claras e volta ao menu (ou repete a pergunta) nos seguintes casos:

* Valor numérico inválido ou fora da faixa permitida (a pergunta é repetida);
* Número de romaneio já existente;
* Data fora do formato DD/MM/AAAA;
* Limite de romaneios atingido;
* Falha ao abrir ou criar o arquivo `romaneios.dat`;
* Nenhum romaneio registrado ainda;
* Busca sem resultados;
* Número de romaneio não encontrado ao pedir os detalhes.

___

## 3. Fluxograma da Lógica do Sistema
<p align="center">
Figura 02 – Fluxograma
</p>
<p align="center">
  <img width="500" alt="Fluxograma" src="https://github.com/user-attachments/assets/591bc431-fee2-4336-bede-1b8393d75430" />
</p>

___

## 4. Estrutura de Dados

O sistema utiliza duas estruturas heterogêneas. O romaneio contém um **vetor de structs** (`itens`), reproduzindo a tabela do documento em papel.

```c
// Constantes
#define MAX_ITENS 10            /* itens por romaneio */
#define MAX_ROMANEIOS 100       /* romaneios guardados no arquivo */
#define ARQUIVO "romaneios.dat"
#define COMISSAO 0.15f          /* 15% sobre o valor dos produtos */

// Uma linha (item) do romaneio
typedef struct {
    char produtor[50];
    char produto[50];           /* variedade da uva */
    int volumes;
    float peso;                 /* peso de cada volume (kg) */
    float preco_unit;           /* preco por kg */
    float preco_total;          /* volumes * peso * preco_unit */
} Item;

// O romaneio completo, com vetor de itens e totais
typedef struct {
    int numero;
    char data[11];              /* DD/MM/AAAA */
    char destino[50];
    Item itens[MAX_ITENS];
    int qtd_itens;
    int total_volumes;
    float peso_total;
    float valor_produtos;
    float comissao;
    float frete;
    float valor_final;
    char situacao[10];          /* "PENDENTE" ou "PAGO" */
} Romaneio;
```

Os romaneios ficam gravados em arquivo binário: `romaneios.dat`.

___

## 5. Organização dos Arquivos

| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Exibe o menu principal e chama as funções de cada opção |
| `romaneio.h` | Constantes, structs (`Item` e `Romaneio`) e protótipos das funções |
| `romaneio.c` | Leitura de números, leitura e gravação do arquivo, registro, histórico e exibição dos romaneios |

___

## 6. Como Compilar e Executar

Com os três arquivos na mesma pasta, compile:

```
gcc -o romaneio main.c romaneio.c
```

Execute:

* Windows: `romaneio.exe`
* Linux/macOS: `./romaneio`

Em IDEs como Code::Blocks ou Dev-C++, crie um projeto de console em C e adicione `main.c` e `romaneio.c`. O arquivo `romaneios.dat` é criado na pasta do programa no primeiro registro.

___

## 7. Conceitos da Disciplina Aplicados

| Conceito | Onde aparece |
|---|---|
| Funções de entrada e saída | `printf` e `scanf` |
| Estruturas de controle | `if`, `else`, `for`, `while` e `do-while` |
| Funções (modularização) | Funções em `romaneio.c`, chamadas pelo `main.c` |
| Estruturas homogêneas | Vetor de romaneios, vetor de itens e strings (`char[]`) |
| Estruturas heterogêneas | `Item` e `Romaneio`, com vetor de structs |
| Arquivos | `fopen`, `fread`, `fwrite` e `fclose` em `romaneios.dat` |
| Divisão do projeto | `main.c`, `romaneio.h`, `romaneio.c` e documentação (este README) |
Em IDEs como Code::Blocks ou Dev-C++, crie um projeto de console em C e adicione os cinco arquivos `.c`. Os arquivos `.dat` são criados na pasta do programa no primeiro cadastro.

