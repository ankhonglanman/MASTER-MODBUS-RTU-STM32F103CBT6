#include "ssd1306_temp_and_humidity_display.h"

void ssd1306_temp_and_humidity_display(float temperature, float humidity)
{
    char buffer[32];

    ssd1306_Fill(Black);

    temperature -= 0.1; 
    int temp_int = (int)temperature;
    int temp_dec = (int)((temperature - temp_int) * 10);
    if (temp_dec < 0) temp_dec = -temp_dec;  // tránh số âm bị lệch dấu
    snprintf(buffer, sizeof(buffer), "Temp: %d.%d C", temp_int, temp_dec);
    ssd1306_SetCursor(30, 10);
    ssd1306_WriteString(buffer, Font_7x10, White);

    int humidity_int = (int)humidity;
    int humidity_dec = (int)((humidity - humidity_int) * 10);
    if (humidity_dec < 0) humidity_dec = -humidity_dec;
    snprintf(buffer, sizeof(buffer), "Humi: %d.%d %%", humidity_int, humidity_dec);
    ssd1306_SetCursor(30, 25);
    ssd1306_WriteString(buffer, Font_7x10, White);

    ssd1306_UpdateScreen();
}