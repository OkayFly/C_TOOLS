#include "sync_time.h"
#include <stdbool.h>


static const char *TAG = "CURRENT_TIME";
//不同平台配置区
//时间片  us 为单位 (frize 100)
#define SLICE_TIME          (10000)
//获取时间接口
// #define GET_TIME_TICKS                  (lld_evt_time_get)
//#define ELAPS_TICKS(begin,end)         (time_abs27bit(begin,end))

//ticks转化为秒
#define OS_TICKS_2_SEC(x)    ( ( (x)) / (1000000/SLICE_TIME))
//ticks转化为秒剩余
#define OS_TICKS_2_SEC_REM(x)    ( ( (x)) % (1000000/SLICE_TIME))
//ticks转化为毫秒
#define OS_TICKS_2_MS(x)    ( ( (x) * SLICE_TIME ) / 1000)

//起始时间戳
static const sysTime_t kConstTime = {1970, 1, 1, 0, 0, 0, 0};
static sysTime_t kCurrentTime = {1970,1,1,0,0,0,0};
static uint32_t kStartTick;


static uint32_t ElapseTicks(uint32_t start_tick, uint32_t cur_tick)
{
    return cur_tick - start_tick;
}

//判断是否是闰年  是 return:true 
static bool CheckIsRunYear(const uint16_t year)
{
    bool bRet = false;
    if( ((0 == year%4) && (year%100 != 0)) || (0 == year%400))
      bRet = true;

    return bRet;
}


sysTime_t GetCurrentTime()
{
    sysTime_t curTime;
    RefreshSysTime();
    curTime = kCurrentTime;
    return curTime;
}

uint32_t GetCurrentTime_Sec()
{
    sysTime_t curTime;
    RefreshSysTime();
    curTime = kCurrentTime;
    uint32_t pSec;
    uint32_t pTicks;

    DateToMSec( curTime,  &pSec,  &pTicks);
    return pSec;
}

static  bool kHasSyncRTCTimer = false;
void PrintCurrentTime() 
{
  if( kHasSyncRTCTimer) {
    sysTime_t current_date = GetCurrentTime();
    ESP_LOGI(TAG," ...%u_%u_%u: %u:%u:%u->ticks[%u]...", current_date.year, current_date.month, current_date.day,
                                            current_date.hour, current_date.min, current_date.sec, current_date.ticks);
  }
  else {
    ESP_LOGI(TAG," ... Run ticks[%u]...", xTaskGetTickCount());;
  }
}


// void printCurrentTime_Zh()
// {
//     if( kHasSyncRTCTimer) {
//     sysTime_t current_date = GetCurrentTime();

//     //printf("\n ...%u_%u_%u: %u:%u:%u->ticks[%u]...", current_date.year, current_date.month, current_date.day,
//                                             // current_date.hour, current_date.min, current_date.sec, current_date.ticks);
//   }
//   else {
//     //printf("\n ... Run ticks[%u]...", xTaskGetTickCount());;
//   }
// }

void RefreshSysTime()
{
    uint32_t cur_tick = xTaskGetTickCount();
    uint32_t elapse_ticks = ElapseTicks(kStartTick, cur_tick);

    uint32_t DateSec = 0;
    uint32_t rem_ticks = 0;
    DateToMSec(kCurrentTime, &DateSec, &rem_ticks);
    uint32_t sum_ticks = elapse_ticks + rem_ticks;

    DateSec += OS_TICKS_2_SEC(sum_ticks);
    rem_ticks = OS_TICKS_2_SEC_REM(sum_ticks);
    kCurrentTime = MSecToDate(DateSec, rem_ticks);

    kStartTick = cur_tick;
}

