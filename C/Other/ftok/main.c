#include <stdio.h>
#include <sys/sem.h>

#include <stdlib.h>

int main()
{
    key_t semkey;
    if((semkey = ftok("./test",2)) <0)
    {
        printf("ftok failed\n");
        exit(EXIT_FAILURE);
    }
    printf("ftok ok, semky = %d\n",semkey);
    return 0;

}

