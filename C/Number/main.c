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



int main()
{
    printf("Number test\n");

	signed char a = -62;
	printf("a:%d\n",a);
	printf("a:%x\n",a);
	unsigned char aa= (unsigned char)a;
    signed char sa = (signed char)a;
    printf("a:[%d, %x]\n", sa, sa);
	
	printf("aa:%d\n",aa);
	printf("aa:%x\n",aa);
	signed char a1 = -92;
	printf("a1:%d\n",a1);
	printf("a1:%x\n",a1);
	unsigned char aa1= (unsigned char)a1;
	
	printf("aa1:%d\n",aa1);
	printf("aa1:%x\n",aa1);

}