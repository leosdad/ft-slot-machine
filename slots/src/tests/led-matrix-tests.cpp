// Program to exercise the MD_MAX72XX library
//
// Uses most of the functions in the library
#include <MD_MAX72xx.h>

// Turn on debug statements to the serial output
#define DEBUG false

#if DEBUG
#define PRINT(s, x)         \
	{                       \
		Serial.print(F(s)); \
		Serial.print(x);    \
	}
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)

#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)

#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES	  4

#define CLK_PIN		  SCK
#define DATA_PIN	  MOSI
#define CS_PIN		  SS

// SPI hardware interface
MD_MAX72XX mxt = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Specific SPI hardware interface
// MD_MAX72XX mxt = MD_MAX72XX(HARDWARE_TYPE, SPI1, CS_PIN, MAX_DEVICES);
// Arbitrary pins
// MD_MAX72XX mxt = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// We always wait a bit between updates of the display
#define DELAYTIME 40  // in milliseconds

void scrollText(const char *p)
{
	uint8_t charWidth;
	uint8_t cBuf[8];  // this should be ok for all built-in fonts

	PRINTS("\nScrolling text");
	mxt.clear();

	while(*p != '\0') {
		charWidth = mxt.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

		for(uint8_t i = 0; i <= charWidth; i++)	 // allow space between characters
		{
			mxt.transform(MD_MAX72XX::TSL);
			mxt.setColumn(0, (i < charWidth) ? cBuf[i] : 0);
			delay(DELAYTIME);
		}
	}
}

void zeroPointSet()
// Demonstrates the use of setPoint and
// show where the zero point is in the display
{
	PRINTS("\nZero point highlight");
	mxt.clear();

	if(MAX_DEVICES > 1)
		mxt.setChar((2 * COL_SIZE) - 1, '0');

	for(uint8_t i = 0; i < ROW_SIZE; i++) {
		mxt.setPoint(i, i, true);
		mxt.setPoint(0, i, true);
		mxt.setPoint(i, 0, true);
		delay(DELAYTIME);
	}

	delay(DELAYTIME * 3);
}

void rows()
// Demonstrates the use of setRow()
{
	PRINTS("\nRows 0->7");
	mxt.clear();

	for(uint8_t row = 0; row < ROW_SIZE; row++) {
		mxt.setRow(row, 0xff);
		delay(2 * DELAYTIME);
		mxt.setRow(row, 0x00);
	}
}

void checkboard()
// nested rectangles spanning the entire display
{
	uint8_t chkCols[][2] = {{0x55, 0xaa}, {0x33, 0xcc}, {0x0f, 0xf0}, {0xff, 0x00}};

	PRINTS("\nCheckboard");
	mxt.clear();

	for(uint8_t pattern = 0; pattern < sizeof(chkCols) / sizeof(chkCols[0]); pattern++) {
		uint8_t col = 0;
		uint8_t idx = 0;
		uint8_t rep = 1 << pattern;

		while(col < mxt.getColumnCount()) {
			for(uint8_t r = 0; r < rep; r++)
				mxt.setColumn(col++, chkCols[pattern][idx]);	 // use odd/even column masks
			idx++;
			if(idx > 1) idx = 0;
		}

		delay(10 * DELAYTIME);
	}
}

void columns()
// Demonstrates the use of setColumn()
{
	PRINTS("\nCols 0->max");
	mxt.clear();

	for(uint8_t col = 0; col < mxt.getColumnCount(); col++) {
		mxt.setColumn(col, 0xff);
		delay(DELAYTIME / MAX_DEVICES);
		mxt.setColumn(col, 0x00);
	}
}

