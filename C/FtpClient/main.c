#include <stdio.h>

#include "mftp_client.h"

int main(int argc, const char* argv[])
{
	printf("Test:----->\n");
    do_firmware_upgrade_ftp("1",615792,1);
    return 0;
}
