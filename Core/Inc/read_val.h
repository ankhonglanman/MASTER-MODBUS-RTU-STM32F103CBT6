#include "usart.h"

typedef struct {
    float temp;
    float humid; 
} Sensor_Data_t;

Sensor_Data_t read_val(volatile uint8_t *pFrame, uint8_t pLen);