void cross()
// Combination of setRow() and setColumn() with user controlled
// display updates to ensure concurrent changes.
{
	PRINTS("\nMoving cross");
	mxt.clear();
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

	// diagonally down the display R to L
	for(uint8_t i = 0; i < ROW_SIZE; i++) {
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0xff);
			mxt.setRow(j, i, 0xff);
		}
		mxt.update();
		delay(DELAYTIME);
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0x00);
			mxt.setRow(j, i, 0x00);
		}
	}

	// moving up the display on the R
	for(int8_t i = ROW_SIZE - 1; i >= 0; i--) {
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0xff);
			mxt.setRow(j, ROW_SIZE - 1, 0xff);
		}
		mxt.update();
		delay(DELAYTIME);
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0x00);
			mxt.setRow(j, ROW_SIZE - 1, 0x00);
		}
	}

	// diagonally up the display L to R
	for(uint8_t i = 0; i < ROW_SIZE; i++) {
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0xff);
			mxt.setRow(j, ROW_SIZE - 1 - i, 0xff);
		}
		mxt.update();
		delay(DELAYTIME);
		for(uint8_t j = 0; j < MAX_DEVICES; j++) {
			mxt.setColumn(j, i, 0x00);
			mxt.setRow(j, ROW_SIZE - 1 - i, 0x00);
		}
	}
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void bullseye()
// Demonstrate the use of buffer based repeated patterns
// across all devices.
{
	PRINTS("\nBullseye");
	mxt.clear();
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

	for(uint8_t n = 0; n < 3; n++) {
		byte b = 0xff;
		int i = 0;

		while(b != 0x00) {
			for(uint8_t j = 0; j < MAX_DEVICES + 1; j++) {
				mxt.setRow(j, i, b);
				mxt.setColumn(j, i, b);
				mxt.setRow(j, ROW_SIZE - 1 - i, b);
				mxt.setColumn(j, COL_SIZE - 1 - i, b);
			}
			mxt.update();
			delay(3 * DELAYTIME);
			for(uint8_t j = 0; j < MAX_DEVICES + 1; j++) {
				mxt.setRow(j, i, 0);
				mxt.setColumn(j, i, 0);
				mxt.setRow(j, ROW_SIZE - 1 - i, 0);
				mxt.setColumn(j, COL_SIZE - 1 - i, 0);
			}

			bitClear(b, i);
			bitClear(b, 7 - i);
			i++;
		}

		while(b != 0xff) {
			for(uint8_t j = 0; j < MAX_DEVICES + 1; j++) {
				mxt.setRow(j, i, b);
				mxt.setColumn(j, i, b);
				mxt.setRow(j, ROW_SIZE - 1 - i, b);
				mxt.setColumn(j, COL_SIZE - 1 - i, b);
			}
			mxt.update();
			delay(3 * DELAYTIME);
			for(uint8_t j = 0; j < MAX_DEVICES + 1; j++) {
				mxt.setRow(j, i, 0);
				mxt.setColumn(j, i, 0);
				mxt.setRow(j, ROW_SIZE - 1 - i, 0);
				mxt.setColumn(j, COL_SIZE - 1 - i, 0);
			}

			i--;
			bitSet(b, i);
			bitSet(b, 7 - i);
		}
	}

	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void stripe()
// Demonstrates animation of a diagonal stripe moving across the display
// with points plotted outside the display region ignored.
{
	const uint16_t maxCol = MAX_DEVICES * ROW_SIZE;
	const uint8_t stripeWidth = 10;

	PRINTS("\nEach individually by row then col");
	mxt.clear();

	for(uint16_t col = 0; col < maxCol + ROW_SIZE + stripeWidth; col++) {
		for(uint8_t row = 0; row < ROW_SIZE; row++) {
			mxt.setPoint(row, col - row, true);
			mxt.setPoint(row, col - row - stripeWidth, false);
		}
		delay(DELAYTIME);
	}
}

void spiral()
// setPoint() used to draw a spiral across the whole display
{
	PRINTS("\nSpiral in");
	int rmin = 0, rmax = ROW_SIZE - 1;
	int cmin = 0, cmax = (COL_SIZE * MAX_DEVICES) - 1;

	mxt.clear();
	while((rmax > rmin) && (cmax > cmin)) {
		// do row
		for(int i = cmin; i <= cmax; i++) {
			mxt.setPoint(rmin, i, true);
			delay(DELAYTIME / MAX_DEVICES);
		}
		rmin++;

		// do column
		for(uint8_t i = rmin; i <= rmax; i++) {
			mxt.setPoint(i, cmax, true);
			delay(DELAYTIME / MAX_DEVICES);
		}
		cmax--;

		// do row
		for(int i = cmax; i >= cmin; i--) {
			mxt.setPoint(rmax, i, true);
			delay(DELAYTIME / MAX_DEVICES);
		}
		rmax--;

		// do column
		for(uint8_t i = rmax; i >= rmin; i--) {
			mxt.setPoint(i, cmin, true);
			delay(DELAYTIME / MAX_DEVICES);
		}
		cmin++;
	}
}

void bounce()
// Animation of a bouncing ball
{
	const int minC = 0;
	const int maxC = mxt.getColumnCount() - 1;
	const int minR = 0;
	const int maxR = ROW_SIZE - 1;

	int nCounter = 0;

	int r = 0, c = 2;
	int8_t dR = 1, dC = 1;	// delta row and column

	PRINTS("\nBouncing ball");
	mxt.clear();

	while(nCounter++ < 200) {
		mxt.setPoint(r, c, false);
		r += dR;
		c += dC;
		mxt.setPoint(r, c, true);
		delay(DELAYTIME / 2);

		if((r == minR) || (r == maxR))
			dR = -dR;
		if((c == minC) || (c == maxC))
			dC = -dC;
	}
}

void intensity()
// Demonstrates the control of display intensity (brightness) across
// the full range.
{
	uint8_t row;

	PRINTS("\nVary intensity ");

	mxt.clear();

	// Grow and get brighter
	row = 0;
	for(int8_t i = 0; i <= MAX_INTENSITY; i++) {
		mxt.control(MD_MAX72XX::INTENSITY, i);
		if(i % 2 == 0)
			mxt.setRow(row++, 0xff);
		delay(DELAYTIME * 3);
	}

	mxt.control(MD_MAX72XX::INTENSITY, 8);
}

void blinking()
// Uses the test function of the MAX72xx to blink the display on and off.
{
	int nDelay = 1000;

	PRINTS("\nBlinking");
	mxt.clear();

	while(nDelay > 0) {
		mxt.control(MD_MAX72XX::TEST, MD_MAX72XX::ON);
		delay(nDelay);
		mxt.control(MD_MAX72XX::TEST, MD_MAX72XX::OFF);
		delay(nDelay);

		nDelay -= DELAYTIME;
	}
}

void scanLimit(void)
// Uses scan limit function to restrict the number of rows displayed.
{
	PRINTS("\nScan Limit");
	mxt.clear();

	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
	for(uint8_t row = 0; row < ROW_SIZE; row++)
		mxt.setRow(row, 0xff);
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

	for(int8_t s = MAX_SCANLIMIT; s >= 0; s--) {
		mxt.control(MD_MAX72XX::SCANLIMIT, s);
		delay(DELAYTIME * 5);
	}
	mxt.control(MD_MAX72XX::SCANLIMIT, MAX_SCANLIMIT);
}

void transformation1()
// Demonstrates the use of transform() to move bitmaps on the display
// In this case a user defined bitmap is created and animated.
{
	uint8_t arrow[COL_SIZE] =
		{
			0b00001000,
			0b00011100,
			0b00111110,
			0b01111111,
			0b00011100,
			0b00011100,
			0b00111110,
			0b00000000};

	MD_MAX72XX::transformType_t t[] =
		{
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TFLR,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TRC,
			MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
			MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
			MD_MAX72XX::TFUD,
			MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
			MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
			MD_MAX72XX::TINV,
			MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
			MD_MAX72XX::TINV};

	PRINTS("\nTransformation1");
	mxt.clear();

	// use the arrow bitmap
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
	for(uint8_t j = 0; j < mxt.getDeviceCount(); j++)
		mxt.setBuffer(((j + 1) * COL_SIZE) - 1, COL_SIZE, arrow);
	mxt.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
	delay(DELAYTIME);

	// run through the transformations
	mxt.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
	for(uint8_t i = 0; i < (sizeof(t) / sizeof(t[0])); i++) {
		mxt.transform(t[i]);
		delay(DELAYTIME * 4);
	}
	mxt.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
}

void transformation2()
// Demonstrates the use of transform() to move bitmaps on the display
// In this case font characters are loaded into the display for animation.
{
	MD_MAX72XX::transformType_t t[] =
		{
			MD_MAX72XX::TINV,
			MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
			MD_MAX72XX::TINV,
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
			MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
			MD_MAX72XX::TSD, MD_MAX72XX::TSU, MD_MAX72XX::TSD, MD_MAX72XX::TSU,
			MD_MAX72XX::TFLR, MD_MAX72XX::TFLR, MD_MAX72XX::TFUD, MD_MAX72XX::TFUD};

	PRINTS("\nTransformation2");
	mxt.clear();
	mxt.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);

	// draw something that will show changes
	for(uint8_t j = 0; j < mxt.getDeviceCount(); j++) {
		mxt.setChar(((j + 1) * COL_SIZE) - 1, '0' + j);
	}
	delay(DELAYTIME * 5);

	// run thru transformations
	for(uint8_t i = 0; i < (sizeof(t) / sizeof(t[0])); i++) {
		mxt.transform(t[i]);
		delay(DELAYTIME * 3);
	}
}

