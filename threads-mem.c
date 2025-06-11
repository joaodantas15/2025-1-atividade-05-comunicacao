#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// --- Memória Compartilhada Global ---
char shared_memory[256];
int data_ready = 0; // Flag para sincronização
// ------------------------------------

void* writer_thread(void* arg) {
    printf("Writer Thread: Escrevendo na memória compartilhada...\n");
    sleep(1); // Simula algum processamento
    strcpy(shared_memory, "Olá, threads via memória!");
    data_ready = 1; // Sinaliza que os dados estão prontos
    return NULL;
}

void* reader_thread(void* arg) {
    printf("Reader Thread: Esperando por dados...\n");

    // Espera até que a flag 'data_ready' seja 1
    while (data_ready == 0) {
        usleep(10000);
    }

    printf("Reader Thread: Mensagem lida: \"%s\"\n", shared_memory);
    return NULL;
}

int main() {
    pthread_t writer, reader;

    printf("Iniciando comunicação entre threads via memória compartilhada...\n");

    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_create(&writer, NULL, writer_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    printf("Comunicação finalizada.\n");
    return 0;
}
