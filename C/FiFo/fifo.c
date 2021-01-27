/**
*********************************************************************************
 *(C) @@.        [ftd] All Rights Reserved.
 *FileName:      fifo.c
 *Author:        fly
 *Telephtone:    
 *Version:       1.0.0
 *Data:          2021.01.04
 *Description:   
 *Others:        
 *History:       
**********************************************************************************
*/

#include "fifo.h"
#define MIN_BUFFER_SIZE (0x04)
//static const char * TAG="fifo";
static inline U32 roundup_power_of_2(U32 n){
	 if(n==0)
		 return 0;
	  U32 position = 0;
	  U32 i=0;
	  for (i = n; i != 0; i >>= 1){
		   position++;
	 }
	 return (1<<position);
}

static inline bool is_power_of_2(U32 n){
     return ((n&(n-1))==0)? true:false;
}

static inline unsigned int init_fifo_size(U32 size){
     if(size<MIN_BUFFER_SIZE)size=MIN_BUFFER_SIZE;
     if(!is_power_of_2(size)){
	 	size=roundup_power_of_2(size);
	 }
     return size;
}

FIFO fifo_init(U32 size){
	//ESP_LOGI(TAG,"you want init fifo size=%d",size);
	size=init_fifo_size(size);
	//ESP_LOGI(TAG,"real init fifo size=%d",size);
	fifo_t *priv=(fifo_t *)malloc(sizeof(fifo_t));
	if(!priv){
		return NULL;
	}
	priv->in=0;
	priv->out=0;
	priv->size=size;
	priv->mask=size-1;
	priv->buffer=(char *)malloc(size);
	if(!priv->buffer){
		free(priv);
		priv=NULL;
		return priv;
	}
	//ESP_LOGI(TAG,"fifo malloc ok=%d",size);
	memset(priv->buffer,0,size);
	return priv;
}


bool fifo_uninit(FIFO fifo){
	if(fifo){
		if(fifo->buffer){
			free(fifo->buffer);
			fifo->buffer=NULL;
		}
		free(fifo);
		fifo=NULL;
	}
	return true;
}

U32 fifo_size(FIFO fifo){
	if(!fifo)
		return 0;
	return fifo->mask+1;//return fifo->size;
}

U32 fifo_used(FIFO fifo){
	return (fifo->in-fifo->out);
}

U32 fifo_unused(FIFO fifo){
	return (fifo->size-fifo->in+fifo->out);
}

U32 fifo_obtain_in_offset(FIFO fifo){
	return (fifo->in & (fifo->mask));
}

char* fifo_obtain_in_adress(FIFO fifo){
	return fifo->buffer +(fifo->in & (fifo->mask));
}

U32 fifo_in(FIFO fifo,char *buffer,U32 len){

	assert(fifo||buffer);
	U32 l;
	len=MIN(len,fifo->size-fifo->in+fifo->out);
	l = MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
	memcpy(fifo->buffer, buffer + l, len - l);
	fifo->in += len;

	return len;
}

U32 fifo_obtain_out_offset(FIFO fifo){
	return (fifo->out& (fifo->mask));
}

char* fifo_obtain_out_address(FIFO fifo){
	return fifo->buffer+(fifo->out& (fifo->mask));
}

U32 fifo_out(FIFO fifo,char *buffer,U32 len){
	assert(fifo||buffer);
	U32 l;
	len = MIN(len, fifo->in - fifo->out);
	l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
	memcpy(buffer + l, fifo->buffer, len - l);
	fifo->out += len;
	return len;
}


U32 fifo_prefetch(FIFO fifo,char *buffer,U32 len){
	assert(fifo||buffer);
	U32 l;
	len = MIN(len, fifo->in - fifo->out);
	l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
	memcpy(buffer + l, fifo->buffer, len - l);
	//fifo->out += len;
	return len;
}

boolean fifo_move_out_to_index(FIFO fifo,U32 len){
	assert(fifo);
	if((fifo->in - fifo->out)>=len){
		fifo->out+=len;
		return true;
	}
	return false;	
}


void fifo_reset(FIFO fifo){
	//assert(x)(fifo);
	//__asm__("MEMW");
	memset(fifo->buffer,'\0',fifo_used(fifo));
	
	fifo->in=0;
	fifo->out=0;
	//__asm__("MEMW");
		

}


