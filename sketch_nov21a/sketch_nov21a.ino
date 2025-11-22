#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2     
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Sistem Basliyor");
  delay(100);
  lcd.clear();
}

void loop() {
  // 1. Ölçüm Al
  float t = dht.readTemperature();

  // 2. Hata Kontrolü
  if (isnan(t)) {
    Serial.println("Hata");
    lcd.print("Sensor Hatasi!");
    return;
  }

  // 3. Bilgisayara Gönder (CSV için saf veri)
  Serial.println(t); 

  // 4. Ekrana Yazdır (Görsellik için)
  lcd.setCursor(0,0); 
  lcd.print("Sicaklik:"); // Üst satır
  
  lcd.setCursor(0,1); 
  lcd.print(t);           // Alt satır sıcaklık değeri
  lcd.print(" C");        // Birim
  delay(2000);
}