void RefreshSysTime_Zh()
{
  ;
}
//日期转换函数
void DateToMSec(sysTime_t date, uint32_t *psec, uint32_t *pticks)
{
    uint32_t retSec = 0;
    uint32_t RunYearNum = 0;
    if ((date.year < kConstTime.year) || (psec == NULL) || (pticks == NULL))
    {
        return;
    }

    uint16_t year = date.year - kConstTime.year;
    //计算共经过了多少个闰年
    for (uint32_t i = kConstTime.year; i < date.year; i++)
    {
        if (CheckIsRunYear(i))
        {
            RunYearNum++;
        }
    }
    //处理天
    uint8_t monthMax[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //判断最后一年是否闰年
    if (CheckIsRunYear(date.year))
        monthMax[1] = 29;
    uint32_t mon = date.month - kConstTime.month;
    uint16_t sunMontDay = 0;
    for (int i = 0; i < mon; i++)
    {
        sunMontDay += monthMax[i];
    }

    uint32_t day = date.day - kConstTime.day;
    uint32_t SumDay = year * 365 + RunYearNum + sunMontDay + day;
    uint16_t hour = date.hour - kConstTime.hour;
    uint16_t min = date.min - kConstTime.min;
    uint16_t sec = date.sec - kConstTime.sec;
    uint32_t ticks = date.ticks;

    retSec = (SumDay * 3600 * 24 + hour * 3600 + min * 60 + sec);
    *psec = retSec;
    *pticks = ticks;
}


//毫秒转换成日期   sec:总秒数         ticks:剩余的整除整秒后剩余的ticks
sysTime_t MSecToDate(uint32_t sec,uint32_t ticks)
{
  sysTime_t date = kConstTime;
  const uint32_t DaySum = 3600 * 24;

  uint32_t HaveDay = sec / DaySum;
  uint32_t leftSec = sec - HaveDay*DaySum;

  bool runFlag = CheckIsRunYear(date.year);

  while(HaveDay >= (365 + runFlag))
  {
    date.year += 1;
    HaveDay -= (365 + runFlag);
    runFlag = CheckIsRunYear(date.year);
    
  }

  //处理天
  uint8_t monthMax[12]={31,28,31,30,31,30,31,31,30,31,30,31};

  //判断最后一年是否闰年
  if( CheckIsRunYear(date.year) )
    monthMax[1]   = 29;

  uint8_t compDay = monthMax[0];
  uint8_t mon = kConstTime.month;
  
  while( (mon<=12) && (HaveDay>=compDay) )
  {
    HaveDay -= monthMax[mon-1];
    date.month = mon+1;
    mon++;
    if(mon>=12)
      break;
    else
      compDay = monthMax[mon-1];
  }

  date.day += HaveDay;

  date.hour   = leftSec / 3600;
  date.min     = (leftSec - date.hour*3600)/60;
  date.sec     = leftSec - date.hour*3600 - date.min*60;
  date.ticks   = ticks;
  return date;
}

static bool check_time_legal(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
    bool ret = true;
    uint8_t monthMax[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    
    ret &= (year>=kConstTime.year)&&(month>0&&month<=12)&&(minute<60)&&(second<60);
    if(ret)
    {
        //判断是否为闰年
        if( CheckIsRunYear(year) )
        {
          monthMax[1]   = 29;
          ret &= (day>0 && day<=monthMax[month-1]);
        }
    }
    return ret;
}


//按照秒同步时间   同步成功:return true
bool SyncSecTime(uint32_t sec)
{
  sysTime_t t = MSecToDate(sec,0);
  return SyncRTCTimer(t.year,t.month,t.day,t.hour,t.min,t.sec);
}
bool IsSyncRTCTimer()
{
  return kHasSyncRTCTimer;
}


//同步RTC时间  同步成功:return true
bool SyncRTCTimer(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{


  printf("tttt, year:%u\n" , year);
   printf("tttt, month:%u\n" , month);
    printf("tttt, day:%u\n" , day);
     printf("tttt, hour:%u\n" , hour);
      printf("tttt, min:%u\n" , minute);
       printf("tttt, sec:%u\n" , second);


  bool bRet = true;

  if(check_time_legal(year,month,day,hour,minute,second)){
    kCurrentTime.year   = year;
    kCurrentTime.month  = month;
    kCurrentTime.day        = day;
    kCurrentTime.hour       = hour;
    kCurrentTime.min        = minute;
    kCurrentTime.sec        = second;
    kCurrentTime.ticks      = 0;

    kStartTick = xTaskGetTickCount();
    kHasSyncRTCTimer = true;
  }
  else
      bRet = false;
  return bRet;
}
