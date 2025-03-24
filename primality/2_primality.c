#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_N 1000000000  // Valore massimo gestibile

unsigned long long N;  // Numero massimo fino a cui cercare primi
unsigned long long current_number = 1;  // Contatore globale
unsigned long long prime_count = 0;  // Contatore globale per il numero di primi trovati
unsigned long long test_count = 0;  // Contatore globale per il numero di test eseguiti

pthread_spinlock_t lock;  // Spinlock per proteggere il contatore

int is_prime(unsigned long long num, unsigned long long *tests_done) {
    if (num < 2) return 0;
    if (num == 2 || num == 3) return 1;
    if (num % 2 == 0) return 0;

    *tests_done += 1;  // Primo test (escludere i numeri pari)

    for (unsigned long long i = 3; i <= sqrt(num); i += 2) {
        *tests_done += 1;  // Ogni iterazione è un test di divisibilità
        if (num % i == 0) return 0;
    }
    return 1;
}

void *check_primes(void *arg) {
    unsigned long long local_prime_count = 0;
    unsigned long long local_test_count = 0;
    unsigned long long num;

    while (1) {
        // Sezione critica: ottenere il numero da testare
        if (current_number > N) {
            break;  // Uscire se abbiamo terminato tutti i numeri
        }
        num = current_number++;

        // Controllo se il numero è primo
        unsigned long long tests_done = 0;
        if (is_prime(num, &tests_done)) {
            local_prime_count++;
        }
        local_test_count += tests_done;
    }

    // Sezione critica: aggiornare i contatori globali
    prime_count += local_prime_count;
    test_count += local_test_count;

    printf("Thread terminato -> Primi trovati: %llu, Test effettuati: %llu\n",
           local_prime_count, local_test_count);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <N> <NUM_THREADS>\n", argv[0]);
        return 1;
    }

    N = strtoull(argv[1], NULL, 10);
    unsigned long long NUM_THREADS = strtoull(argv[2], NULL, 10);

    if (N < 1 || NUM_THREADS < 1) {
        printf("Errore: N e il numero di thread devono essere maggiori di 0.\n");
        return 1;
    }

    pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));


    // Creazione dei thread
    for (unsigned long long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    // Attesa della terminazione dei thread
    for (unsigned long long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Distruzione dello spinlock
    pthread_spin_destroy(&lock);

    printf("Totale numeri primi trovati: %llu\n", prime_count);
    printf("Totale test di divisibilità effettuati: %llu\n", test_count);

    free(threads);
    return 0;
}
