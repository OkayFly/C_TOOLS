#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


// 1: movingaveragefilter
// 2: weightedfilter
float true_value = -50.0;

#define Max_Moving_Window  20
float values[Max_Moving_Window] = {0.0f};
int Current_Window_Data_Count = 0; 

float moving_average_filter(float in);


//************** 有局限性 为加权限幅********************//
// 
int K = Max_Moving_Window / 2; //权值
//#define Min_Valid_Value  -1000； //最小有效指D0
float D0 = -1000;//Min_Valid_Value
float Dmean = 0.0; //均值
float Dj = 0.0; // 4/3 * Dmean; //[D0  Dj] 
float Dnj = 0.0; // -4/3 *Dmean [Dnj Dj]正常值;
float Df = 0.0; // 2 * Dmean  [Df ] 不正常 用Dmean代替
float Dnf = 0.0; // -2 * Dmean [ -Df]不正常 用Dmean代替
float weighted_limited_filter(float in);

int main()
{

    srand(  (unsigned)(time(NULL)));
    float out_vaules[1000] = {0.0f};

    for(int i=0; i<1000; i++)
    {
        float tmp_value = true_value + rand() % (20)-10;
        
        out_vaules[i] = weighted_limited_filter(tmp_value);
        printf("data[%f] ->  [%f]\n", tmp_value,  out_vaules[i]);

    }


    return 0;
}

float moving_average_filter(float in)
{
    float value_sum = 0.0f;

    //save data
    if( Current_Window_Data_Count < Max_Moving_Window)
    {
        values[Current_Window_Data_Count++] = in;
        for(int i=0; i< Current_Window_Data_Count; i++)
        {
            value_sum += values[i];
        }
    }
    else
    {
        for(int i =0; i < Max_Moving_Window-1; i++)
        {
            values[i] = values[i+1];
            value_sum += values[i];
        }
        values[Max_Moving_Window-1] = in;
        value_sum += values[Max_Moving_Window-1];
    }
    
    return value_sum / Current_Window_Data_Count;

}


float weighted_limited_filter(float in)
{

    if( in < D0)
        return Dmean;

    float value_sum = 0.0f;

    //save data
    if( Current_Window_Data_Count < Max_Moving_Window)
    {
        values[Current_Window_Data_Count++] = in;
        for(int i=0; i< Current_Window_Data_Count; i++)
        {
            value_sum += values[i];
        }
        Dmean = value_sum / Current_Window_Data_Count;
    }
    else
    {

        for(int i =0; i < Max_Moving_Window-1; i++)
        {
            values[i] = values[i+1];
            value_sum += values[i];
        }
        Dj = 4.0 / 3.0 *  fabs(Dmean);
        Dnj = -Dj;
        Df = 2.0 * fabs(Dmean);
        Dnf = -2 * Df;

        if(  (in > Dj && in <Df) || (in < Dnj && in > Dnf))
            in = ( in * K + Dmean * (Max_Moving_Window - K)) / Max_Moving_Window;
        else
            in = Dmean;



        values[Max_Moving_Window-1] = in;
        value_sum += values[Max_Moving_Window-1];
        Dmean = value_sum / Current_Window_Data_Count;

    }
    
  

    return Dmean;
}