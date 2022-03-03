#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include <Bounce2.h>
#include <GEM_adafruit_gfx.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display = Adafruit_SSD1306 (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Encoder knob1(0, 1);
int buttonCancelPin = 6;
int buttonOkayPin = 12;
Button buttonCancel = Button();
Button buttonOkay = Button();



// Use this MIDI channel.
#define MIDI_CHANNEL 1

long positionKnob1  = -999;


// menu stuff....
GEM_adafruit_gfx menu(display, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
int number = -512;
boolean enablePrint = false;
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Print?:", enablePrint);
void printData(); // Forward declaration
GEMItem menuItemButton("Print", printData);
GEMPage menuPageMain("Main Menu");

void setup() {

  Serial.begin(38400);  
  while (usbMIDI.read()) {
  // MIDI Controllers should discard incoming MIDI messages.    
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  buttonCancel.attach( buttonCancelPin,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  buttonOkay.attach( buttonOkayPin, INPUT_PULLUP );
  buttonCancel.interval(5); 
  buttonOkay.interval(5);   
  buttonCancel.setPressedState(LOW); 
  buttonOkay.setPressedState(LOW); 
  
  display.setRotation(2);
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

//  this works
//  display.fillScreen(0xFFFF);
//  display.display();
//  delay(1000); // Pause for 2 seconds  

  menu.init(); // menu init makes the display stop working... think it had to do with fonts.... 
//  menu.setForegroundColor(0);
  setupMenu();
  menu.drawMenu();
  display.display();
}

void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}


void drawText(String text, uint8_t line) {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,(10 * line));             // Start at top-left corner
  display.println(text);
  display.display();

}

void printData() {
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint) {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(number);
  } else {
    Serial.println("Printing is disabled, sorry:(");
  }
}

void loop() {
  if (menu.readyForKey()) {
    buttonCancel.update();
    buttonOkay.update();
    
    if (buttonCancel.pressed()) {
      Serial.println("cancel");
      menu.registerKeyPress(GEM_KEY_CANCEL);
      display.display();
    }
    
  

    if (buttonOkay.pressed()) {
      menu.registerKeyPress(GEM_KEY_OK);
      Serial.println("okay");      
      display.display();
    }
    
    long newKnob1Value = knob1.read();
    if ((newKnob1Value != positionKnob1) && ((newKnob1Value - positionKnob1) % 4 == 0 )) {
      if(newKnob1Value > positionKnob1){
        Serial.println("menu up");
        menu.registerKeyPress(GEM_KEY_UP);
        display.display();
      } else {
        Serial.println("menu down");        
        menu.registerKeyPress(GEM_KEY_DOWN);
        display.display();
      }
      positionKnob1 = newKnob1Value;      
//      drawText("Knob 1: " + String(positionKnob1), 1);
      Serial.print("knob1 = ");
      Serial.print(newKnob1Value);
      Serial.println();
    }
  }
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset knobs to zero");
    knob1.write(0);
  }
}
