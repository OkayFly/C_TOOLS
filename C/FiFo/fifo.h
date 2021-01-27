/**
*********************************************************************************
 *(C) @@.        [ftd] All Rights Reserved.
 *FileName:      fifo.h
 *Author:        fly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2021.01.26
 *Description:   
 *Others:        
 *History:       
**********************************************************************************
*/

#ifndef __FIFO_H__
#define __FIFO_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "utility.h"

#define FIFO_CHECK(p,ret) \
    if (p==NULL) { \
        ret=false; \
    }	\
    ret=true;

typedef struct _fifo_data_t{
	char *buffer;
	U32 size;
	U32 mask;
	U32 in;
	U32 out;
}fifo_t;

typedef fifo_t *FIFO;
FIFO fifo_init(U32 size);
//bool fifo_uninit(FIFO fifo);
U32 fifo_size(FIFO fifo);
U32 fifo_used(FIFO fifo);
U32 fifo_unused(FIFO fifo);
U32 fifo_obtain_in_offset(FIFO fifo);
char* fifo_obtain_in_address(FIFO fifo);
U32 fifo_in(FIFO fifo,char *buffer,U32 len);
U32 fifo_obtain_out_offset(FIFO fifo);
char* fifo_obtain_out_address(FIFO fifo);
U32 fifo_out(FIFO fifo,char *buffer,U32 len);
U32 fifo_prefetch(FIFO fifo,char *buffer,U32 len);
boolean fifo_move_out_to_index(FIFO fifo,U32 len);
void fifo_reset(FIFO fifo);
#ifdef __cplusplus
}
#endif
#endif
