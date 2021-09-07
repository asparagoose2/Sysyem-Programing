#include <stdio.h>
#include <stdlib.h>

// initialized by the os - must use this exact name
extern char** environ;

int main(int argc, char const *argv[])
{
    for(int i = 0; environ[i] != NULL; ++i)
    {
        printf("%s\n", environ[i]);
    }
    printf("\n\nHOME: %s\n", getenv("HOME"));
    return 0;
}
