#include "double_list.h"

<<<<<<< HEAD




#include <stdio.h>


// uart0 image save list


struct dl_list g_uart_list[3];





#define MAX_UART_LIST_LEN  80





/*


 * MACRO DEFINITIONS


 ****************************************************************************************


 */





/// Get the number of elements within an array


#define ARRAY_LEN(array) (sizeof((array))/sizeof((array)[0]))








static int IS_SET_BLMAC[3];


static int totalsenddev[3];





/**


 * @brief UART peripheral number


 */


typedef enum {


    UART_NUM_0 = 0x0,  /*!< UART base address 0x3ff40000*/


    UART_NUM_1 = 0x1,  /*!< UART base address 0x3ff50000*/


    UART_NUM_2 = 0x2,  /*!< UART base address 0x3ff6e000*/


    UART_NUM_MAX,


} uart_port_t;





typedef struct


{


    //now


    struct dl_list node;


    int          cmd_id;


    int          img_id;





=======
#include <stdio.h>
// uart0 image save list
struct dl_list g_uart_list[3];

#define MAX_UART_LIST_LEN  80

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/// Get the number of elements within an array
#define ARRAY_LEN(array) (sizeof((array))/sizeof((array)[0]))


static int IS_SET_BLMAC[3];
static int totalsenddev[3];

/**
 * @brief UART peripheral number
 */
typedef enum {
    UART_NUM_0 = 0x0,  /*!< UART base address 0x3ff40000*/
    UART_NUM_1 = 0x1,  /*!< UART base address 0x3ff50000*/
    UART_NUM_2 = 0x2,  /*!< UART base address 0x3ff6e000*/
    UART_NUM_MAX,
} uart_port_t;

typedef struct
{
    //now
    struct dl_list node;
    int          cmd_id;
    int          img_id;

>>>>>>> temp
}send_task_t;



<<<<<<< HEAD








static send_task_t  __cache_memory[80];


static struct dl_list s_cache_list;





void dlist_cache_init(void)


{


    dl_list_init(&s_cache_list);





    for (int i = 0; i < ARRAY_LEN(__cache_memory); i++) {


        dl_list_add(&s_cache_list, &__cache_memory[i].node);


    }





    printf("len:%d\n", dl_list_len(&s_cache_list));


}





send_task_t *dlist_cache_new(void)


{


    send_task_t *item = dl_list_first(&s_cache_list, send_task_t, node);


    if (item) {


        dl_list_del(&item->node);


    }


    


    return item;


}





send_task_t *dlist_task_new(void)


{


    send_task_t *item = (send_task_t*) malloc(sizeof(send_task_t));


    if (item) {


       printf("wfk\n");


    }


    dl_list_init(&item->node);


    


    return item;


}





void dlist_task_free(send_task_t *item)


{


    free(item);


}








void uart_manage_init()


{


    printf("%s\n",__func__);





    for(int port =0; port<UART_NUM_MAX; port++)


    {


        dl_list_init(&g_uart_list[port]);


        IS_SET_BLMAC[port] =1;


    }


    dlist_cache_init();





}





static int choose_candidate_port_by_count()


{


	int ret = -1;


    


	if (dl_list_len(&g_uart_list[0]) >= MAX_UART_LIST_LEN || dl_list_len(&g_uart_list[1]) >= MAX_UART_LIST_LEN || dl_list_len(&g_uart_list[2]) >= MAX_UART_LIST_LEN)


	{


        printf("init port[%d] img_list length[%d]\n",0, dl_list_len(&g_uart_list[0]));


        printf("init port[%d] img_list length[%d]\n",1, dl_list_len(&g_uart_list[1]));


        printf("init port[%d] img_list length[%d]\n",2, dl_list_len(&g_uart_list[2]));


	}


	else


	{


		for (int port = 0; port < UART_NUM_MAX; ++port)


		{


			if (IS_SET_BLMAC[port] == 0)


			{


				totalsenddev[port] = 0xFF;


			}


            else


            {


                totalsenddev[port] = dl_list_len(&g_uart_list[port]);


            }


            


		}





		if ((totalsenddev[UART_NUM_1] <= totalsenddev[UART_NUM_2]) && (totalsenddev[UART_NUM_1] <= totalsenddev[UART_NUM_0]) && (IS_SET_BLMAC[UART_NUM_1] == 1))


		{


			ret = UART_NUM_1;


		}


		else


		{


			if ((totalsenddev[UART_NUM_2] <= totalsenddev[UART_NUM_0]) && (IS_SET_BLMAC[UART_NUM_2] == 1))


			{


				ret = UART_NUM_2;


			}


			else


			{


				if (IS_SET_BLMAC[UART_NUM_0] == 1)


				{


					ret = UART_NUM_0;


				}


				// else if (IS_SET_BLMAC[UART_NUM_2] == 1)


				// {


				// 	ret = UART_NUM_2;


				// }


				// else if (IS_SET_BLMAC[UART_NUM_1] == 1)


				// {


				// 	ret = UART_NUM_1;


				// }


			}


		}





		for (int port = 0; port < UART_NUM_MAX; ++port)


		{


			if (IS_SET_BLMAC[port] == 0)


			{


				totalsenddev[port] = 0;


			}


		}


	}





	if (ret == -1)


	{





		printf("\n xx@@FATAL->[%s]...IS_SET_BLMAC[%d][%d][%d], totalsenddev[%d][%d][%d]\n", __func__,


			   IS_SET_BLMAC[0], IS_SET_BLMAC[1], IS_SET_BLMAC[2], totalsenddev[0], totalsenddev[1], totalsenddev[2]);


	}





	return ret;


}





void get_candidate_task(send_task_t *candidate_task, int i)


{


    //先解析出待分配任务


    // memset(candidate_task, 0, sizeof(send_task_t));


    // dl_list_init(&candidate_task->node); 


    candidate_task->cmd_id = i;


    candidate_task->img_id = i ;





}





int main()


{


    printf("double_list test-->\n");


    


    uart_manage_init();


    for(int port=0; port <3;port++)


    {


        printf("init port[%d] img_list length[%d]\n",port, dl_list_len(&g_uart_list[port]));


    }





    


    //add image to uart 


    for(int i=0; i<10; i++)


    {


        // send_task_t *new = dlist_cache_new();


        send_task_t *new = dlist_task_new();


        


        if(new == NULL)


        {


            printf("wfk\n");


            break;


        }


        


        get_candidate_task(new, i);


        printf("candidate_task:imgid:%p\n",&new->node);


        


        int candidate_uart_port = 0;//choose_candidate_port_by_count();


        printf("candidate_uar_port:%d\n",candidate_uart_port);


        dl_list_add(&g_uart_list[candidate_uart_port], &new->node);        





    }





    for(int port=0; port <3;port++)


    {


        printf("init port[%d] img_list length[%d]\n",port, dl_list_len(&g_uart_list[port]));
        totalsenddev[port] = dl_list_len(&g_uart_list[port]);


    }





    // for(int port=0; port<3; port++)


    // {


    //     send_task_t *item;


    //     dl_list_for_each(item, &g_uart_list[port], send_task_t, node)


    //     {


    //         printf("port[%d] img->[%d]\n", port,  item->cmd_id);


    //     }





    // }


        // delete item

    send_task_t* my_item;
    for(int port=0; port<3; port++)


    {


        int i =0;


        //    send_task_t *item ;


           send_task_t *next;


        // send_task_t *item = dl_list_first(&g_uart_list[port], send_task_t, node);


        //  printf("port[%d] img->[%d]\n", port,  item->cmd_id);


        //  dl_list_del(&item->node);

        

        dl_list_for_each_safe(my_item,next, &g_uart_list[port], send_task_t, node)


        {


            // printf("port[%d] img->[%d]\n", port,  item->cmd_id);


            printf("i:%d\n",i);


           // if(item->cmd_id ==2)


            {


                printf("port[%d] my_item->[%d]\n", port,  my_item->cmd_id);


               dl_list_del(&my_item->node);
               	totalsenddev[port]--;

                 printf("my item:%p [%d]", my_item, my_item==NULL);
               dlist_task_free(my_item);
               my_item = NULL;
               printf("my item:%p [%d]", my_item, my_item==NULL);


               //break;


            }


            i++;


        }


        





    }





        for(int port=0; port <3;port++)


    {


        printf("init port[%d] img_list length[%d] len:[%d]\n",port, dl_list_len(&g_uart_list[port]), totalsenddev[port]);
        


    }






    for(int port=0; port<3; port++)


    {


        send_task_t *item;


        dl_list_for_each(item, &g_uart_list[port], send_task_t, node)


        {


            printf("port[%d] img->[%d]\n", port,  item->cmd_id);


        }





    }








    return 0;


}




=======
static send_task_t  __cache_memory[80];
static struct dl_list s_cache_list;

void dlist_cache_init(void)
{
    dl_list_init(&s_cache_list);

    for (int i = 0; i < ARRAY_LEN(__cache_memory); i++) {
        dl_list_add(&s_cache_list, &__cache_memory[i].node);
    }

    printf("len:%d\n", dl_list_len(&s_cache_list));
}

send_task_t *dlist_cache_new(void)
{
    send_task_t *item = dl_list_first(&s_cache_list, send_task_t, node);
    if (item) {
        dl_list_del(&item->node);
    }
    
    return item;
}

send_task_t *dlist_task_new(void)
{
    send_task_t *item = (send_task_t*) malloc(sizeof(send_task_t));
    if (item) {
       printf("wfk\n");
    }
    dl_list_init(&item->node);
    
    return item;
}

void dlist_task_free(send_task_t *item)
{
    free(item);
}


void uart_manage_init()
{
    printf("%s\n",__func__);

    for(int port =0; port<UART_NUM_MAX; port++)
    {
        dl_list_init(&g_uart_list[port]);
        IS_SET_BLMAC[port] =1;
    }
    dlist_cache_init();

}

static int choose_candidate_port_by_count()
{
	int ret = -1;
    
	if (dl_list_len(&g_uart_list[0]) >= MAX_UART_LIST_LEN || dl_list_len(&g_uart_list[1]) >= MAX_UART_LIST_LEN || dl_list_len(&g_uart_list[2]) >= MAX_UART_LIST_LEN)
	{
        printf("init port[%d] img_list length[%d]\n",0, dl_list_len(&g_uart_list[0]));
        printf("init port[%d] img_list length[%d]\n",1, dl_list_len(&g_uart_list[1]));
        printf("init port[%d] img_list length[%d]\n",2, dl_list_len(&g_uart_list[2]));
	}
	else
	{
		for (int port = 0; port < UART_NUM_MAX; ++port)
		{
			if (IS_SET_BLMAC[port] == 0)
			{
				totalsenddev[port] = 0xFF;
			}
            else
            {
                totalsenddev[port] = dl_list_len(&g_uart_list[port]);
            }
            
		}

		if ((totalsenddev[UART_NUM_1] <= totalsenddev[UART_NUM_2]) && (totalsenddev[UART_NUM_1] <= totalsenddev[UART_NUM_0]) && (IS_SET_BLMAC[UART_NUM_1] == 1))
		{
			ret = UART_NUM_1;
		}
		else
		{
			if ((totalsenddev[UART_NUM_2] <= totalsenddev[UART_NUM_0]) && (IS_SET_BLMAC[UART_NUM_2] == 1))
			{
				ret = UART_NUM_2;
			}
			else
			{
				if (IS_SET_BLMAC[UART_NUM_0] == 1)
				{
					ret = UART_NUM_0;
				}
				// else if (IS_SET_BLMAC[UART_NUM_2] == 1)
				// {
				// 	ret = UART_NUM_2;
				// }
				// else if (IS_SET_BLMAC[UART_NUM_1] == 1)
				// {
				// 	ret = UART_NUM_1;
				// }
			}
		}

		for (int port = 0; port < UART_NUM_MAX; ++port)
		{
			if (IS_SET_BLMAC[port] == 0)
			{
				totalsenddev[port] = 0;
			}
		}
	}

	if (ret == -1)
	{

		printf("\n xx@@FATAL->[%s]...IS_SET_BLMAC[%d][%d][%d], totalsenddev[%d][%d][%d]\n", __func__,
			   IS_SET_BLMAC[0], IS_SET_BLMAC[1], IS_SET_BLMAC[2], totalsenddev[0], totalsenddev[1], totalsenddev[2]);
	}

	return ret;
}

void get_candidate_task(send_task_t *candidate_task, int i)
{
    //先解析出待分配任务
    // memset(candidate_task, 0, sizeof(send_task_t));
    // dl_list_init(&candidate_task->node); 
    candidate_task->cmd_id = i;
    candidate_task->img_id = i ;

}

int main()
{
    printf("double_list test-->\n");
    
    uart_manage_init();
    for(int port=0; port <3;port++)
    {
        printf("init port[%d] img_list length[%d]\n",port, dl_list_len(&g_uart_list[port]));
    }

    
    //add image to uart 
    for(int i=0; i<10; i++)
    {
        // send_task_t *new = dlist_cache_new();
        send_task_t *new = dlist_task_new();
        
        if(new == NULL)
        {
            printf("wfk\n");
            break;
        }
        
        get_candidate_task(new, i);
        printf("candidate_task:imgid:%p\n",&new->node);
        
        int candidate_uart_port = choose_candidate_port_by_count();
        printf("candidate_uar_port:%d\n",candidate_uart_port);
        dl_list_add_tail(&g_uart_list[candidate_uart_port], &new->node);        

    }

    for(int port=0; port <3;port++)
    {
        printf("init port[%d] img_list length[%d]\n",port, dl_list_len(&g_uart_list[port]));
    }

    // for(int port=0; port<3; port++)
    // {
    //     send_task_t *item;
    //     dl_list_for_each(item, &g_uart_list[port], send_task_t, node)
    //     {
    //         printf("port[%d] img->[%d]\n", port,  item->cmd_id);
    //     }

    // }
        // delete item
    for(int port=0; port<3; port++)
    {
        int i =0;
           send_task_t *item ;
           send_task_t *next;
        // send_task_t *item = dl_list_first(&g_uart_list[port], send_task_t, node);
        //  printf("port[%d] img->[%d]\n", port,  item->cmd_id);
        //  dl_list_del(&item->node);
        dl_list_for_each_safe(item,next, &g_uart_list[port], send_task_t, node)
        {
            // printf("port[%d] img->[%d]\n", port,  item->cmd_id);
            printf("i:%d\n",i);
            if(item->cmd_id ==2)
            {
                printf("port[%d] img->[%d]\n", port,  item->cmd_id);
               dl_list_del(&item->node);
               dlist_task_free(item);
               break;
            }
            i++;
        }
        

    }

        for(int port=0; port <3;port++)
    {
        printf("init port[%d] img_list length[%d]\n",port, dl_list_len(&g_uart_list[port]));
    }

    for(int port=0; port<3; port++)
    {
        send_task_t *item;
        dl_list_for_each(item, &g_uart_list[port], send_task_t, node)
        {
            printf("port[%d] img->[%d]\n", port,  item->cmd_id);
        }

    }


    return 0;
}

>>>>>>> temp
