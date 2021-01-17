#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER; 


int All = 0;
int A1 = 0;
int A2 = 0;
int A3 = 0;

int cirl1 =0, cirl2 =0, cirl3=0, cirl =0;

int myStatus[3] = {0, 0, 0};

int Allmut = 0;


// int x=1, y=20;

#define msleep(x) usleep(x*1000) 

void *thread1_tt(void *parameter){
    while(1){
        cirl1++;
        int tempA =0;
        myStatus[1] = 1;
        //logon
        pthread_mutex_lock(&mut1); //互斥锁
        printf("*call thread1  mutex \n");
        printf("*[1 %d %d\n", cirl1, A1);
        tempA = A1;
        pthread_mutex_unlock(&mut1);
        myStatus[1] = 0;


        msleep(10);
        printf("*[1 %d %d\n", cirl1, A1);
        if(A1 == tempA && Allmut==1)
            printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF cirl1:%d, tmepA:%d, A1:%d\n",cirl1, tempA, A1);
        printf("*call thread1  end \n");


		
    }
}


void *thread2_tt(void*parameter){
    while(1)
    {
        cirl2++;
        int tempA =0;
        myStatus[2] = 1;
        pthread_mutex_lock(&mut2); //互斥锁
        printf("**call thread2  mutex \n");
        printf("**[2 %d %d\n", cirl2, A2);
        tempA = A2;
        pthread_mutex_unlock(&mut2);
        myStatus[2] = 0;

        msleep(10);
        printf("**[2 %d %d\n", cirl2, A2);
        if(A2 == tempA && Allmut==1)
            printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF cirl2:%d, tmepA:%d, A2:%d\n",cirl2, tempA, A2);
        printf("**call thread2  end \n");
	
    }
} 


void *thread3_tt(void*parameter){
    while(1)
    {
        cirl3++;
        int tempA =0;
        myStatus[3] = 1;
        pthread_mutex_lock(&mut3); //互斥锁
        printf("***call thread3  mutex \n");
        printf("***[3 %d %d\n", cirl3, A3);
        tempA = A3;
        pthread_mutex_unlock(&mut3);
        myStatus[3] = 0;
        

        msleep(10);
        printf("***[3 %d %d\n", cirl3, A3);
        if(A3 == tempA && Allmut ==1)
            printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF cirl3:%d, tmepA:%d, A3:%d\n",cirl3, tempA, A3);
        printf("***call thread3  end \n");
	
    }
} 



void *thread_tt(void*parameter){
    while(1)
    {
        cirl++;
        pthread_mutex_lock(&mut1); //互斥锁
        pthread_mutex_lock(&mut3); //互斥锁
        pthread_mutex_lock(&mut2); //互斥锁
        Allmut =1;
        printf("\n\ncall MMMMMMMM  mutex \n");
        //msleep(1);
        A1++;
        A2++;
        A3++;
        All = A1+A2+A3;
        printf("[ALL  %d %d\n", cirl, All);
        
        pthread_mutex_unlock(&mut1);
        pthread_mutex_unlock(&mut2);
        pthread_mutex_unlock(&mut3);
        Allmut = 0;
        msleep(2);
        printf("\n\ncall MMMMMMMMMM end \n");
	
    }
} 

// void *thread3_tt(void*parameter){
//     while(1)
//     {

//         pthread_mutex_lock(&mut);
//         printf("\n\ncall thread2 \n");
//         while(i%10 != 0)//惊群效应
//         {
//             printf("pthread_cond_wait\n");
//             pthread_cond_wait(&cond, &mut);
//             //用于阻塞当前线程，等待别的线程使用 pthread_cond_signal() 或pthread_cond_broadcast来唤醒
//         }
       
//         printf("thread222\n");
//         printf("x:%d, y:%d\n",x, y);

//         pthread_mutex_unlock(&mut);
//          sleep(1);
//         // pthread_mutex_lock(&mut);
//         // printf("\n\ncall thread2 \n");
//         // while(i%3 != 0)
//         //     pthread_cond_wait(&cond,&mut); //wait signal from p1
        
// 		// printf("p2: %d step1\n",i);
//         // pthread_mutex_unlock(&mut);
 
// 		// printf("p2: sleep i=%d step2\n", i);
//         // sleep(1);
// 		// printf("p2: sleep i=%d step3\n", i);		
//     }
// } 


// void* thread1_t(void* param) //void  与传会的值
// {
//     while(1)
//     {
         
//         pthread_mutex_lock(&mut);
//         printf("thread1\n");
//         while(x%y != 0)//惊群效应
//         {
//             printf("pthread_cond_wait\n");
//             // printf("thread1\n");
//             // printf("x:%d, y:%d\n",x, y);
//             pthread_cond_wait(&cond, &mut);
//             //用于阻塞当前线程，等待别的线程使用 pthread_cond_signal() 或pthread_cond_broadcast来唤醒
//         }
       
//         printf("thread1\n");
//         printf("x:%d, y:%d\n",x, y);

//         pthread_mutex_unlock(&mut);
//         sleep(1);
//     }
//     return NULL;
// }

// void* thread2_t(void* param)
// {
    
//     while(1)
//     {
       
//         pthread_mutex_lock(&mut);
//         printf("thead2\n");
         
//         // x++;
//         if(x%y == 0)
//         {
//             // pthread_cond_broadcast(&cond);
//             printf("pthread_cond_signal\n");
//             pthread_cond_signal(&cond);
//             //pthread_cond_signal函数的作用是发送一个信号给另外一个正在处于阻塞等待状态的线程,使其脱离阻塞状态,继续执行.如果没有线程处在阻塞等待状态,pthread_cond_signal也会成功返回。
//         }
//       x++; //TODO(Zhangx) 
//        printf("!!!!!!!!!!!x:%d\n",x);
//         pthread_mutex_unlock(&mut);
//          sleep(1);
//     }
    
//     return NULL;
// }

int main()
{
    printf("Mutipthread test\n");
    pthread_t thread1, thread2, thread3, thread4;
    void* pthread_value1, *pthread_value2, *pthread_value3, *pthread_value4;
    pthread_create(&thread1, NULL, thread_tt, NULL);

    pthread_create(&thread2, NULL, thread1_tt, NULL);
    pthread_create(&thread3, NULL, thread2_tt, NULL);
    pthread_create(&thread4, NULL, thread3_tt, NULL);

    pthread_join(thread1, &pthread_value1);
    pthread_join(thread2, &pthread_value2);
    pthread_join(thread3, &pthread_value3);
    pthread_join(thread4, &pthread_value4);


    return 0;
}