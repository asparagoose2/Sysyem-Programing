#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>




pthread_mutex_t token = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t *forks;
int num_of_phllosophers = 0;

// pthread_cond_wait

pthread_mutex_t main_dish = PTHREAD_MUTEX_INITIALIZER;

void *philosofer_logic(void *id);

void request_token(int id);

void *waiter(void *id);

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
    if (num_of_phllosophers < 1)
    {
        fprintf(stderr, "Number of philosophers should be greather than 1\n");
        exit(1);
    }

    // array to save all of the threads
    pthread_t *phllosophers;
    if(!(phllosophers = malloc(sizeof(pthread_t) * num_of_phllosophers)))
    {
        perror("malloc");
        exit(1);
    }

    // array to save all of the forks
    if(!(forks = malloc(sizeof(pthread_mutex_t) * num_of_phllosophers)))
    {
        perror("malloc");
        exit(1);
    }
    // init forks
    for(int i = 0; i < num_of_phllosophers; i++)
    {
        if(pthread_mutex_init(&forks[i], NULL))
        {
            perror("pthread_mutex_init");
            exit(1);
        }
    }

    // creates threads representing philosophers
    int *arg;
    for (int i = 0; i < num_of_phllosophers; i++)
    {
        arg = malloc(sizeof(int));
        *arg = i + 1;
        if (pthread_create(&phllosophers[i], NULL, philosofer_logic, arg))
        {
            perror("pthread_create()");
            return 1;
        }
    }

    // joining all off the threads
    for (int i = 0; i < num_of_phllosophers; i++)
        pthread_join(phllosophers[i], NULL);
    return 0;
}

void *philosofer_logic(void *_id)
{
    int r = (rand() % 10) + 1;
    int id = *((int *)_id);
    free(_id);
    for (;;)
    {
        // try to get the main dish (wait untill you get it)
        pthread_mutex_lock(&main_dish);
        printf("%d: Trying to lift forks\n", id);
        // try to get the lift fork (don't wait if unavilable)
        if(!pthread_mutex_trylock(&forks[id - 1]))
        {
            printf("%d: Left fork lifted\n", id);
            // if left fork lifted try to luft the right fork (don't wait if unavilable);
            if(!pthread_mutex_trylock(&forks[id%num_of_phllosophers]))
            {
                // release the main dish and eat for random anout of seconds (0-20)
                printf("%d: Right fork lifted\n", id);
                printf("%d: Starting to eat for %d seconds\n", id, r);
                pthread_mutex_unlock(&main_dish);
                sleep(r);
                r = (rand() % 10) + 10;
                // after done eating release forks and go to sleep for random amout of time
                pthread_mutex_unlock(&forks[id - 1]);
                pthread_mutex_unlock(&forks[id % num_of_phllosophers]);
                printf("%d: Done eating, Going to sleep %d seconds\n", id,r);
                sleep(r);
                r = (rand() % 10) + 1;
            }
            // if right fork is unavilable, release main dish and left fork and try agin
            else 
            {
                printf("%d: Could not lift right fork\n", id);
                pthread_mutex_unlock(&forks[id - 1]);
                pthread_mutex_unlock(&main_dish);
            }
        }
        // if left fork is unavilable realese main dish and try agin
        else
        {
            printf("%d: Could not lift left fork\n", id);
            pthread_mutex_unlock(&main_dish);
        }
    }
}
