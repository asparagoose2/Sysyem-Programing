#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct min_value_args
{
  int size;
  const char** arr;
  int min;
};

pthread_t flag;

struct min_value_args A;
struct min_value_args B;

void* minValue()
{
    
    if(flag == pthread_self())
    {
        int min = INT_MAX;
        char* p;
        int temp;
        for (int i = 0; i < A.size; i++)
        {
            temp = strtol(A.arr[i], &p, 10);
            if (temp < min)
                min = temp;
        }
        A.min = min;
    } 
    else
    {
        int min = INT_MAX;
        char* p;
        int temp;
        for (int i = 0; i < B.size; i++)
        {
            temp = strtol(B.arr[i], &p, 10);
            if (temp < min)
                min = temp;
        }
        B.min = min;
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        fprintf (stderr,"No numbers were provided!\n");
        exit(1);
    }
    pthread_t tid;

    void* ret; 

    if((argc - 1) % 2)
    {
        A.size = (argc-1) / 2;
        B.size = ((argc-1) / 2) + 1;
    }
    else
    {
        A.size = B.size = (argc-1) / 2;
    }
    A.arr = argv + 1;
    B.arr = argv + 1 + A.size;

    if (pthread_create(&flag, NULL, minValue, (void*)&A))
        return 1;

    if (pthread_create(&tid, NULL, minValue, (void*)&B))
        return 1;


    if (pthread_join(flag, &ret))
    {
        fprintf(stderr,"Could not join A Thread\n");
        return 1;
    }

    if (pthread_join(tid, &ret))
    {
        fprintf(stderr,"Could not join B Thread\n");
        return 1;
    }

    // I could have used the return value of minValue (make it return the pointer to the relavant struct),
    // It is simpler that way in my opinion.
    printf("Min Value is: %d\n", A.min < B.min? A.min : B.min);
    
    return 0;
}
