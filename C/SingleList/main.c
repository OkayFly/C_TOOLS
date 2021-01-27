#include <stdio.h>

#include "list.h"
#include "single_list.h"

typedef struct My_Data
{
    int num;
    int data;
} my_data_t;



int main();
int main_list()
{
    printf("test single list-->\n");

    list_node *my_list;
    my_data_t my_data_1 = {0,0};
    my_list = list_create(&my_data_1);

   
        //
    struct list_node *p;
    p = (struct list_node*) malloc(sizeof(struct list_node));
         int i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
     my_data_t my_data_2 = {1,1};
     list_insert_after(my_list, &my_data_2);
          i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }

printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    my_data_t my_data_3 = {2,2};

   // list_node *new_list 
   printf("l11ist:%p\n", my_list);
    // my_list=  
    list_insert_beginning(&my_list, &my_data_3);
    printf("22list:%p\n", my_list);
       //list_insert_after(my_list, &my_data_3);
            i =0;
    for(p = my_list; p != NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }
printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    my_data_t my_data_4 = {3,3};
    list_insert_end(my_list, &my_data_4);


     i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }



    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
list_insert_end(my_list, &my_data_4);
     i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }



    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

list_remove_by_data(&my_list, &my_data_4);
     i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }



    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    list_remove_by_data(&my_list, &my_data_4);
     i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }



    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


    list_remove_by_data(&my_list, &my_data_4);
     i =0;
    for(p = my_list; p!= NULL; p = p->next)
    {
        printf("[%d, %d, %d]\n",i++, ((my_data_t *) p->data)->num, ((my_data_t *) p->data)->data);

    }



    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");




    

}
