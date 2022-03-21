/***************************************************
   This is a example sketch demonstrating graphic drawing
   capabilities of the SSD1351 library for the 1.5"
   and 1.27" 16-bit Color OLEDs with SSD1351 driver chip

   Pick one up today in the adafruit shop!
   ------> http://www.adafruit.com/products/1431
   ------> http://www.adafruit.com/products/1673

   If you're using a 1.27" OLED, change SCREEN_HEIGHT to 96 instead of 128.

   These displays use SPI to communicate, 4 or 5 pins are required to
   interface
   Adafruit invests time and resources providing this open source code,
   please support Adafruit and open-source hardware by purchasing
   products from Adafruit!

   Written by Limor Fried/Ladyada for Adafruit Industries.
   BSD license, all text above must be included in any redistribution

   The Adafruit GFX Graphics core library is also required
   https://github.com/adafruit/Adafruit-GFX-Library
   Be sure to install it!
  ****************************************************/

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins
#define SCLK_PIN 13
#define MOSI_PIN 11
#define DC_PIN 8
#define CS_PIN 10
#define RST_PIN 9

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define NUMBER_OF_VERTICIES 10
#define MAX_VELOCITY 3

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <Bounce2.h>
#include <Encoder.h>

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

float p = 3.1415926;

int pixels[NUMBER_OF_VERTICIES][2];
int velocities[NUMBER_OF_VERTICIES][2];
int colors[] = {BLUE, RED, GREEN, YELLOW, MAGENTA, CYAN, WHITE};
int numOfVerticesToDraw = NUMBER_OF_VERTICIES;
int encValue = -999;
Encoder enc(0, 1);
Button button = Button();

void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  tft.begin();

  Serial.println("init");

  // You can optionally rotate the display by running the line below.
  // Note that a value of 0 means no rotation, 1 means 90 clockwise,
  // 2 means 180 degrees clockwise, and 3 means 270 degrees clockwise.
  // tft.setRotation(1);
  button.attach(32, INPUT_PULLUP);
  button.interval(5);
  button.setPressedState(LOW);

  uint16_t time = millis();
  tft.fillRect(0, 0, 128, 128, BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);



  // optimized lines
  randomizeVerticies();
}

void randomizeVerticies() {
  for (int i = 0; i < NUMBER_OF_VERTICIES; i++) {
    pixels[i][0] = random(0, SCREEN_WIDTH);
    pixels[i][1] = random(0, SCREEN_WIDTH);
//     velocities[i][0] = random(0, 2);
//     velocities[i][1] = random(0, 2);
//     if (velocities[i][0] == 0) {
//       velocities[i][0] = -1;
//     }
//   }

    velocities[i][0] = random(-MAX_VELOCITY, MAX_VELOCITY);
    velocities[i][1] = random(-MAX_VELOCITY, MAX_VELOCITY);
    if (velocities[i][0] == 0) {
      velocities[i][0] = (i % 2 == 0) ? -1 : 1;
    }
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(BLACK);
  for (uint16_t y = 0; y < tft.height() - 1; y += 5) {
    tft.drawFastHLine(0, y, tft.width() - 1, color1);
  }
  for (uint16_t x = 0; x < tft.width() - 1; x += 5) {
    tft.drawFastVLine(x, 0, tft.height() - 1, color2);
  }
}

void movePixels(){
    for(int i=0; i< NUMBER_OF_VERTICIES; i++){
        pixels[i][0] += velocities[i][0];
        pixels[i][1] += velocities[i][1];
        if(pixels[i][0] < 0){
            pixels[i][0] = 0;
            velocities[i][0] = -velocities[i][0];
        }
        if(pixels[i][0] > SCREEN_WIDTH){
            pixels[i][0] = SCREEN_WIDTH;
            velocities[i][0] = -velocities[i][0];
        }
        if(pixels[i][1] < 0){
            pixels[i][1] = 0;
            velocities[i][1] = -velocities[i][1];
        }
        if(pixels[i][1] > SCREEN_HEIGHT){
            pixels[i][1] = SCREEN_HEIGHT;
            velocities[i][1] = -velocities[i][1];
        }
    }
}

void drawLinesBetweenPixels(){
  for (int i = 0; i < numOfVerticesToDraw; i++) {

    // tft.drawLine(pixels[i][0], pixels[i][1], pixels[(i + 1) % NUMBER_OF_VERTICIES][0], pixels[(i + 1) % NUMBER_OF_VERTICIES][1], colors[i % 7]);
    // tft.drawLine(pixels[i][0], pixels[i][1], pixels[(i + 2) % NUMBER_OF_VERTICIES][0], pixels[(i + 2) % NUMBER_OF_VERTICIES][1], colors[i % 7]);
    tft.drawLine(pixels[i][0], pixels[i][1], pixels[(i + 1) % numOfVerticesToDraw][0], pixels[(i + 1) % numOfVerticesToDraw][1], colors[i % 7]);
    tft.drawLine(pixels[i][0], pixels[i][1], pixels[(i + 2) % numOfVerticesToDraw][0], pixels[(i + 2) % numOfVerticesToDraw][1], colors[i % 7]);
    tft.drawLine(pixels[(i + 1) % numOfVerticesToDraw][0], pixels[(i + 1) % numOfVerticesToDraw ][1], pixels[(i + 2) % numOfVerticesToDraw][0], pixels[(i + 2) % numOfVerticesToDraw][1], colors[i % 7]);
  }
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the screen
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  static const uint16_t PROGMEM colors[] =
    { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, BLACK, WHITE };

  for(uint8_t c=0; c<8; c++) {
    tft.fillRect(0, tft.height() * c / 8, tft.width(), tft.height() / 8,
      pgm_read_word(&colors[c]));
  }
}

void loop() {
  tft.fillScreen(BLACK);
  movePixels();
  drawLinesBetweenPixels();

  if (button.pressed()) {
    randomizeVerticies();
    Serial.println("button pressed");
  }
  long newEncValue = enc.read();
  if ((newEncValue != encValue) && (abs(newEncValue - encValue) > 4)) {
      encValue = newEncValue;
      Serial.println("env value: " + String(encValue));
      numOfVerticesToDraw = (numOfVerticesToDraw + 1) % NUMBER_OF_VERTICIES;
      numOfVerticesToDraw = numOfVerticesToDraw < 4 ? 4 : numOfVerticesToDraw;
  }
}
