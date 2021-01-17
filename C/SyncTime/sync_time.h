#ifndef __CURRENT_TIME_H__
#define __CURRENT_TIME_H__

#include <stdint.h>

 //北京时间零点更新
// #define AUTO_REBOOT_HOUR 16

typedef struct tag_SysTime{
      uint16_t  year;              //年
      uint8_t   month;            //月
      uint8_t   day;              //日    
      uint8_t   hour;              //小时
      uint8_t   min;              //分
      uint8_t   sec;              //秒
      uint32_t  ticks;            //ticks 与使用的系统时钟相关   
} sysTime_t;


sysTime_t GetCurrentTime();
uint32_t GetCurrentTime_Sec();
void  PrintCurrentTime();
void  printCurrentTime_GMT_Zh();


void DateToMSec(sysTime_t data, uint32_t *psec, uint32_t *pticks);

sysTime_t MSecToDate(uint32_t sec,uint32_t ticks);
bool SyncSecTime(uint32_t sec);
bool SyncRTCTimer(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
void RefreshSysTime();
// void RefreshSysTime_Zh();

bool IsSyncRTCTimer();


#endif