void wrapText()
// Display text and animate scrolling using auto wraparound of the buffer
{
	PRINTS("\nwrapText");
	mxt.clear();
	mxt.wraparound(MD_MAX72XX::ON);

	// draw something that will show changes
	for(uint16_t j = 0; j < mxt.getDeviceCount(); j++) {
		mxt.setChar(((j + 1) * COL_SIZE) - 1, (j & 1 ? 'M' : 'W'));
	}
	delay(DELAYTIME * 5);

	// run thru transformations
	for(uint16_t i = 0; i < 3 * COL_SIZE * MAX_DEVICES; i++) {
		mxt.transform(MD_MAX72XX::TSL);
		delay(DELAYTIME / 2);
	}
	for(uint16_t i = 0; i < 3 * COL_SIZE * MAX_DEVICES; i++) {
		mxt.transform(MD_MAX72XX::TSR);
		delay(DELAYTIME / 2);
	}
	for(uint8_t i = 0; i < ROW_SIZE; i++) {
		mxt.transform(MD_MAX72XX::TSU);
		delay(DELAYTIME * 2);
	}
	for(uint8_t i = 0; i < ROW_SIZE; i++) {
		mxt.transform(MD_MAX72XX::TSD);
		delay(DELAYTIME * 2);
	}

	mxt.wraparound(MD_MAX72XX::OFF);
}

