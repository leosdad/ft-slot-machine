
#include "led-matrix.h"

// ------------------------------------------------------------ Global variables

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::DR1CR0RR0_HW, MOSI, SCK, SS, 4);

#define MAX_LEDMATRIX_CHAR_LENGTH 10
#define MAX_STRING 256

// ----------------------------------------------------- Public member functions

void LedMatrix::setup()
{
	if(!mx.begin()) {
		// Error
	}
	// Module #0 is the rightmost one.
	columnZero = mx.getColumnCount() - 1;
}

void LedMatrix::clearColumns(uint8_t start, uint8_t end)
{
	for(uint8_t i = columnZero - end; i <= columnZero - start; i++) {
		mx.setColumn(i, 0x00);
	}
}

/**
 * Scroll text once.
 */
void LedMatrix::scrollText(const char *str, uint8_t offset = 0)
{
	uint8_t charWidth;
	uint8_t charBuffer[MAX_LEDMATRIX_CHAR_LENGTH];
	uint8_t pos = 0;

	while(str[pos] != '\0') {
		charWidth = mx.getChar(str[pos++], sizeof(charBuffer) / sizeof(charBuffer[0]), charBuffer);

		for(uint8_t i = 0; i < charWidth + charSpacing; i++) {
			mx.transform(0, offset < 8 ? 3 : 2, MD_MAX72XX::TSL);
			mx.setColumn(0, i < charWidth ? charBuffer[i] : 0);
		}
	}
}

/**
 * Wraps text on the whole display.
 * @param text The input text to be wrapped.
 * @param loopFn The loop function (may be `NULL`). Exits the loop if true.
 */
void LedMatrix::wrapText(const char *text, bool loopFn())
{
	uint8_t charWidth;
	uint8_t charBuffer[MAX_LEDMATRIX_CHAR_LENGTH];
	uint16_t length = strlen(text);
	uint16_t pos = 0;

	// The buffer must be at least twice the size of the longest string
	char buffer[MAX_STRING * 2 + 1];
	// Create a copy
	memcpy(buffer, text, length);
	// Create a second copy
	memcpy(buffer + length + 1, text, length);

	while(true) {
		charWidth = mx.getChar(buffer[pos++], sizeof(charBuffer) / sizeof(charBuffer[0]), charBuffer);
		for(uint8_t i = 0; i <= charWidth; i++) {
			mx.transform(MD_MAX72XX::TSL);
			mx.setColumn(0, i < charWidth ? charBuffer[i] : 0);
			if(loopFn) {
				if(loopFn()) {
					// HACK
					// clearColumns(0, 8 * 4 - 1);
					goto EXIT;
				}
			}
		}
		if(pos == length) {
			pos = 0;
		}
	}

	EXIT:

	return;
}

void LedMatrix::printText(const char *str, uint8_t offset = 0)
{
	uint8_t charWidth;
	uint8_t pos = columnZero - offset;
	uint8_t len = strlen(str);

	for(uint8_t i = 0; str[i] != '\0', pos, i < len; i++) {
		charWidth = mx.setChar(pos, str[i]);
		pos -= (charWidth + charSpacing);
		mx.setColumn(pos + 1, 0x00);
	}

	mx.setColumn(pos + 1, 0x00);
}

void LedMatrix::printChar(const char ch, uint8_t offset = 0)
{
	mx.setChar(columnZero - offset, ch);
}

void LedMatrix::printUnsignedInteger(
	uint16_t value,
	uint8_t bufferSize,
	uint8_t offset = 0,
	NumberPadding padding = NumberPadding::NONE,
	uint8_t zero = '0'
)
{
	char buffer[bufferSize + 1];
	buffer[bufferSize] = '\0';

	switch(padding) {

		case NumberPadding::ZEROS:

			for(int8_t i = bufferSize - 1; i >= 0; i--) {
				buffer[i] = zero + value % 10;
				value /= 10;
			}
			break;

		case NumberPadding::NONE:

			memset(buffer, ' ', bufferSize - 1);
			for(int8_t i = bufferSize - 1; i >= 0; i--) {
				buffer[i] =  (value == 0 && i < bufferSize - 1) ? ' ' : zero + value % 10;
				value /= 10;
			}
			break;

		case NumberPadding::SPACES:

			memset(buffer, '/', bufferSize - 1);	// '/' is a number-sized space
			for(int8_t i = bufferSize - 1; i >= 0; i--) {
				buffer[i] =  (value == 0 && i < bufferSize - 1) ? '/' : zero + value % 10;
				value /= 10;
			}

			break;
	}

	printText(buffer, offset);
}

// ------------------------------------------------------------------------- End
