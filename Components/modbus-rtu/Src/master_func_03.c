#include "master_func_03.h"

void master_func_03(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_register){
    uint8_t frame[8];
    request[0] = slave_address;
    request[1] = function_code;
    request[2] = (start_address >> 8) & 0xFF; 
    request[3] = start_address & 0xFF;        
    request[4] = (num_registers >> 8) & 0xFF; 
    request[5] = num_registers & 0xFF;        

    uint16_t crc = mb_crc16(request, 6);
    request[6] = crc & 0xFF;                  
    request[7] = (crc >> 8) & 0xFF;           

    HAL_UART_Transmit(&huart1, frame, sizeof(frame), 12);
}