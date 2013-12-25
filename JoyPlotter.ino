#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SD.h>
#include <SPI.h>

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define SD_CS    4  // Chip select line for SD card
#define TFT_CS  10  // Chip select line for TFT display
#define TFT_DC   8  // Data/command line for TFT
#define TFT_RST  -1  // Reset line for TFT (or connect to +5V)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

int x = 10;
int y = 10;

void setup() {
  x = 10;
  y = 10;
  
  tft.initR(INITR_REDTAB);
  tft.fillScreen(0x0000);
  drawScreen();
}

uint8_t readButton(void) {
  float a = analogRead(3);
  
  a *= 5.0;
  a /= 1024.0;
  
  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}

void clearScreen(){
  tft.fillRect(0, 0, 35, 16, ST7735_BLACK);
  tft.drawFastHLine(0, y, tft.width(), ST7735_BLACK);
  tft.drawFastVLine(x, 0, tft.height(), ST7735_BLACK);
}

void drawScreen(){
  tft.drawFastHLine(0, y, tft.width(), ST7735_RED);
  tft.drawFastVLine(x, 0, tft.height(), ST7735_BLUE);
  tft.drawPixel(x,y, ST7735_GREEN);
  
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.print("x: ");
  tft.println(x);
  tft.print("y: ");
  tft.println(y);
}

void loop() {
  int dx = 0;
  int dy = 0;
  
  uint8_t b = readButton();
  
  if (b == BUTTON_DOWN)
    dy++;
  else if (b == BUTTON_LEFT)
    dx--;
  else if (b == BUTTON_UP)
    dy--;
  else if (b == BUTTON_RIGHT)
    dx++;
  else if (b == BUTTON_SELECT){
    /**
    tft.fillScreen(ST7735_BLACK);
    tft.setRotation(tft.getRotation()+1);
    drawScreen();
    delay(500);
    */
    
    tft.invertDisplay(true);
    delay(100);
    tft.invertDisplay(false);
    delay(100);
    tft.invertDisplay(true);
    delay(250);
    tft.invertDisplay(false);
    return;
  }
  else
    return;
  
  clearScreen();
  
  x += dx;
  y += dy;
  
  if(x > tft.width())
    x = 0;
  else if(x < 0)
    x = tft.width();
  
  if(y > tft.height())
    y = 0;
  else if(y < 0)
    y = tft.height();
  
  drawScreen();
}
