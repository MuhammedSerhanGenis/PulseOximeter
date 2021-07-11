#include "MAX30100_PulseOximeter.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "OakOLED.h"
#define REPORTING_PERIOD_MS 500
#define batarya A0
OakOLED oled;

PulseOximeter pox;

const int okumaSayisi = 60;
float filtreOrani = 0.5;
uint32_t alinanSonOrnek = 0;
uint32_t sonAtim = 0;
int okunanSayi = 0;
int ortalamaAtim = 0;
int ortalamaSpo2 = 0;
bool hesaplamaTamamlandi = false;
bool hesaplaniyor = false;
bool baslat = false;
byte beat = 0;

int yuzde = 0.0;
int maxAtim = 100;
int maxSpO2 = 100;
int minAtim = 60;
int minSpo2 = 90;
int bekle = 0;
bool max_min = false;
bool uyari = false;
bool mesaj = true;
int uyariTipi = 0;

void atimAlgilandi()
{
  sonAtim = millis();
  if ((millis() == sonAtim) and (uyari))
  {
    bekle++;
    if (bekle < 6)
    {
      degerEkrani_bluetoothGonder();
    }
    if (bekle == 6)
    {
      uyariEkrani();
    }
    if (bekle == 11)
    {
      bekle = 0;
    }
  }
}

void acilisEkrani()
{
  if (not baslat)
  {
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(36, 0);
    oled.print(F("PLACE"));
    oled.setCursor(30, 16);
    oled.print(F("FINGER"));
    oled.setCursor(30, 32);
    oled.print(F("ON THE"));
    oled.setCursor(30, 48);
    oled.print(F("SENSOR"));
    oled.display();
    baslat = true;
  }
}

void hesaplamaEkrani(int j)
{
  if (not hesaplaniyor) {
    oled.clearDisplay();
    hesaplaniyor = true;
    baslat = false;
  }
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(1);
  oled.setCursor(8, 0);
  oled.print(F("MEASURING"));
  for (int i = 0; i <= j; i++) {
    yuzde = map(i, 0, okumaSayisi, 0, 100);
  }
  oled.setTextSize(3);
  oled.setCursor(36, 30);
  oled.print(yuzde);
  oled.print(F("%"));
  oled.display();
}

void uyariEkrani()
{
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(1);
  oled.setCursor(22, 0);
  oled.print(F("WARNING!"));

  if (uyariTipi == 1)
  {
    if (max_min)
    {
      oled.setCursor(45, 16);
      oled.print(F("HIGH"));
    }
    if (not max_min)
    {
      oled.setCursor(60, 16);
      oled.print(F("LOW"));
    }
    oled.setCursor(38, 32);
    oled.print(F("HEART"));
  }
  if (uyariTipi == 2)
  {
    if (max_min)
    {
      oled.setCursor(45, 16);
      oled.print(F("HIGH"));
    }
    if (not max_min)
    {
      oled.setCursor(60, 16);
      oled.print(F("LOW"));
    }
    oled.setCursor(45, 32);
    oled.print(F("SPO2"));
  }
  if (uyariTipi == 3)
  {
    if (max_min)
    {
      oled.setCursor(45, 16);
      oled.print(F("HIGH"));
    }
    if (not max_min)
    {
      oled.setCursor(60, 16);
      oled.print(F("LOW"));
    }
    oled.setCursor(0, 32);
    oled.print(F("HEART-SPO2"));
  }
  if (uyariTipi == 4)
  {
    oled.setCursor(20, 16);
    oled.print(F("LOW HEART"));
    oled.setCursor(20, 32);
    oled.print(F("HIGH SPO2"));
  }
  if (uyariTipi == 5)
  {
    oled.setCursor(18, 16);
    oled.print(F("HIGH HEART"));
    oled.setCursor(23, 32);
    oled.print(F("LOW SPO2"));
  }

  oled.setCursor(45, 48);
  oled.print(F("RATE"));
  oled.display();
  //uyari=false;
  //delay(2500);

}
void degerEkrani_bluetoothGonder()
{
  float lipo = (5.00 / 1024.00) * analogRead(batarya);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(1);
  oled.setCursor(0, 0);
  oled.print(F("BPM : "));
  oled.print(ortalamaAtim);
  oled.setCursor(0, 18);
  oled.print(F("SpO2: "));
  oled.print(ortalamaSpo2);
  oled.print(F("%"));
  oled.setCursor(0, 36);
  oled.print(F("LiPo:"));
  oled.print(lipo);
  oled.print(F("v"));
  oled.display();
  if (mesaj)
  {
    Serial.print(F("BPM: "));
    Serial.println(ortalamaAtim);
    Serial.print(F("SpO2: "));
    Serial.print(ortalamaSpo2);
    Serial.println(F("%"));
    if (not uyari)
    {
      Serial.print(lipo);
      Serial.println(F("v"));
    }
    if (uyari)
    {
      Serial.print(lipo);
      Serial.println(F("v"));
      if (uyariTipi == 1)
      {
        Serial.print(F("WARNING!."));
        if (max_min)
          Serial.print(F("HIGH "));
        if (not max_min)
          Serial.print(F("LOW "));
        Serial.println(F("HEART RATE"));
      }
      if (uyariTipi == 2)
      {
        Serial.print(F("WARNING!."));
        if (max_min)
          Serial.print(F("HIGH "));
        if (not max_min)
          Serial.print(F("LOW "));
        Serial.println(F("SPO2 RATE"));
      }
      if (uyariTipi == 3)
      {
        Serial.print(F("WARNING!."));
        if (max_min)
          Serial.print(F("HIGH "));
        if (not max_min)
          Serial.print(F("LOW "));
        Serial.println(F("HEART AND SPO2 RATE"));
      }
      if (uyariTipi == 4)
      {
        Serial.print(F("WARNING!.LOW HEART AND HIGH SPO2 RATE"));
      }
      if (uyariTipi == 5)
      {
        Serial.print(F("WARNING!.HIGH HEART AND LOW SPO2 RATE"));
      }
    }
    mesaj = false;
  }
}

