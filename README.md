
# Projeto: Montador, Linker e Máquina Virtual para ISA Simples

## Descrição

Este projeto implementa um **assembler**, **linker** e **máquina virtual** para uma arquitetura de instruções simples (ISA), inspirada na MIPS. A arquitetura possui **memória separada para dados e instruções**, e o sistema foi projetado para ser modular e eficiente, facilitando o estudo de compiladores, montadores e arquiteturas de baixo nível.

## Funcionalidades

- **Assembler**: Lê arquivos `.txt` contendo variáveis, labels e instruções, e gera uma estrutura intermediária.
- **Linker**: Recebe múltiplos arquivos montados, resolve labels globais e locais, e combina tudo em duas memórias finais:  
  *Memória de Instruções* e *Memória de Dados*.
- **VM (Máquina Virtual)**: Executa os binários gerados, interpretando a ISA definida para o projeto.

## Estrutura do Projeto

```
.
├── assembler.cpp       # Lógica para montagem de código
├── assembler.h
├── instructions.h      # Definição da ISA
├── linker.cpp          # Lógica de linkedição
├── linker.h
├── main.cpp            # Entrada principal do programa
├── vm.cpp              # Implementação da VM
├── vm.h
├── Makefile            # Compilação dos binários
├── programs/           # Programas de exemplo para teste
└── main                # Binário gerado
```

## Compilação

```bash
make
```

Isso gera o executável `main`.

## Execução

```bash
./main p1.txt p2.txt
```

A saída será o código linkado em memória e executado pela VM.

## Exemplo de Uso

### Arquivo `.asm` de entrada (exemplo simplificado):

```
WORD x 10
WORD y 20
LOAD r1 x
ADD r1 y
STORE r1 z
```

### Memória Final (após linkagem):

| Instruções        | Dados          |
|-------------------|----------------|
| LOAD r1, [0x00]   | x: 10          |
| ADD r1, [0x01]    | y: 20          |
| STORE r1, [0x02]  | z: -           |

## Código de Exemplo

```cpp
Instruction instr;
instr.opcode = Opcode::LOAD;
instr.reg = 1;
instr.address = 0x00;
memory.push_back(instr);
```

## Tabela de Instruções

| Instrução | Descrição         |
|----------:|------------------:|
| `LOAD`    | Carrega da memória |
| `STORE`   | Armazena na memória |
| `ADD`     | Soma valores       |
| `SUB`     | Subtrai valores    |
| `JMP`     | Salto incondicional |
| `BEQ`     | Salto condicional  |


## Instruções da ISA

A arquitetura definida neste projeto utiliza instruções simples e diretas, com operações típicas de uma ULA e controle de fluxo. Abaixo está a tabela completa das instruções implementadas:

| Instrução | Código | Descrição                                                                 |
|-----------|--------|---------------------------------------------------------------------------|
| `NOP`     | -1     | No operation — instrução vazia, usada como preenchimento                  |
| `ADD`     | 0      | Soma dois registradores e armazena o resultado em um registrador          |
| `SUB`     | 1      | Subtrai o segundo registrador do primeiro                                 |
| `MUL`     | 2      | Multiplica dois registradores                                              |
| `DIV`     | 3      | Divide dois registradores                                                  |
| `MV`      | 4      | Move o valor de uma variável (memória de dados) para um registrador        |
| `ST`      | 5      | Armazena o valor de um registrador em uma variável (memória de dados)     |
| `JMP`     | 6      | Salto incondicional para um endereço de instrução                         |
| `JEQ`     | 7      | Salta se dois registradores forem iguais                                   |
| `JGT`     | 8      | Salta se o primeiro registrador for maior que o segundo                   |
| `JLT`     | 9      | Salta se o primeiro registrador for menor que o segundo                   |
| `W`       | 10     | Escreve na saída o valor de uma variável ou registrador                   |
| `R`       | 11     | Lê da entrada um valor para uma variável                                  |
| `STP`     | 12     | Interrompe a execução do programa                                          |

> **Observação:** Os saltos (`JMP`, `JEQ`, `JGT`, `JLT`) operam sobre endereços de instrução (LABELS), não variáveis.


## Linkagem

> O linker identifica todas as labels e resolve seus endereços reais, rearranjando os blocos de instruções e dados, para que todos os acessos à memória estejam corretos após a fusão dos arquivos.

## Licença

MIT © Artur de Camargo
