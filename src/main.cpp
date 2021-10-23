#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

// OLED declaration
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C g_OLED(U8G2_R0, 5, 4);
int g_lineHeight;
String disp;

// PWM control 
int fan_percentage;
int maxPotRead = 1023;

// Pins
#define POT_PIN A6
#define FAN_PWM A7

void updateOLED(int fan_percentage){
  // Clear the buffer
  g_OLED.clearBuffer();
  // Print the fan speed
  g_OLED.setCursor(10, g_lineHeight);
  disp = String(fan_percentage) + "%   ";
  g_OLED.print(disp);
  // Send buffer
  g_OLED.sendBuffer();
}

int round5(int in) {
 static signed char round5delta[5] = {0, -1, -2, 2, 1};  // difference to the "rounded to nearest 5" value
 return in + round5delta[in%5];
}

void setup() {
  // Pin setup
  pinMode(POT_PIN, INPUT);
  pinMode(FAN_PWM, OUTPUT);

  // OLED setup
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.clearDisplay();
  g_OLED.setFont(u8g2_font_profont29_tf);
  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();
  g_OLED.sendBuffer();

  // Serial
  Serial.begin(9600);
}

void loop() {
  // Check the potentiometer
  fan_percentage = int(map(analogRead(POT_PIN), 0, maxPotRead, 0, 100));
  if (fan_percentage > 100){
    fan_percentage = 100;
  }
  else if (fan_percentage < 3){
    fan_percentage = 0;
  }

  fan_percentage = round5(fan_percentage);
  
  // Run the fan at PWM
  analogWrite(FAN_PWM, map(fan_percentage, 0, 100, 0, 255));

  // Update the screen
  updateOLED(fan_percentage);
}