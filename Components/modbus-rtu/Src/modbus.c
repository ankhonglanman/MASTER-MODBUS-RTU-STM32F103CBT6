#include "usart.h"
#include "mb_crc16.h"
#include "modbus.h"

#define     FUNCTION_01_FRAME_LENGTH        (8)
#define     FUNCTION_02_FRAME_LENGTH        (8)
#define     FUNCTION_03_FRAME_LENGTH        (8)
#define     FUNCTION_04_FRAME_LENGTH        (8)
#define     FUNCTION_05_FRAME_LENGTH        (8)
#define     FUNCTION_06_FRAME_LENGTH        (8)
#define     FUNCTION_15_FRAME_LENGTH        (8) /* variable length */ 
#define     FUNCTION_16_FRAME_LENGTH        (8) /* variable length */
#define     LOW_BYTE_OFFSET                 (8) /* Low byte offset to */
#define     SLAVE_ADDRESS_BIT_LENGTH        (8)
#define     FUNCTION_CODE_BIT_LENGTH        (8)
#define     START_ADDRESS_BIT_LENGTH        (16)
#define     NUMBER_OF_REGISTERS_BIT_LENGTH  (16)
#define     NUMBER_OF_COIL_BIT_LENGTH       (16)
#define     NUMBER_OF_INPUT_BIT_LENGTH      (16)
#define     CRC_BIT_LENGTH                  (16)    
#define     BYTE_COUNT_BIT_LENGTH           (8)

// Function 01 - Read Coils

// typedef struct mbFrame 
typedef struct mbFrame01{
    uint8_t  slave_address : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address : START_ADDRESS_BIT_LENGTH;
    uint16_t num_coils     : NUMBER_OF_COIL_BIT_LENGTH;
    uint16_t crc           : CRC_BIT_LENGTH;

    //mbFrame01() : function_code(0x01){} //Cpp syntax
}mbFrame01_t;

// Function 02 - Read Discrete Inputs
typedef struct mbFrame02{
    uint8_t  slave_address : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address : START_ADDRESS_BIT_LENGTH;
    uint16_t num_inputs    : NUMBER_OF_INPUT_BIT_LENGTH;
    uint16_t crc           : CRC_BIT_LENGTH;

    //mbFrame02() : function_code(0x02){} //cpp syntax
}mbFrame02_t;

// Function 03 - Read Holding Registers
typedef struct mbFrame03{

    uint8_t slave_address  : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t function_code  : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address : START_ADDRESS_BIT_LENGTH;
    uint16_t num_registers : NUMBER_OF_REGISTERS_BIT_LENGTH;
    uint16_t crc           : CRC_BIT_LENGTH;

    //mbFrame03() : function_code(0x03){} //cpp syntax
}mbFrame03_t;

// Function 04 - Read Input Registers
typedef struct mbFrame04{
    uint8_t  slave_address : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address : START_ADDRESS_BIT_LENGTH;
    uint16_t num_registers : NUMBER_OF_REGISTERS_BIT_LENGTH;
    uint16_t crc           : CRC_BIT_LENGTH;

    //mbFrame04() : function_code(0x04){} //cpp syntax
}mbFrame04_t;

// Function 05 - Write Single Coil
typedef struct mbFrame05{
    uint8_t  slave_address : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code : FUNCTION_CODE_BIT_LENGTH;
    uint16_t output_address: START_ADDRESS_BIT_LENGTH;
    uint16_t output_value  : NUMBER_OF_REGISTERS_BIT_LENGTH;  // 0xFF00 = ON, 0x0000 = OFF
    uint16_t crc           : CRC_BIT_LENGTH;

    //mbFrame05() : function_code(0x05){} //cpp syntax
}mbFrame05_t;

// Function 06 - Write Single Register
typedef struct mbFrame06{
    uint8_t  slave_address    : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code    : FUNCTION_CODE_BIT_LENGTH;
    uint16_t register_address : START_ADDRESS_BIT_LENGTH;
    uint16_t register_value   : NUMBER_OF_REGISTERS_BIT_LENGTH;
    uint16_t crc              : CRC_BIT_LENGTH;

    //mbFrame06() : function_code(0x06){} //cpp syntax 
}mbFrame06_t;

// Function 0F (15) - Write Multiple Coils
typedef struct mbFrame15{
    uint8_t  slave_address  : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code  : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address  : START_ADDRESS_BIT_LENGTH;
    uint16_t num_coils      : NUMBER_OF_COIL_BIT_LENGTH;
    uint8_t  byte_count     : BYTE_COUNT_BIT_LENGTH;
    uint8_t  coil_values[];
    
    //mbFrame0F() : function_code(0x0F){} //cpp syntax
}mbFrame15_t;

// Function 10 (16) - Write Multiple Registers
typedef struct mbFrame16{
    uint8_t  slave_address  : SLAVE_ADDRESS_BIT_LENGTH;
    uint8_t  function_code  : FUNCTION_CODE_BIT_LENGTH;
    uint16_t start_address  : START_ADDRESS_BIT_LENGTH;
    uint16_t num_registers  : NUMBER_OF_REGISTERS_BIT_LENGTH;
    uint8_t  byte_count     : BYTE_COUNT_BIT_LENGTH;
    uint16_t register_values[];
    
    //mbFrame10() : function_code(0x10){} //cpp syntax
}mbFrame16_t;

void master_func_03(uint8_t slave_address, uint8_t function_code, uint16_t start_address, uint16_t num_registers, volatile uint8_t *pFrame, uint8_t pLen){
    uint8_t frame[FUNCTION_03_FRAME_LENGTH];
    frame[0] = slave_address;
    frame[1] = function_code;
    frame[2] = (start_address >> 8) & 0xFF; 
    frame[3] = start_address & 0xFF;        
    frame[4] = (num_registers >> 8) & 0xFF; 
    frame[5] = num_registers & 0xFF;        

    uint16_t crc = mb_crc16(frame, 6);
    frame[6] = (crc >> LOW_BYTE_OFFSET) & 0xFF;                  
    frame[7] = crc & 0xFF;

    /*
        In MODBUS-RTU, the CRC is Low to High, but others byte is High to Low
    */
    
    /*
    frame[6] = 0xc4;
    frame[7] = 0x0b;
    */
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), 12);

    HAL_UART_Receive_DMA(&huart1, pFrame, pLen);
}