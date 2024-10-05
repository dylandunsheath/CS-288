#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

typedef struct
{
    int num_threads;
    long *id;
    long n;
    long double *pi_approx;
} thread_data;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void CalcPiAprox(void *threadarg)
{
    // Get the thread number
    long thread_num = ((thread_data *)threadarg)->id[counter];
    counter++;
    // Get the number of terms
    long n = ((thread_data *)threadarg)->n;
    // Get the number of threads
    int num_threads = ((thread_data *)threadarg)->num_threads;
    // Get the array of approximations
    long double *pi_approx = ((thread_data *)threadarg)->pi_approx;

    // Calculate the number of terms for each thread
    long double x;
    long double partial_sum = 0.0;
    for (int i = n / num_threads * thread_num + 1; i <= n / num_threads * (thread_num + 1); i++)
    {
        // Calculate the approximation
        x = ((double)i - 0.5) / n;
        partial_sum += 4.0 / (1.0 + x * x);
    }
    pi_approx[thread_num] = partial_sum;
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    if (argc != 3) {
       printf("ERROR: Program must be of the following format: ./executableName <# of threads> <# of terms>\n");
       return 1;
    }
    long double PI = 3.14159265358979323846264338327950288419716939937510;
    // Create a thread_data struct
    thread_data *data = malloc(sizeof(thread_data));
    // Get the number of threads from the command line
    data->num_threads = atoi(argv[1]);
    // Get number of terms  from command line
    data -> n = atoi(argv[2]);
    // Create an array to store the approximations
    data->pi_approx = malloc(data->num_threads * sizeof(long double));
    // Create an array of threads
    pthread_t *threads = malloc(data->num_threads * sizeof(pthread_t));
    // Create array of thread numbers
    data->id = malloc(data->num_threads * sizeof(long));
    // Create the threads
    for (int i = 0; i < data->num_threads; i++)
    {
        data->id[i] = i;
        pthread_create(&threads[i], NULL, (void *)CalcPiAprox, (void *)data);
    }
    // Wait for the threads to finish
    for (int i = 0; i < data->num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // Calculate the final approximation
    long double pi_approx = 0;
    for (int i = 0; i < data->num_threads; i++)
    {
        pi_approx += data->pi_approx[i];
    }
    long double pi = pi_approx / data->n;
    // Print the final approximation
    printf("Approx Value of pi = %.22Lf\nExact Value of pi = %.22Lf\nError is %.22LF\n", pi, PI, fabsl(pi - PI));
    return 0;
}
