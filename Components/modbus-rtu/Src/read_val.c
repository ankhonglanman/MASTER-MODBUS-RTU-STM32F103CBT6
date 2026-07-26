#include "read_val.h" 

Sensor_Data_t read_val(volatile uint8_t *pFrame, uint8_t pLen){

    Sensor_Data_t data; 

    uint16_t temp_raw  = (uint16_t)(pFrame[3] << 8) | pFrame[4];
    int16_t humid_raw = (uint16_t)(pFrame[5] << 8) | pFrame[6];

    data.temp  = (float)temp_raw  / 10.0f;
    data.humid = (float)humid_raw / 10.0f;

    return data;
}



