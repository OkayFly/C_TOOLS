#include <stdio.h>
#include <stdarg.h>

int msm_printf(const char* format, ...) {
    va_list ap;
    int ret;

    va_start(ap,format);

    ret = vprintf(format, ap);

    va_end(ap);

    return ret;
}

int main() 
{
    msm_printf("xx","x%d\n",1);
}
