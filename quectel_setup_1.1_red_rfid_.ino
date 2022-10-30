#include <SoftwareSerial.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include <NfcAdapter.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//Create software serial object to communicate with SIM800L
PN532_SPI interface(SPI, 3); // create a PN532 SPI interface with the SPI CS terminal located at digital pin 10
NfcAdapter nfc = NfcAdapter(interface);
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial mySerial(5, 4); //SIM800L Tx & Rx is connected to Arduino #3 & #2

#define boadquic 4800
#define boadard 4800
#define buzzerPin 7
#define terminalID "164522982240839"
#define relayPin 8

unsigned long Time;
String contentU;
String content;

void setup()
{

 lcd.begin();
 
 setupModule();
 lcd.print("    READY -_-");  
   
}

void loop()
{
  

  if(millis() - Time >= 60000)
  {
    setupModule();
    Time =  millis();
    }
   
      if (! nfc.tagPresent())
 {
   return;
 
 }
    NfcTag tag = nfc.read();
    content = tag.getUidString();
    content.replace(" ", "");
    content.toLowerCase();
    Serial.println(content);
 
  lcd.clear();
  lcd.print("  Please Wait!");

  tone(buzzerPin, 3000);
  delay(200);
  noTone(buzzerPin);
  String Payload = "{\"terminalID\":\""terminalID"\",\"cardID\":\"" + content + "\"}";
  int Length = Payload.length();
  Serial.println(Payload);
   String dataSizeTimeOut = String("at+qhttppost=")+Length+String(",80,80");
    mySerial.println("AT+QHTTPCFG=\"contextid\",1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("AT+QHTTPCFG=\"sslctxid\",1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("AT+QSSLCFG=\"sslversion\",1,1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("AT+QHTTPCFG=\"contenttype\",1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("at+qhttpurl=57,80"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("https://us-central1-lift-os.cloudfunctions.net/payForRide"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println(dataSizeTimeOut); //Check whether it has registered in the network
     updateSerial();
     Time =  millis();
     while (mySerial.available() == 0)
    { 
      
    Serial.println("Wait for connection");
    
    if (millis() - Time > 20000)
      {
        Serial.println ("Connention Error");
        lcd.clear();
        lcd.print("(CE) CHECK SIGNAL"); //NO CONNECTION
        delay(2000);
        lcd.clear();
        lcd.print("    READY -_-");
        return;
      }
    
    }
    mySerial.println(Payload);
   Time = millis();
  while(millis() - Time <= 30000)
  {
    if(millis() - Time >=20000){
      Serial.print("Error Time");
      lcd.clear();
      lcd.print("(NR) FROM SERVER"); //NR NO RESPONSE
      delay(2000);
      lcd.clear();
      lcd.print("    READY -_-");
      return;
      }
    while(mySerial.available())
    {
      int Parse = mySerial.parseInt();
      Serial.println(Parse);
      if (Parse == 201)
        {
          Serial.println("good");
          digitalWrite(relayPin, 0);
          
          
          tone(buzzerPin, 3000); // Send 1KHz sound signal...
          delay(100);        // ...for 1 sec
          noTone(buzzerPin);     // Stop sound...
          delay(100);        // ...for 1sec
          tone(buzzerPin, 3000);
          delay(100); 
          noTone(buzzerPin); 
          delay(100); 
          tone(buzzerPin, 3000);
          delay(100);
          noTone(buzzerPin);
          delay(500);
          digitalWrite(relayPin, 1);
          mySerial.println("AT+QHTTPREAD=80"); //Check whether it has registered in the network
          Time = millis();
         delay(50);
            while(mySerial.available())
            {
            String Amount = mySerial.readString();
           int End = Amount.indexOf('\n');
         
         String s = Amount.substring(End, 100);
         int End1 = s.indexOf(";");
         int End2 = End1 + 1;
         String s1 = s.substring(End2, 100);
         int End3 = s1.indexOf(";");
         String s2 = s1.substring(0, End3);
              
              
              Serial.println(s2);
            
              lcd.clear();
              lcd.print(s2);
              delay(3000);
              lcd.clear();
              lcd.print("    READY -_-");
              return;
            }
          
          
          
          return;          
        }

       if(Parse > 201 && Parse < 300)
       {
      
       Serial.println("Response Error"); 
       lcd.backlight();
       lcd.clear();
       lcd.setCursor(0,0);
        lcd.print("   CARD ERROR");
       tone(buzzerPin, 2800);
       delay(1000); 
       noTone(buzzerPin);
       
       delay(2000);
       lcd.clear();
       lcd.print("    READY -_-");
       return;
       
       
        } 
    }
    
  }
   
}

void updateSerial()
{
  delay(100);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void setupModule()
{
  pinMode(buzzerPin, OUTPUT); 
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 1);


  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  nfc.begin();
  Serial.begin(boadard);
  Serial.setTimeout(5);
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(boadquic);
  mySerial.setTimeout(5);

  
  Serial.println("Initializing...");
  delay(500);

mySerial.println("at+csq?"); //Check whether it has registered in the network
    updateSerial();
    mySerial.println("at+csq"); //Check whether it has registered in the network
    updateSerial();
   mySerial.println("at+cpin?"); //Check whether it has registered in the network
    updateSerial();
    mySerial.println("at+qiact?"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("AT+QHTTPCFG=\"contextid\",1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("AT+QHTTPCFG=\"sslctxid\",1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("at+QSSLCFG=\"sslversion\",1,1"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("at+QHTTPCFG=\"contenttype\",1"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("at+IPR?"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("at+qhttpurl=57,80"); //Check whether it has registered in the network
  updateSerial();
    mySerial.println("https://us-central1-lift-os.cloudfunctions.net/payForRide"); //Check whether it has registered in the network
  updateSerial();
 
  
  }
