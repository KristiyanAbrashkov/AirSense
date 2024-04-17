#define BLYNK_TEMPLATE_ID "TMPL4rmyrV_SI"
#define BLYNK_TEMPLATE_NAME "Измерване на качеството на въздуха"
#define BLYNK_AUTH_TOKEN "43XDA8FfKDr7P6PlUjEunaEwNwYKdtf-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> // Correct WiFi library for ESP8266
#include <BlynkSimpleEsp8266.h> // Correct Blynk library for ESP8266

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = " "; // Your WiFi name
char pass[] = " "; // Your WiFi password

BlynkTimer timer;

const int gasPin = A0; // ESP8266 analog input pin is A0
int sensorThreshold = 100;

#define DHTPIN 4 // DHT sensor connected to pin 4
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius by default

 /* if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
*/
  int co2Level = analogRead(gasPin); // Read CO2 level from MQ-135
  Blynk.virtualWrite(V2, co2Level);
 //Blynk.virtualWrite(V0, t);
 // Blynk.virtualWrite(V1, h);

  Serial.print("CO2 Level: "); Serial.println(co2Level);
//  Serial.print("Temperature: "); Serial.print(t); Serial.print(" C  ");
 // Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(30000L, sendSensor);

  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(3, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(2000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int co2Level = analogRead(gasPin);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.write(1); // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print("%");
  delay(4000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("CO2: ");
  lcd.print(co2Level);
  lcd.print(" ppm");
  delay(4000);
  lcd.clear();
}
