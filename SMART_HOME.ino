
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <IRremoteESP8266.h>
#include <IRrecv.h>
IRrecv irrecv(D7);
decode_results results;

#define DIOD A0
#define RELE D0
#define RELE2 D3
#define RELE3 D4

#define DEBUGS 0
#define SERIAL_OUT 0 //Settings(serial out)

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4O5GbVN3qs8JJoVQapy6s5KEHRxx8CYS";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "a22home";
char pass[] = "1234561234560";
bool pinValue;
int irrecv_value = 0;

WidgetLED led1(V100);
WidgetLED led2(V101);

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(DIOD, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(RELE3, OUTPUT);
  digitalWrite(DIOD, LOW);
  digitalWrite(RELE, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  
  irrecv.enableIRIn();
  
  Blynk.begin(auth, ssid, pass);
}
  
BLYNK_WRITE (V0)
{
  if (param.asInt()== 1){                           //Checks if Virtual port V1 is active, and if it is, it will run the script
    digitalWrite(DIOD, HIGH);                          //Sets the state of port D1 to LOW
    //Blynk.virtualWrite(V0, LOW);                    //Sets Virtual port V0 to be disabled
    }
  else  if (param.asInt()== 0){                           //Checks if Virtual port V1 is active, and if it is, it will run the script
    digitalWrite(DIOD, LOW);                          //Sets the state of port D1 to LOW
   // Blynk.virtualWrite(V0, LOW);                    //Sets Virtual port V0 to be disabled
    }
}

BLYNK_WRITE (V1) //Дверь(щеколда)
{
  
  Serial.println(param.asInt());
  if (param.asInt()== 1)
  { 
  digitalWrite(RELE, LOW); 
  delay(2000);
  digitalWrite(RELE, HIGH);
  Blynk.virtualWrite(V1, LOW);
  }
}

BLYNK_WRITE (V2) //Реле2(розетка)
{
  if (param.asInt()== 1){           //Включение реле2   //Checks if Virtual port V2 is active, and if it is, it will run the script
    digitalWrite(RELE2, LOW);      //Sets the state of port D1 to LOW
    //Blynk.virtualWrite(V2, HIGH);                    //Sets Virtual port V2 to be disabled
    led1.on();
    }
  else  if (param.asInt()== 0){         //Выключение реле2 //Checks if Virtual port V2 is active, and if it is, it will run the script
    digitalWrite(RELE2, HIGH);          //Sets the state of port D1 to LOW
    //Blynk.virtualWrite(V2, LOW);                       //Sets Virtual port V2 to be disabled
    led1.off();
    }
}

BLYNK_WRITE (V3) //Реле3(розетка)
{
  if (param.asInt()== 1){           //Включение реле2   //Checks if Virtual port V2 is active, and if it is, it will run the script
    digitalWrite(RELE3, LOW);      //Sets the state of port D1 to LOW
    //Blynk.virtualWrite(V2, HIGH);                    //Sets Virtual port V2 to be disabled
    led2.on();
    }
  else  if (param.asInt()== 0){         //Выключение реле2 //Checks if Virtual port V2 is active, and if it is, it will run the script
    digitalWrite(RELE3, HIGH);          //Sets the state of port D1 to LOW
    //Blynk.virtualWrite(V2, LOW);                       //Sets Virtual port V2 to be disabled
    led2.off();
    }
}

/*BLYNK_WRITE (V4) //Шаговый двигатель
{
  StepperDirection = param.asInt();
  StepperDirectionChanged = true;
  //Serial.print("Stepper = ");
  //Serial.println(StepperDirection);
}*/
  
void loop()
{
  if(millis()/300) 
      {Blynk.run();}
}
