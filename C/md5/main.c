#include <stdio.h>
#include "md5.h"

int main()
{
    printf("MD5 test!\n");
    char topic_emit[100];

    char topic0[33] = {'\0'};
    char topic_mac[33]  = {'\0'};
    MD5Create((unsigned char*) ("bee-CPRICE"), topic0);
    MD5Create((unsigned char*)("3c 71 66 aa 27 93"), topic_mac);

    sprintf(topic_emit, "/BeesmartEmit/%s/%s\n", topic0, topic_mac);
    printf("\n\t%s",topic_emit);
    fflush((stdout));
    return 0;
}
