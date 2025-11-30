#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 9
#define FAN_PIN 3

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
      Serial.print("{\"temp\": ");
      Serial.print(t);
      Serial.print(", \"hum\": ");
      Serial.print(h);
      Serial.println("}");
    }
  }
  
  if(Serial.available() > 0){
    String command = Serial.readStringUntil('\n');
    command.trim();

    if(command.startsWith("VOLT:")){
      String valStr = command.substring(5);
      float voltage = valStr.toFloat();

      if (voltage > 5.0) voltage = 5.0;
      if (voltage < 0.0) voltage = 0.0;

      int pwmValue = (voltage / 5.0) * 255;
      
      analogWrite(FAN_PIN, pwmValue);
    }
  }
}