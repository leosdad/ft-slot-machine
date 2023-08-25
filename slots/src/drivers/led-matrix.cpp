
#include "led-matrix.h"

// ------------------------------------------------------------ Global variables

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::DR1CR0RR0_HW, MOSI, SCK, SS, 4);

#define MAX_LEDMATRIX_CHAR_LENGTH 10

// ----------------------------------------------------- Public member functions

void LedMatrix::start()
{
	if(!mx.begin()) {
		// Error
	}
	// Module #0 the rightmost one.
	columnZero = mx.getColumnCount() - 1;
}

// TODO: Must use a delay-less loop
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
			mx.transform(0, 2, MD_MAX72XX::TSL);
			mx.setColumn(0, i < charWidth ? charBuffer[i] : 0);
		}
	}
}

void LedMatrix::printText(const char *str, uint8_t offset = 0)
{
	uint8_t charWidth;
	uint8_t pos = columnZero - offset;

	// TODO: actually strlen() should not be necessary
	uint8_t len = strlen(str);

	// Serial.print("String: ");
	// Serial.println(str);
	// Serial.print("Strlen: ");
	// Serial.println(len);
	// Serial.print("Pos before: ");
	// Serial.println(pos);

	for(uint8_t i = 0; str[i] != '\0', pos, i < len; i++) {
		charWidth = mx.setChar(pos, str[i]);
		pos -= (charWidth + charSpacing);
		mx.setColumn(pos + 1, 0x00);
	}

	// Serial.print("Pos before: ");
	// Serial.println(pos);
	// Serial.println();

	mx.setColumn(pos + 1, 0x00);
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
