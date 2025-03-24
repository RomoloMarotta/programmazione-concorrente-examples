#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


typedef struct {
    unsigned long long start;
    unsigned long long end;
    unsigned long long prime_count;
    unsigned long long test_count;
} ThreadData;

int is_prime(unsigned long long num, unsigned long long *test_count) {
    if (num < 2) return 0;
    if (num == 2 || num == 3) return 1;
    if (num % 2 == 0) return 0;

    *test_count += 1;  // Primo test (escludere i numeri pari)

    for (unsigned long long i = 3; i <= sqrt(num); i += 2) {
        *test_count += 1;  // Ogni iterazione è un test di divisibilità
        if (num % i == 0) return 0;
    }
    return 1;
}

void *check_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->prime_count = 0;
    data->test_count = 0;

    for (unsigned long long i = data->start; i <= data->end; i++) {
        unsigned long long test_count = 0;
        if (is_prime(i, &test_count)) {
            data->prime_count++;
        }
        data->test_count += test_count;
    }

    // Stampa i risultati del thread
    printf("Thread [%llu - %llu] -> Primi trovati: %llu, Test effettuati: %llu\n",
           data->start, data->end, data->prime_count, data->test_count);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <N> <NUM_THREADS>\n", argv[0]);
        return 1;
    }

    unsigned long long N = strtoull(argv[1], NULL, 10);
    unsigned long long NUM_THREADS = strtoull(argv[2], NULL, 10);

    if (N < 1 || NUM_THREADS < 1) {
        printf("Errore: N e il numero di thread devono essere maggiori di 0.\n");
        return 1;
    }

    pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));
    ThreadData *thread_data = malloc(NUM_THREADS * sizeof(ThreadData));
    unsigned long long prime_count = 0;
    unsigned long long test_count  = 0;
    unsigned long long chunk_size = (N + NUM_THREADS - 1) / NUM_THREADS;

    for (unsigned long long i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i + 1) * chunk_size;
        if (thread_data[i].end > N) thread_data[i].end = N;
        thread_data[i].prime_count = 0;
        thread_data[i].test_count = 0;

        pthread_create(&threads[i], NULL, check_primes, &thread_data[i]);
    }

    for (unsigned long long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        prime_count += thread_data[i].prime_count;
        test_count += thread_data[i].test_count;
    }


    printf("Totale numeri primi trovati: %llu\n", prime_count);
    printf("Totale test di divisibilità effettuati: %llu\n", test_count);


    free(threads);
    free(thread_data);
    return 0;
}