void ortalamaHesapla(int beat, int SpO2)
{
  if (okunanSayi == okumaSayisi) {
    hesaplamaTamamlandi = true;
    hesaplaniyor = false;
    baslat = false;
    okunanSayi = 0;
    if (ortalamaAtim > maxAtim)
    {
      uyariTipi = 1;
      uyari = true;
      max_min = true;
    }
    if (minAtim > ortalamaAtim)
    {
      uyariTipi = 1;
      uyari = true;
      max_min = false;
    }
    if (ortalamaSpo2 > maxSpO2)
    {
      uyariTipi = 2;
      uyari = true;
      max_min = true;
    }
    if (minSpo2 > ortalamaSpo2)
    {
      uyariTipi = 2;
      uyari = true;
      max_min = false;
    }
    if ((ortalamaAtim > maxAtim) and (ortalamaSpo2 > maxSpO2))
    {
      uyariTipi = 3;
      uyari = true;
      max_min = true;
    }

    if ((minAtim > ortalamaAtim) and (minSpo2 > ortalamaSpo2))
    {
      uyariTipi = 3;
      uyari = true;
      max_min = false;
    }

    if ((minAtim > ortalamaAtim) and (ortalamaSpo2 < maxSpO2))
    {
      uyariTipi = 4;
      uyari = true;
    }

    if ((ortalamaAtim > maxAtim) and (minSpo2 > ortalamaSpo2))
    {
      uyariTipi = 5;
      uyari = true;
    }
    degerEkrani_bluetoothGonder();
  }

  if (not hesaplamaTamamlandi and beat > 30 and beat<220 and SpO2>50)
  {
    ortalamaAtim = filtreOrani * (beat) + (1 - filtreOrani ) * ortalamaAtim;
    ortalamaSpo2 = filtreOrani * (SpO2) + (1 - filtreOrani ) * ortalamaSpo2;
    okunanSayi++;
    hesaplamaEkrani(okunanSayi);
  }
}

void setup()
{
  pinMode(batarya, INPUT);
  Serial.begin(9600);

  oled.begin();
  oled.clearDisplay();
  delay(1000);
  if (!pox.begin())
  {
    for (;;);
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(atimAlgilandi);
  acilisEkrani();
}

void loop()
{
  pox.update();

  if ((millis() - alinanSonOrnek > REPORTING_PERIOD_MS) and (not hesaplamaTamamlandi) and (not uyari))
  {
    ortalamaHesapla(pox.getHeartRate(), pox.getSpO2());
    alinanSonOrnek = millis();
  }

  if ((millis() - sonAtim > 6000))
  {
    hesaplamaTamamlandi = false;
    uyari = false;
    mesaj = true;
    uyariTipi = 0;
    bekle = 0;
    yuzde = 0;
    ortalamaAtim = 0;
    ortalamaSpo2 = 0;
    acilisEkrani();
  }
}
