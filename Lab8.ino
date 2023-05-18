#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
String fil = "";//File name
File myFile;

int SamplingRate;


int inputSamplingRate(){
  String input="";
  int rate;
  while(rate=0){
    input = Serial.readString();
    input.trim(); //去除字串尾段的換行與回車鍵
    rate= input.toInt();
  }
  return rate;
}

void ECG(int rate){
  Serial.begin(9600);
    if((digitalRead(10)==1) || (digitalRead(11)==1)){
      Serial.println("NIL");
    }
    else{
      Serial.println(analogRead(A0));
    }
    delay(10);
    Serial.end();
    delay(10);
}

void OLED(double value){
  String SerialData="";
  SerialData = String(value,4);
  const char* x=SerialData.c_str();
  u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(20, 29,x );
}

void SDCard(double data){
  myFile = SD.open(fil, FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void setup() {
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void loop() {
  double value;
  value= inputSamplingRate();
  ECG(value);
  OLED(value);
  SDCard(value);

}
