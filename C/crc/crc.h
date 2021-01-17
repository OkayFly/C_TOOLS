/*********************************************************************************
  *(C) 2003-2017  honestar Inc. All rights reserved.
  *FileName: uart_crc.h
  *Author:  james.du@honestar.com
  *Telephone 18628243172
  *Version:  1.0.0
  *Date:  2018.8.9
  *Description:  crc 8 ʹ�ö���ʽ��x8+x5+x4+1��������Ϊ��100110001��
   *crc32  ʹ�ö���ʽ       X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
  *Others:  
  *Function List:  
  *History:  
**********************************************************************************/
#ifndef __UART_CRC_H__
#define __UART_CRC_H__
#include "utility.h"
#ifdef __cplusplus
extern "C" {
#endif

U8 cal_crc8_table(U8 *ptr, U32 len);
//U32 cal_crc32(U32 crc, const void *buf, size_t size);
U32 cal_crc32(U32 crc, U8 *buf, U32 size);

#ifdef __cplusplus
}
#endif
#endif