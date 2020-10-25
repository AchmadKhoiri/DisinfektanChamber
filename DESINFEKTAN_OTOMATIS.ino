/*
 * Coding desinfektan otomatis arduino uno dengan settingan jarak sensor dan delay ON pompa desinfektan
 * Terima jasa custom project arduino, aplikasi android, Internet of Things (IoT) hub nomor pada deskripsi video
 * Chanel youtube : Guntur Agusta
 * 
 * 
 * Download skema rangkaian desinfektan otomatis >>> https://github.com/guntur1997/DisinfektanChamber
 */


#define pinMenu        A0
#define pinPlus        A1
#define pinMinus       A2
#define pinOk          A3
#define pompa          2

const int pingPin = 11;
const int echoPin = 10;
 
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //ganti dengan alamat i2c kalian, biasanya 0x27 atau 0x3F

char Menu[][17] = {
  "KEMBALI         ",
  "SET JARAK SENSOR",
  "SET DELAY POMPA ",
  "SEMPROT MANUAL  ",
  
};

int setDelay ;
int setJarak ;
char buff[40];

byte menuLevel = 0;
byte menuIndex[2];


void setup() {
  pinMode(pinMenu, INPUT_PULLUP);
  pinMode(pinPlus, INPUT_PULLUP);
  pinMode(pinMinus, INPUT_PULLUP);
  pinMode(pinOk, INPUT_PULLUP);
  pinMode(pompa, OUTPUT);
  digitalWrite(pompa, HIGH);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("AUTO DESINFEKTAN");
  lcd.setCursor(0, 1);
  lcd.print("CHAMBER ver.1.0 ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   system by :  ");
  lcd.setCursor(0, 1);
  lcd.print("KaryatronikaBali");
  delay(2000);
  lcd.clear();
  delay(50);
}

void loop() {
  
  if (menuLevel == 0){  //.............................TAMPILAN HOME
    
    setJarak = EEPROM.read(0);
    setDelay = EEPROM.read(1);
    
    long duration, jarak;
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);

    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    jarak = microsecondsToCentimeters(duration);

    if (jarak <= setJarak) {
        digitalWrite(pompa, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DESINFEKTAN ON");
        lcd.setCursor(0, 1);
        lcd.print("HARAP TUNGGU");
        delay(setDelay * 1000);
        lcd.clear();
        digitalWrite(pompa, HIGH);
    }
    else {
        digitalWrite(pompa, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("AUTO DESINFEKTAN");
        lcd.setCursor(0, 1);
        lcd.print("CHAMBER ver.1.0 ");

    }
  }



 if (!digitalRead(pinMenu)) //.............................tombol MENU
  {
    delay(100);
    if (!digitalRead(pinMenu))
    {
      if (menuLevel == 0)
      {
        menuLevel = 1;
        menuIndex[0] = 0;
      }
      else
      {
        menuLevel = 0;
        lcd.noBlink();
        lcd.clear();
        delay(50);
      }
    displayMenu();
    }
  }

 if (!digitalRead(pinPlus)) //.............................tombol PLUS
  {
    delay(100);
    if (!digitalRead(pinPlus))
    {
      lcd.noBlink();
      if (menuLevel == 1)
      {
        menuIndex[0]++;
        if (menuIndex[0] >= sizeof(Menu) / sizeof(Menu[0]))
        {
          menuIndex[0] = 0;
        }
      displayMenu();
      }
      if (menuLevel == 2)
      {
        if (menuIndex[0] == 1)
          {
             setJarak++;
             if (setJarak > 100){
              setJarak = 10;
              }
          }
        else if (menuIndex[0] == 2)
          {
             setDelay++;
             if (setDelay > 30){
              setDelay = 3;
              }
          }
       displayMenu();
      }
      delay(100);
      }
    }

 if (!digitalRead(pinMinus)) //.............................tombol MIN
  {
    delay(100);
    if (!digitalRead(pinMinus))
    {
      lcd.noBlink();
      if (menuLevel == 1)
      {
        if (menuIndex[0] == 0)
        {
          menuIndex[0] = sizeof(Menu) / sizeof(Menu[0]) - 1;
        }
        else
        {
          menuIndex[0]--;
        }
        displayMenu();
      }
      if (menuLevel == 2)
      {
        if (menuIndex[0] == 1)
          {
             if (setJarak == 10){ //disini sy set jarak minimal 10 cm, silahkan diganti sesuai keinginan
              setJarak = 100;     //disini sy set jarak maksimal 100 cm, silahkan diganti sesuai keinginan
              }
              else {
                setJarak--;
              }
          }
        else if (menuIndex[0] == 2)
          {
             if (setDelay == 3){  //disini sy set delay minimal 3 second, silahkan diganti sesuai keinginan
              setDelay = 30;      //disini sy set delay maksimal 30 second, silahkan diganti sesuai keinginan
              }
              else {
                setDelay--;
              }
          }
        displayMenu();
      }
      delay(100);
      }
    }

  if (!digitalRead(pinOk))  //.............................tombol OK
  {
    delay(100);
    if (!digitalRead(pinOk))
    {
      if (menuLevel == 1)
      {
        menuLevel = 2;
        //menuIndex[1] = 0;
        
        if (menuIndex[0] == 0) {
          menuLevel = 0;
        }
        else if (menuIndex[0] == 1)
         {
          lcd.clear();
          delay(50);
          displayMenu();
          lcd.setCursor(0, 1);
          lcd.blink();
         }
        else if (menuIndex[0] == 2)
         {
          lcd.clear();
          delay(50);
          displayMenu();
          lcd.setCursor(0, 1);
          lcd.blink();
         }
         else if (menuIndex[0] == 3)
         {
          lcd.clear();
          delay(50);
          setDelay = EEPROM.read(1);
          lcd.setCursor(0, 0);
          lcd.print("SEMPROT MANUAL");
          lcd.setCursor(0, 1);
          lcd.print("DESINFEKTAN");
          digitalWrite(pompa, LOW);
          delay(setDelay * 1000);
          menuLevel = 0;
          lcd.clear();
         }
       }

       else if (menuLevel == 2)
      {
        if (menuIndex[0] == 1)
        {
          menuLevel = 1;
          EEPROM.write(0, setJarak);
          displayMenu();
          lcd.noBlink();
        }
        else if (menuIndex[0] == 2)
        {
          menuLevel = 1;
          EEPROM.write(1, setDelay);
          displayMenu();
          lcd.noBlink();
        }
        
      }
      while (!digitalRead(pinOk));
        delay(500);
      }
    }
}
void displayMenu()  
{
  if (menuLevel == 1)
  {
    lcd.clear();
    //delay(50);
    lcd.setCursor(0, 0);
    lcd.print("**MENU SETTING**");
    lcd.setCursor(0, 1);
    lcd.print(Menu[menuIndex[0]]);
  }
  else if (menuLevel == 2)
  {
    if (menuIndex[0] == 1)
    {
      lcd.clear();
      //delay(50);
      lcd.setCursor(0, 0);
      lcd.print(Menu[menuIndex[0]]);
      sprintf(buff, "JARAK : %02d cm", setJarak);
      lcd.setCursor(0, 1);
      lcd.print(buff);
    }
    else if (menuIndex[0] == 2)
    {
      lcd.clear();
      //delay(50);
      lcd.setCursor(0, 0);
      lcd.print(Menu[menuIndex[0]]);
      sprintf(buff, "DELAY : %02d sc", setDelay);
      lcd.setCursor(0, 1);
      lcd.print(buff);
    } 
  }
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
