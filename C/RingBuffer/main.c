/**
*********************************************************************************
 *(C) @@.        [fly] All Rights Reserved.
 *FileName:      main.c
 *Author:        fly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2020.12.15
 *Description:   
 *Others:        
 *History:       
**********************************************************************************
*/


#include <stdio.h>
#include "user_buffer.h"

#define MY_BUFFER_COUNT (int)  1800
#define MY_BUFFER_HWM   (int)   850
#define MY_BUFFER_LWM   (int)   650
RingBuffer my_buffer;


int main()
{
    printf("RingBuffer test\n");
    if(!user_buffer_initialized(&my_buffer))
    {
        printf("** user buffer create--->\n");
        user_buffer_create(&my_buffer, MY_BUFFER_COUNT, MY_BUFFER_LWM, MY_BUFFER_HWM);

    }

}