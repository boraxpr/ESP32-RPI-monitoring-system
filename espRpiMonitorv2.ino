#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "DHT.h"

#define DHTTYPE DHT10
#define TRIG_PIN 32
#define ECHO_PIN 33
long duration;
int distance;

DHT dht(0, DHTTYPE);

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
uint32_t updateTime = 0;       // time for next update

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the ECHO_PIN as an Input
  tft.init();
  Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(3);
  Wire.begin();

  dht.begin();

  updateTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp_hum_val[2] = {0}; 
  if (updateTime <= millis()) {
    if(!dht.readTempAndHumidity(temp_hum_val)){
//      Serial.print("Humidity: "); 
//      Serial.print(temp_hum_val[0]);
//      Serial.print(" %\t");
//      Serial.print("Temperature: "); 
//      Serial.print(temp_hum_val[1]);
//      Serial.println(" *C");
          // Clears the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Sets the TRIG_PIN on HIGH state for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;

  byte font = 7;
  byte sfont = 2;
  int padding = tft.textWidth("99999",font);
  int spadding = tft.textWidth("Temperature",sfont);
  byte decimal_places = 2;
  int x = 170;
  int y = 50;
    header("ESP32-RPi Monitor");
    tft.setCursor(0, 0, 1);
    
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.setTextPadding(spadding);
    tft.drawString("Humidity", x-140, y, sfont);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Temperature", x-130, y+60, sfont);
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.drawString("Distance", x-140, y+120, sfont);
    tft.setTextPadding(padding);
        tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawFloat(temp_hum_val[0],decimal_places,x-10,y,font);
    tft.drawString("%", x+138, y, sfont);
        tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawFloat(temp_hum_val[1],decimal_places,x-10,y+60,font);
    tft.drawString("*C", x+138, y+60, sfont);
        tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.drawFloat(distance,decimal_places,x-10,y+120,font);
    tft.drawString("cm", x+138, y+120, sfont);
//    tft.println("  %");
//    tft.print("T:");
//    tft.print(temp_hum_val[1]);
//    tft.println(" *C");
//    tft.print("d:");
//    tft.print(distance);
//    tft.println(" cm");
    Serial.println(distance);
  }
  else{
     Serial.println("Failed to get temprature and humidity value.");
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0, 1);
      tft.setTextColor(TFT_RED,TFT_BLACK);  tft.setTextSize(5);
      tft.print("Fail");
      tft.fillScreen(TFT_BLACK);
  }
 }
 delay(500);
// tft.fillScreen(TFT_BLACK);
}

// Print the header for a display screen
void header(const char *string)
{
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(string, 160, 10, 4);
 
}
