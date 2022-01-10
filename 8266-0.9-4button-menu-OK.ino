// written for LilyGO TTGO ESP8266 - with 0.91 inch OLED Display
// Arduino IDE module generic ESP8266
#include        <Arduino.h>
#include        <U8g2lib.h>         // chose font https://github.com/olikraus/u8g2/wiki/fntgrp fonts   
#include        <SPI.h>
#include        <Wire.h>
#define x 0                         // screenpos to display info
#define y 30                        // screenpos to display info 
#define OLED_SDA  2
#define OLED_SCL 14
#define OLED_RST  4
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA , OLED_RST);

#define lbutton 13
#define rbutton 15
int    threshold = 25;
int oldthreshold;
String  temp; 

void setup() {
  Serial.begin        (115200);
  u8g2.begin          ();
  delay               (500);
  Serial.println      (__FILE__);//name of this doc 
  pinMode (lbutton,   INPUT);
  pinMode (rbutton,   INPUT);
  Serial.println      ("done!");
  displaystring       ("done!");
}

void loop() {
  //READ SWITCH
if (digitalRead (lbutton)==1||digitalRead (rbutton)==1){buttons();}
  Serial.print      ("lbutton:");
  Serial.print      (digitalRead (lbutton));
  Serial.print      (" rbutton:");
  Serial.print      (digitalRead (rbutton));
  temp = String     (threshold);
  temp += "%";
  Serial.print      (" threshold:");
  Serial.println    (threshold);
  displaystring     (temp);
  delay             (100);
}

void displaystring  (String value){
  u8g2.setFont      (u8g2_font_courB24_tf);       // bigger font
  u8g2.clearBuffer  ();    
  u8g2.setCursor    (x,y);
  u8g2.print        (value);
  u8g2.sendBuffer   ();    
}

void buttons(){  
  Serial.println ("Entering setup mode.");
  displaystring  ("setup");
  if      (digitalRead(lbutton)==1){
    while (digitalRead(lbutton)==1){delay(10);} //wait 4 release
    bool  lbut   = false;
    bool  rbut   = false;
    int  timeout = 0;
    int maxtime  = 100;
    oldthreshold = threshold;
    while (timeout<maxtime){
      int pressx = 0;
      int oldth  = threshold;
      lbut=digitalRead  (lbutton);
      rbut=digitalRead  (rbutton);
      temp = String     (threshold);
      temp += "% <>";
      displaystring        (temp);
      if (lbut)  {if(threshold> 0){threshold--;while(digitalRead(lbutton)==1){delay(10);pressx++;}}timeout=0;}      
      if (rbut)  {if(threshold<50){threshold++;while(digitalRead(rbutton)==1){delay(10);pressx++;}}timeout=0;}     
      if (oldth!=threshold){
        Serial.println       (threshold);               // update output when changed
        temp = String        (threshold);
        temp += "% <>";
        displaystring        (temp);
      }
      if (pressx>50){break;}                            // longer press
      timeout++;
      delay(50); // prox 15 seconds - if you skip this delay the processor crashes when timed out
    }
    while   (digitalRead(lbutton)==1){delay(10);}      // wait 4 release
    if (threshold!=oldthreshold){
          Serial.println  ("writing new threshold to eeprom");
          displaystring   ("store eep");
    }
  } else  {
  if    (digitalRead(rbutton)==1){
    Serial.println("right button");
    while (digitalRead(rbutton)==1){delay(10);}      // wait 4 release
    }
  }
  Serial.println ("Back to main loop.");
}
