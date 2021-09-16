#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t *forks;
int num_of_phllosophers = 0;

void *philosofer_logic(void *id);

int main(int argc, const char *argv[])
{
    // seed the random function
    srand(time(NULL));

    // input validation
    if (argc != 2)
    {
        fprintf(stderr, "Usage: <number of philosophers>\n");
        exit(1);
    }
    char *ptr;
    num_of_phllosophers = (int)strtol(argv[1], &ptr, 10);
    if (num_of_phllosophers < 2)
    {
        fprintf(stderr, "Number of philosophers should be greather than 2\n");
        exit(1);
    }

    // array to save all of the threads
    pthread_t *phllosophers;
    if (!(phllosophers = malloc(sizeof(pthread_t) * num_of_phllosophers)))
    {
        perror("malloc");
        exit(1);
    }

    // array to save all of the forks
    if (!(forks = malloc(sizeof(pthread_mutex_t) * num_of_phllosophers)))
    {
        perror("malloc");
        exit(1);
    }
    // init forks
    for (int i = 0; i < num_of_phllosophers; i++)
    {
        if (pthread_mutex_init(&forks[i], NULL))
        {
            perror("pthread_mutex_init");
            exit(1);
        }
    }

    // creates threads representing philosophers
    int *arg;
    for (int i = 0; i < num_of_phllosophers; i++)
    {
        if(!(arg = malloc(sizeof(int))))
        {
            perror("malloc");
            exit(1);
        }
        *arg = i + 1;
        if (pthread_create(&phllosophers[i], NULL, philosofer_logic, arg))
        {
            perror("pthread_create()");
            return 1;
        }
    }

    /*
        Infinite loop so there is no need to thread join / destroy / free memory
    */
    
    return 0;
}

void *philosofer_logic(void *_id)
{
    int r = 0;
    int id = *((int *)_id);
    int fork_to_lift = -1;
    free(_id);
    for (;;)
    {
        // Try to lift the first fork - with the smaller number
        fork_to_lift = id - 1 > id % num_of_phllosophers ? id % num_of_phllosophers : id - 1;
        printf("%d: Trying to lift first fork\n", id);
        if(pthread_mutex_lock(&forks[fork_to_lift]))
        {
            perror("pthread_mutex_lock");
            exit(1);
        }
        printf("%d: First fork lifted\n", id);
        // Try to lift the second fork - with the greater number
        fork_to_lift = id - 1 < id % num_of_phllosophers ? id % num_of_phllosophers : id - 1;
        printf("%d: Trying to lift second fork\n", id);
        if(pthread_mutex_lock(&forks[fork_to_lift]))
        {
            perror("pthread_mutex_lock");
            exit(1);
        }
        printf("%d: Second fork lifted\n", id);
        // Forks lifted - starting to eat random number of seconds (1-10)
        r = (rand() % 10) + 1;
        printf("%d: Starting to eat for %d seconds\n", id, r);
        sleep(r);
        printf("%d: Done eatting for %d seconds\n", id, r);
        // Done eating release forks and go to sleep for random number of seconds (10-20)
        if(pthread_mutex_unlock(&forks[id - 1]))
        {
            perror("pthread_mutex_unlock");
            exit(1);
        }
        if(pthread_mutex_unlock(&forks[id % num_of_phllosophers]))
        {
            perror("pthread_mutex_unlock");
            exit(1);
        }
        r = (rand() % 11) + 10;
        printf("%d: Start thinking for %d seconds\n", id, r);
        sleep(r);
        printf("%d: Done thinking for %d seconds\n", id, r);
    }
}
