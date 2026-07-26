#include "master_func_03.h"

void master_func_03(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_registers, volatile uint8_t *pFrame, uint8_t pLen){
    uint8_t frame[8];
    frame[0] = slave_address;
    frame[1] = function_code;
    frame[2] = (start_address >> 8) & 0xFF; 
    frame[3] = start_address & 0xFF;        
    frame[4] = (num_registers >> 8) & 0xFF; 
    frame[5] = num_registers & 0xFF;        

    /* uint16_t crc = mb_crc16(frame, 6);
    frame[6] = crc & 0xFF;                  
    frame[7] = (crc >> 8) & 0xFF;
    */
    
    frame[6] = 0xc4;
    frame[7] = 0x0b;
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), 12);

    HAL_UART_Receive_DMA(&huart1, pFrame, pLen);
}