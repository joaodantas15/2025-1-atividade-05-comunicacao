# Relatório da Atividade 05 - Comunicação de Tarefas

- **Nome:** João Pedro Dantas Magalhães
- **Data:** 11 de junho de 2025

## 1. Objetivo

O objetivo desta atividade foi implementar e comparar duas formas de comunicação entre tarefas: uma baseada em processos distintos e outra baseada em threads concorrentes. Foram explorados os mecanismos de comunicação via sistema de arquivos e via memória compartilhada, analisando a complexidade de implementação e a performance de cada abordagem.

## 2. Comparativo de Complexidade

### 2.1 Comunicação via Arquivo
* **Versão Processos:** A implementação exigiu dois programas separados (`writer-file.c`, `reader-file.c`) e um mecanismo de sincronização explícito (o escritor espera o arquivo ser renomeado/removido). A execução requer a coordenação de dois terminais ou o uso de scripts de shell.
* **Versão Threads:** A implementação foi contida em um único arquivo (`threads-file.c`). A lógica de sincronização foi mantida, mas a complexidade de gerenciar múltiplos processos foi eliminada. A compilação exigiu a biblioteca `pthread`.

### 2.2 Comunicação via Memória Compartilhada
* **Versão Processos:** Esta foi a abordagem mais complexa, exigindo o uso da API de baixo nível do System V (`shmget`, `shmat`, `shmctl`). A sincronização e a limpeza dos recursos de memória compartilhada (que persistem no kernel) exigem cuidado.
* **Versão Threads:** Esta foi a abordagem **mais simples e elegante**. Como threads de um mesmo processo compartilham memória por padrão, a comunicação se resumiu a escrever e ler em uma variável global, com uma flag simples para sincronização. Nenhuma API externa de IPC foi necessária.

## 3. Comparativo de Tempo de Execução

Os tempos de execução foram medidos com o utilitário `time`, focando no tempo real (`real time`).

| Mecanismo de Comunicação    | Versão Processos (tempo real) | Versão Threads (tempo real) |
| --------------------------- | ----------------------------- | --------------------------- |
| Via Arquivo                 |              67.02s           |           1s                |
| Via Memória Compartilhada   |              5s               |        1.01s                |
    
**Análise dos Tempos:**
Os resultados mostram que as versões baseadas em threads são ordens de magnitude mais rápidas. Isso ocorre por dois motivos principais:
1.  **Custo de Criação:** Criar um novo processo (`fork`) é uma operação muito mais "pesada" para o sistema operacional do que criar uma nova thread.
2.  **Overhead de Comunicação:** A comunicação via memória global em threads é extremamente rápida, enquanto a comunicação via arquivos (I/O de disco) ou via memória compartilhada do kernel (que envolve chamadas de sistema) possui um overhead significativamente maior.
3.  **ia Arquivo de Versão de processos:** Porque 67.02s ? Foi o que demorei mais tempo para abrir o outro terminal e pressionar ENTER.

## 4. Conclusão

Esta atividade demonstrou que, para tarefas que precisam se comunicar dentro de uma mesma aplicação, o uso de **threads é vastamente superior em simplicidade de código (no caso de memória) e em performance**. A abordagem com processos separados é mais complexa e lenta, sendo mais adequada para a comunicação entre programas independentes. A diferença entre o acesso direto à memória em threads e o uso da API `shmget` para processos ilustra perfeitamente o benefício do espaço de endereço compartilhado das threads.
