
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define FILENAME "comunicacao_threads.txt"

// Thread que escreve no arquivo
void* writer_thread(void* arg) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) { perror("fopen writer"); pthread_exit(NULL); }

    const char *message = "Olá, threads via arquivo!\n";
    fwrite(message, 1, strlen(message), file);
    fclose(file);

    printf("Writer Thread: Mensagem escrita.\n");
    return NULL;
}

// Thread que lê o arquivo
void* reader_thread(void* arg) {
    // Espera o arquivo ser criado
    while (access(FILENAME, F_OK) != 0) {
        usleep(10000); // Espera 10ms
    }

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) { perror("fopen reader"); pthread_exit(NULL); }

    char buffer[256] = {0};
    fread(buffer, 1, sizeof(buffer)-1, file);
    fclose(file);

    printf("Reader Thread: Mensagem lida: %s", buffer);

    // Remove o arquivo para sinalizar que terminou
    remove(FILENAME);
    return NULL;
}

int main() {
    pthread_t writer, reader;

    printf("Iniciando comunicação entre threads via arquivo...\n");

    // Cria a thread leitora primeiro, para que ela fique esperando
    pthread_create(&reader, NULL, reader_thread, NULL);
    // Pequena pausa para garantir que o leitor já está no loop de espera
    sleep(1);
    // Cria a thread escritora
    pthread_create(&writer, NULL, writer_thread, NULL);

    // Espera as threads terminarem
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    printf("Comunicação finalizada.\n");
    return 0;
}
