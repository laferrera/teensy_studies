#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
int octaves[6] = {0,819,1638,2457,3276,4095};
int octaveIndex = 0;
float semitone = 68.25f;
int notes[] = {0,7,0,5,0,7};
void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x60);
    
}

void loop(void) {
  

  for(int oct = 0; oct < 4; oct++){
      for(int i = 0; i < 6; i++){
          int note = notes[i]; 
          int volt = oct * (819) + note * semitone;
          dac.setVoltage(volt, false);    
          Serial.println(String(volt));
          delay(200);
      }    
  }
  
//  for(int i; i < 6; i++){
//      dac.setVoltage(octaves[i], false);    
//      Serial.println(octaves[i]);
//      delay(200);
//  }
}
