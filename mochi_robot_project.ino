#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Core Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Timing variables for non-blocking animation loops
unsigned long prevBlinkMillis = 0;
bool isBlinking = false;

void setup() {
  Serial.begin(115200);

  // Initialize the OLED Display at standard I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Halt execution if screen isn't wired properly
  }
  
  display.clearDisplay();
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  // Natural Blinking Timer (Blinks every 2.5 seconds for 120 milliseconds)
  if (!isBlinking && (currentMillis - prevBlinkMillis >= 2500)) {
    isBlinking = true;
    prevBlinkMillis = currentMillis;
  }
  if (isBlinking && (currentMillis - prevBlinkMillis >= 120)) {
    isBlinking = false;
    prevBlinkMillis = currentMillis;
  }

  // Clear display canvas frame
  display.clearDisplay();
  
  // Render the current animation frame state
  if (isBlinking) {
    drawCuteBlink();
  } else {
    drawCuteNormal();
  }
  
  // Push graphic vector frame buffer onto active glass
  display.display();
}

// ==========================================
// CUTE RECTANGULAR VECTOR GRAPHICS DICTIONARY
// ==========================================

// 😐 BIG RECTANGULAR EYES + CURVED ANIME SMILE
void drawCuteNormal() {
  // Left Eye: X=20, Y=14, Width=24, Height=28, Corner_Radius=4
  display.fillRoundRect(20, 14, 24, 28, 4, SSD1306_WHITE); 
  // Right Eye: X=84, Y=14, Width=24, Height=28, Corner_Radius=4
  display.fillRoundRect(84, 14, 24, 28, 4, SSD1306_WHITE); 
  
  // Wide open-arc smile (U-shape curve outline)
  display.drawLine(54, 45, 57, 49, SSD1306_WHITE);
  display.drawLine(57, 49, 71, 49, SSD1306_WHITE);
  display.drawLine(71, 49, 74, 45, SSD1306_WHITE);
}

// 😉 SLEEPY / BLINKING FLAT RECTANGLES
void drawCuteBlink() {
  // Eyes collapse down into narrow horizontal rectangles
  display.fillRoundRect(20, 26, 24, 6, 2, SSD1306_WHITE); 
  display.fillRoundRect(84, 26, 24, 6, 2, SSD1306_WHITE); 
  
  // Keep the friendly smile visible during a blink
  display.drawLine(54, 45, 57, 49, SSD1306_WHITE);
  display.drawLine(57, 49, 71, 49, SSD1306_WHITE);
  display.drawLine(71, 49, 74, 45, SSD1306_WHITE);
}