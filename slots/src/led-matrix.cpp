
#include "led-matrix.h"

// ------------------------------------------------------------ Global variables

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::FC16_HW, MOSI, SCK, SS, 4);

#define MAX_LEDMATRIX_CHAR_LENGTH 8  // this should be ok for all built-in fonts

// ----------------------------------------------------- Public member functions

void LedMatrix::start()
{
	if(!mx.begin()) {
		// Error
	}
}

/**
 * Scroll text once.
 */
void LedMatrix::scrollText(const char *str, bool clear = false)
{
	uint8_t charWidth;
	uint8_t charBuffer[MAX_LEDMATRIX_CHAR_LENGTH];
	uint8_t pos = 0;

	if(clear) {
		mx.clear();
	}

	while(str[pos] != '\0') {
		charWidth = mx.getChar(str[pos++], sizeof(charBuffer) / sizeof(charBuffer[0]), charBuffer);

		for(uint8_t i = 0; i < charWidth + charSpacing; i++)
		{
			mx.transform(MD_MAX72XX::TSL);
			mx.setColumn(0, i < charWidth ? charBuffer[i] : 0);
			// TODO: use a delay-less loop
			delay(10);
		}
	}
}

void LedMatrix::printText(const char *str, uint8_t offset = 0, bool clear = false)
{
	uint8_t charWidth;
	const uint8_t colZero = mx.getColumnCount() - 1;
	uint8_t pos = colZero - offset;

	if(clear) {
		mx.clear();
	}

	// TODO: actually strlen() should not be necessary
	uint8_t len = strlen(str);
	for(uint8_t i = 0; str[i] != '\0', pos, i < len; i++) {
		charWidth = mx.setChar(pos, str[i]);
		pos -= (charWidth + charSpacing);
		mx.setColumn(pos + 1, 0x00);
	}
}

void LedMatrix::printUnsignedInteger(uint16_t value, uint8_t bufferSize, uint8_t offset = 0,
	bool clear = false, bool leftPadWithZeros = false)
{
	char buffer[bufferSize + 1];
	buffer[bufferSize] = '\0';

	if(leftPadWithZeros) {
		for(int8_t i = bufferSize - 1; i >= 0; i--) {
			buffer[i] = '0' + value % 10;
			value /= 10;
		}
	} else {
		memset(buffer, ' ', bufferSize - 1);
		for(int8_t i = bufferSize - 1; i >= 0; i--) {
			buffer[i] =  (value == 0 && i < bufferSize - 1) ? ' ' : '0' + value % 10;
			value /= 10;
		}
	}

	printText(buffer, offset, clear);
}

// ------------------------------------------------------------------------- End
