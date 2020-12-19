
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
//16752735 - вверх
//16720095 - вниз
//16769055 - стоп
#define UP_code 16752735
#define DOWN_code 16720095
#define STOP_code 16769055

#include <CustomStepper.h>            // Подключаем библиотеку управления шаговым двигателем. По умолчанию настроена на двигатель 28BYJ-48-5V
CustomStepper stepper(D1, D2, D5, D6);  // Указываем пины, к которым подключен драйвер шагового двигателя
bool clickedUP = false;
bool clickedDOWN = false;
bool clickedSTOP = false;
bool isInMovement = false;
bool isAtMinimalPoint = false;
bool isAtMaximalPoint = false;
#define up_stopper 3
#define down_stopper D4

#define DIOD A0
#define RELE D0
#define RELE2 D3

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
byte StepperDirection = 0;
bool StepperDirectionChanged = false;
int irrecv_value = 0;

WidgetLED led1(V100);

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(DIOD, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(RELE2, OUTPUT);
  digitalWrite(DIOD, LOW);
  digitalWrite(RELE, HIGH);
  digitalWrite(RELE2, HIGH);
  
  stepper.setRPM(12);                 // Устанавливаем кол-во оборотов в минуту
  stepper.setSPR(4075.7728395);       // Устанавливаем кол-во шагов на полный оборот. Максимальное значение 4075.7728395
  pinMode(up_stopper,INPUT_PULLUP);
  pinMode(down_stopper,INPUT_PULLUP);

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

BLYNK_WRITE (V2) //Реле(розетка)
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

BLYNK_WRITE (V4) //Шаговый двигатель
{
  StepperDirection = param.asInt();
  StepperDirectionChanged = true;
  //Serial.print("Stepper = ");
  //Serial.println(StepperDirection);
}
  
void loop()
{
  if(millis()/300) 
      {Blynk.run();}
     
  if(irrecv.decode(&results) or StepperDirectionChanged)
  {
    if(results.value)
      irrecv_value = results.value;
      else irrecv_value = 0;
      
    if(irrecv_value == 16769055 or (StepperDirectionChanged and StepperDirection == 0)) 
    {
      if(SERIAL_OUT)
        if(irrecv_value == 16769055)
        {
          Serial.print("IR recieved: ");
          Serial.println(STOP_code);
        }
        else if(StepperDirectionChanged) {Serial.println("StepperDirectionChanged");}
      
      clickedUP = false;
      clickedDOWN = false;
      clickedSTOP = true;
      Blynk.virtualWrite(V4, 0);
    }
    else if(irrecv_value == 16752735 or (StepperDirectionChanged and StepperDirection == 1)) 
    {
      if(SERIAL_OUT)
        if(irrecv_value == 16752735)
        {
          Serial.print("IR recieved: ");
          Serial.println(UP_code);
        }
        else if(StepperDirectionChanged) {Serial.println("StepperDirectionChanged");}
        
      clickedUP = true;
      clickedDOWN = false;
      clickedSTOP = false;
      Blynk.virtualWrite(V4, 1);
    }
    else if(irrecv_value == 16720095 or (StepperDirectionChanged and StepperDirection == 2)) 
    {
      if(SERIAL_OUT)
        if(irrecv_value == 16720095)
        {
          Serial.print("IR recieved: ");
          Serial.println(DOWN_code);
        }
        else if(StepperDirectionChanged) {Serial.println("StepperDirectionChanged");}
        
      clickedUP = false;
      clickedDOWN = true;
      clickedSTOP = false;
      Blynk.virtualWrite(V4, 2);
    }

    if(irrecv_value)
      irrecv.resume();
    
    if(StepperDirectionChanged) 
      StepperDirectionChanged = false;
  
  }
  
  if(clickedUP and !GetUpStopper() and !isAtMaximalPoint)
    {
      SetUP();  
       while(!(stepper.isDone())) {stepper.run(); }    
            
      if(isAtMinimalPoint) isAtMinimalPoint = false;
      if(DEBUGS)
        Serial.println(GetUpStopper());
    }
    else if(clickedDOWN and !GetDownStopper() and !isAtMinimalPoint)
    {
      SetDOWN();
       while(!(stepper.isDone())) {stepper.run(); }      
      if(isAtMaximalPoint) isAtMaximalPoint = false;
       if(DEBUGS)
        Serial.println(GetDownStopper());
    }
    else if(clickedSTOP )
    {
      SetSTOP();
      while(!(stepper.isDone())) {stepper.run(); }       
      //delay(50);
      if(SERIAL_OUT)
        Serial.println("ClickedSTOP");
      isInMovement = false;
    }
    
    if((clickedDOWN and GetDownStopper()) or (clickedUP and GetUpStopper())) //Если спускается и дошла до низа, или поднимается и дошла до верха
    {
      if(GetDownStopper())
        isAtMinimalPoint = true;
      else if(GetUpStopper())
        isAtMaximalPoint = true;
      clickedUP = false;
      clickedDOWN = false;
      clickedSTOP = true;
      SetSTOP();
       while(!(stepper.isDone())) {stepper.run(); }   
      isInMovement = false;
      Blynk.virtualWrite(V4, 0);
    }
}


void SetDOWN()
{
  isInMovement = true;
  if (stepper.isDone())
    {
      stepper.setDirection(CCW);
      stepper.rotateDegrees(1); 
         // Будет вращать вниз пока не получит команду о смене направления или пока не получит директиву STOP 
         
    }
  if(SERIAL_OUT)
    Serial.println("DOWN");
}
void SetUP()
{
  isInMovement = true;
  if (stepper.isDone())
      {
        stepper.setDirection(CW);
        stepper.rotateDegrees(1); 
           // Будет вращать вверх пока не получит команду о смене направления или пока не получит директиву STOP    
           // stepper.run();
      }
  if(SERIAL_OUT)
    Serial.println("UP");
}
void SetSTOP()
{
  isInMovement = false;
  if(stepper.isDone())
    {
      stepper.setDirection(STOP);
      stepper.rotate(); 
    }
  if(SERIAL_OUT)
    Serial.println("STOP");
}
bool GetUpStopper()
{
  return !digitalRead(up_stopper);
}
bool GetDownStopper()
{
  return !digitalRead(down_stopper);
}
