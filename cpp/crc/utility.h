/*********************************************************************************
  *(C) 2003-2017  honestar Inc. All rights reserved.
  *FileName: utility.h
  *Author:  james.du@honestar.com
  *Telephone 18628243172
  *Version:  1.0.0
  *Date:  2018.5.14
  *Description: 
  *Others:  
  *Function List:  
  *History:  
**********************************************************************************/
#ifndef __UTILITY_H__
#define __UTILITY_H__
//#ifdef __cplusplus
//extern "C" {
//#endif
#include <stdio.h>
#include <string.h>

#ifndef bool
//typedef unsigned char bool;
#endif

#ifndef boolean
typedef unsigned char boolean;
#endif

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#ifndef error
#define error (-1)
#endif

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U16
typedef unsigned short int U16;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef S8
typedef signed char S8;
#endif

#ifndef S16
typedef signed short int S16;
#endif

#ifndef S32
typedef signed int S32;
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
//#ifdef __cplusplus
//}
//#endif
#endif
