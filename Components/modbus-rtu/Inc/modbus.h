#ifndef __MODBUS_H
#define __MODBUS_H 







void master_func_03(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_registers, volatile uint8_t *pFrame, uint8_t pLen);

void master_func_04(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_registers, volatile uint8_t *pFrame, uint8_t pLen);