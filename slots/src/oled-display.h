
#include <Arduino.h>

void oledSetup();
void oledPrintS(uint8_t row, uint8_t col, const char *str);
void oledPrintN(uint8_t row, uint8_t col, uint16_t number);