// Display wrapping text.
void wrapText2(const char *text)
{
	mxt.clear();

	uint8_t charWidth;
	uint8_t cBuf[8];

	uint8_t length = strlen(text);
	uint8_t pos = 0;
	// The buffer must be at least twice the size of the longest string
	char *buffer = "                                                         ";
	// Create a copy
	memcpy(buffer, text, length);
	// Create a second copy
	memcpy(buffer + length + 1, text, length);

	while(true) {
		charWidth = mxt.getChar(buffer[pos++], sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
		for(uint8_t i = 0; i <= charWidth; i++)	 // allow space between characters
		{
			mxt.transform(MD_MAX72XX::TSL);
			mxt.setColumn(0, (i < charWidth) ? cBuf[i] : 0);
			delay(DELAYTIME);
		}
		if(pos == length) {
			pos = 0;
		}
	}
}

void showCharset(void)
// Run through display of the the entire font characters set
{
	mxt.clear();
	mxt.update(MD_MAX72XX::OFF);

	for(uint16_t i = 0; i < 256; i++) {
		mxt.clear(0);
		mxt.setChar(COL_SIZE - 1, i);

		if(MAX_DEVICES >= 3) {
			char hex[3];

			sprintf(hex, "%02X", i);

			mxt.clear(1);
			mxt.setChar((2 * COL_SIZE) - 1, hex[1]);
			mxt.clear(2);
			mxt.setChar((3 * COL_SIZE) - 1, hex[0]);
		}

		mxt.update();
		delay(DELAYTIME * 2);
	}
	mxt.update(MD_MAX72XX::ON);
}

// -----------------------------------------------------------------------------

void ledMatrixTestsSetup()
{
#if DEBUG
	Serial.begin(57600);
#endif

	PRINTS("\n[MD_MAX72XX Test & Demo]");

	if(!mxt.begin())
		PRINTS("\nMD_MAX72XX initialization failed");

	mxt.clear();
	mxt.control(MD_MAX72XX::INTENSITY, 1);

	wrapText2("Wrap text demo ... ");
}

void ledMatrixTestsLoop()
{
	// scrollText("Graphics");
//   zeroPointSet();
//   rows();
//   columns();
//   cross();
//   stripe();
//   checkboard();
//   bullseye();
//   bounce();
//   spiral();

	// #if 1
	//   scrollText("Control");
	//   intensity();
	//   scanLimit();
	//   blinking();
	// #endif

	// #if 1
	//   scrollText("Transform");
	//   transformation1();
	//   transformation2();
	// #endif

	// #if 1
	//   scrollText("Charset");
	// wrapText();
	// wrapText2();
	//   showCharset();
	// #endif
}
