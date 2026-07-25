#include "usart.h"
#include "mb_crc16.h"

void master_func_03(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_registers);