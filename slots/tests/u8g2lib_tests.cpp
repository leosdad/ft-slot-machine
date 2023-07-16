
#include <U8g2lib.h>  // U8g2lib.h should be enclosed in less than and great than brackets

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

void setup()
{
	u8g2.begin();
	u8g2.setFont(u8g2_font_fub17_tf);
	u8g2.drawStr(10, 26, "Coins:");
	u8g2.drawStr(10, 52, "1.234.567");
	u8g2.sendBuffer();
}

void loop()
{
}
