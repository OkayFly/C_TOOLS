/**
 ****************************************************************************************
 *
 * @file user_buffer.h
 *
 * @brief SPS project source code.
 *
 * Copyright (C) 2016 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information  
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef USER_BUFFER_H_
#define USER_BUFFER_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief SPS Application entry point.
 *
 * @{
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 #include <stdint.h>
 #include <stdbool.h>
 /*
 * DEFINES
 ****************************************************************************************
 */

/*
 * STRUCTURES
 ****************************************************************************************
 */
typedef struct {
    int     buffer_size;
    int     readIdx;        //points to first full
    int     writeIdx;       //points to first empty
    int     endIdx;         //points to element (full or empty) with maximal address
    uint8_t *data_ptr;
    int     low_watermark;
    int     high_watermark;
    bool    hwm_reached;
    unsigned int nof_buffer_write_bytes;
    unsigned int nof_buffer_write_miss_bytes;
    unsigned int nof_read_callback_bytes;
    unsigned int nof_release_bytes;
}RingBuffer;

 /*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief       function used to declare a new buffer
 ****************************************************************************************
 */
void user_buffer_create(RingBuffer* buffer, int buffer_size, int low_watermark, int high_watermark);

/**
 ****************************************************************************************
 * @brief       function used to check if buffer is initialized
 ****************************************************************************************
 */
uint8_t user_buffer_initialized(RingBuffer *buffer);

/**
 ****************************************************************************************
 * @brief function used to count the amount of items in buffer
 ****************************************************************************************
 */
int user_buffer_item_count(RingBuffer* buffer);

/**
****************************************************************************************
* @brief        function used to push multiple items to buffer
****************************************************************************************
*/
void user_buffer_write_items(RingBuffer* buffer, uint8_t* wrdata, uint16_t write_amount);

/**
 ****************************************************************************************
 * @brief Function used to calculate available items and return address of first item
 ****************************************************************************************
 */
uint16_t user_buffer_read_address(RingBuffer* buffer, uint8_t** rddata, uint16_t readAmount);

/**
 ****************************************************************************************
 * @brief Function used release items that were previously pulled with 
 ****************************************************************************************
 */
void user_buffer_release_items(RingBuffer* buffer, uint16_t readAmount);

/**
 ****************************************************************************************
 * @brief           Check if required space is available and return the starting address
 ****************************************************************************************
 */
uint16_t user_buffer_write_check (RingBuffer* buffer, uint8_t** wrdata, uint16_t max_write_amount);


//查看里面的数据
uint16_t user_buffer_peek(RingBuffer* buffer, uint8_t *rddata, uint16_t read_amount);

//释放所有的数据
void user_buffer_release_all_items(RingBuffer* buffer);

/**
 ****************************************************************************************
 * @brief       Confirm bytes written in the buffer. Always used with the user_buffer_write_check
 ****************************************************************************************
 */
void user_buffer_cfm_write (RingBuffer* buffer, uint16_t actual_write_amount);

/**
 ****************************************************************************************
 * @brief       This function checks if the buffer is almost empty.
 ****************************************************************************************
 */
bool user_check_buffer_almost_empty(RingBuffer* buffer);

/**
 ****************************************************************************************
 * @brief       This function checks if the buffer is almost full.
 ****************************************************************************************
 */
bool user_check_buffer_almost_full(RingBuffer* buffer);

/// @} APP

#endif //USER_BUFFER_H_