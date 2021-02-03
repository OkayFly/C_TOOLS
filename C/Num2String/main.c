#include <stdio.h>

int main()
{
    printf("Hello World!\n");
    float a = -6.1110;

    int sig = 5;//significant digits
    char temp_str[25] = {'\0'};
    gcvt(a, sig, temp_str);



    printf("string=%s\n", temp_str);

    printf("len:%d\n", strlen(temp_str));
    return 0;
}
