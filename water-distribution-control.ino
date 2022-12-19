//lcd setting
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

//esp setting
#include "ThingSpeak.h"
#include "WiFiEsp.h"
#include <SoftwareSerial.h>

//define esp setting
SoftwareSerial espserial(10,11); 
char ssid[] = "zzzzz";
char pass[] = "050700aff";
const char *writeAPI = "65PYT6B1V0KZXFVU";
const char *readAPI = "M2197P516TIHKB78";
unsigned long int channelID = 1840193;

//variabel esp
int status = WL_IDLE_STATUS; 
char server[] = "api.thingspeak.com";
unsigned long waktuTerakhir = 0;
const unsigned long interval = 20000L;
WiFiEspClient client; 

//rtc
#include "RTClib.h"
RTC_DS3231 rtc;

//variabel normal puncak 
const int hourPuncak = 17;  
const int hourNormal = 20;
const int hourPuncak2 = 6;
const int hourNormal2 = 8;

//define water flow sensor
#define flowmeter1 2
#define flowmeter2 3
float debit1, debit2, banding, totalDebit;
unsigned long currentTime;
unsigned long cloopTime;

//define floatswitch 
#define floatSwitch_main 34
int flswitchMain;
#define floatSwitch_branch 35
int flswitchBranch;

//define push button
#define pb_on 6
#define pb_off 7
int onValue, offValue;

#define relay_pump 40
int pump;
#define relay_sole 41
int sole;

//define sending data
int statusCode;
int last_relay_state;

//define motor
#include <AccelStepper.h>
const int stepPinMain = 26;
const int dirPinMain = 27;
const int stepPinBranch = 22;
const int dirPinBranch = 23;
const int stepPinOutA = 39;
const int dirPinOutA = 38;
const int stepPinOutB = 29;
const int dirPinOutB = 28;
int MPP;

AccelStepper stepperMain(1, stepPinMain, dirPinMain);
AccelStepper stepperBranch(1, stepPinBranch, dirPinBranch);
AccelStepper stepperOutA(1, stepPinOutA, dirPinOutA);
AccelStepper stepperOutB(1, stepPinOutB, dirPinOutB);

//define limit switch 
#define limitSwitchMain 25
#define limitSwitchBranch 24
#define limitSwitchOutA 30
#define limitSwitchOutB 31

//include multitab
#include "homing.h"
#include "flowcount.h"
#include "floatswitch.h"
#include "waterflowsensor.h"
#include "normal_puncak.h"
#include "solenoid.h"
#include "motoroutput.h"
#include "motorbranch.h"
#include "SendingData.h"
#include "WriteData.h"

void setup() {
  Serial.begin(9600);
  //init LCD 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initialization...");
  
  //motor init
  pinMode(relay_sole, OUTPUT);
  
  pinMode(limitSwitchMain, INPUT_PULLUP);
  pinMode(stepPinMain, OUTPUT);
  pinMode(dirPinMain, OUTPUT);

  pinMode(limitSwitchBranch, INPUT_PULLUP);
  pinMode(stepPinBranch, OUTPUT);
  pinMode(dirPinBranch, OUTPUT);

  pinMode(limitSwitchOutA, INPUT_PULLUP);
  pinMode(stepPinOutA, OUTPUT);
  pinMode(dirPinOutA, OUTPUT);

  pinMode(limitSwitchOutB, INPUT_PULLUP);
  pinMode(stepPinOutB, OUTPUT);
  pinMode(dirPinOutB, OUTPUT);

  //motor init
  stepperMain.setSpeed(800);
  stepperMain.setAcceleration(800);
  stepperBranch.setSpeed(800);
  stepperBranch.setAcceleration(800);
  stepperOutA.setSpeed(800);
  stepperOutA.setAcceleration(800);
  stepperOutB.setSpeed(800);
  stepperOutB.setAcceleration(800);

  //homing
  Main();Branch();
  OutA();OutB();

  stepperBranch.moveTo(0);
  stepperOutA.moveTo(0);
  stepperOutB.moveTo(0);
  stepperBranch.runToPosition();
  stepperOutA.runToPosition();
  stepperOutB.runToPosition();
  
  lcd.setCursor(0,2);
  lcd.print("Motor Init Done!   ");
  delay(1000);
  
  //init esp
  espserial.begin(9600);
  WiFi.init(&espserial);
  
  if(WiFi.status() == WL_NO_SHIELD) {
   //Serial.println("WiFi shield not present");
   lcd.setCursor(0,1);
   lcd.print("ESP Can't Connect");
   while(true);}
    
  while(status != WL_CONNECTED) {
    //Serial.print("Attempting to connect to WPA SSID: ");
    //Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    lcd.setCursor(0,1);
    lcd.print("Try To Connect   ");
    banding; flswitchMain; pump;}
    
  //Serial.print("You're connected to the network");
  long rssi = WiFi.RSSI();

  lcd.setCursor(0,1);
  lcd.print("Connected        ");
  
  delay(100);
  ThingSpeak.begin(client);

  //done init
  lcd.setCursor(0,3);
  lcd.print("ESP Init Done!");
  delay(1000);
  lcd.clear();
  
  digitalWrite(relay_sole, HIGH);
  digitalWrite(relay_pump, HIGH);
  
  //rtc init
  rtc_setup();

  //wateflow sensor init
  pinMode(flowmeter1, INPUT);
  pinMode(flowmeter2, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowmeter1), flowsensorcount1, RISING);
  attachInterrupt(digitalPinToInterrupt(flowmeter2), flowsensorcount2, RISING);

  //floatswitch init
  fs.floatswitch_setup();
}
    
void loop() {
  lcd.backlight();
    
  //digital read
  flswitchMain = digitalRead(floatSwitch_main);
  flswitchBranch = digitalRead(floatSwitch_branch);
  pump = digitalRead(relay_pump);
  
  if(pump == 1) {
    pump = 0;}
  else if(pump == 0) {
    pump = 1;}

  //looping system
  //looping rtc
  DateTime now = rtc.now();
  
  lcd.setCursor(12,0);
  lcd.print("-");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.setCursor(16,0);
  lcd.print(now.minute(), DEC);
  delay(1000);

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(",");

  //penentuan waktu pucak dan waktu normal
  if(now.hour() > hourPuncak && now.hour() < hourNormal) 
  {
    //Serial.println("PUNCAK");
    lcd.setCursor(0,0);
    lcd.print("Waktu Puncak");

    if((debit1>2.00) && (debit1<2.50)) {
       stepperOutA.moveTo(-50);
       stepperOutA.runToPosition(); }
    if((debit2>2.00) && (debit2<2.50)) {
       stepperOutB.moveTo(-50);
       stepperOutB.runToPosition();}
       
    if(debit2>debit1) { 
      if(banding > 0.10) {
        stepperMain.moveTo(75);
        stepperMain.runToPosition();
        MPP = 75;
      
        for (MPP; banding != 0 ; MPP -= 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.print(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
     
          if(!digitalRead(limitSwitchMain)) {
            MPP = 75;}
        
          if((banding > 0) && (banding < 0.05)) {
            stepperMain.stop();
            break;}
        }}

       if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          delay(1000);}

       if((banding>0.10) && (banding<=0.20)){
          for (MPP; banding != 0 ; MPP -= 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
        if(!digitalRead(limitSwitchMain)) {
          MPP = 75;}

        if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          break;}
      }}
    }
    
    if(debit1>debit2) {
      if(banding > 0.10) {
        stepperMain.moveTo(25);
        stepperMain.runToPosition();
        MPP=25;
      
        for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          //Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
      }}

      if((banding>0) && (banding<0.10)) {
        stepperMain.stop();
        waterFlowSensor();
        CadSole();
        output1();
        output2();
        floatSwitch();
        delay(1000);}  

      if((banding>0.10) && (banding<=0.20)) {
        for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
      }}
    }   
  }
  
  else if(now.hour() > hourPuncak2 && now.hour() < hourNormal2) 
  {
    //Serial.println("PUNCAK");
    lcd.setCursor(0,0);
    lcd.print("Waktu Puncak");

    if((debit1>2.00) && (debit1<2.50)) {
      stepperOutA.moveTo(-50);
      stepperOutA.runToPosition();}
    if((debit2>2.00) && (debit2<2.50)) {
      stepperOutB.moveTo(-50);
      stepperOutB.runToPosition();}
    
    if(debit2>debit1) { 
      if(banding > 0.10) {
        stepperMain.moveTo(75);
        stepperMain.runToPosition();
        MPP = 75;
      
        for (MPP; banding != 0 ; MPP -= 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.print(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
     
          if(!digitalRead(limitSwitchMain)) {
            MPP = 75;}
        
          if((banding > 0) && (banding < 0.05)) {
            stepperMain.stop();
            break;}
        }}

       if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          delay(1000);}

       if((banding>0.10) && (banding<=0.20)){
          for (MPP; banding != 0 ; MPP -= 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
        if(!digitalRead(limitSwitchMain)) {
          MPP = 75;}

        if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          break;}
      }}
    }
    
    if(debit1>debit2) {
      if(banding > 0.10) {
        stepperMain.moveTo(25);
        stepperMain.runToPosition();
        MPP=25;
      
        for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          //Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
      }}

      if((banding>0) && (banding<0.10)) {
        stepperMain.stop();
        waterFlowSensor();
        CadSole();
        output1();
        output2();
        floatSwitch();
        delay(1000);}  

      if((banding>0.10) && (banding<=0.20)) {
        for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);*/
          waterFlowSensor();
          CadSole();
          output1();
          output2();
          floatSwitch();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
      
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
      }}
    }
  }
  else
  {
    //Serial.println("NORMAL");
    lcd.setCursor(0,0);
    lcd.print("Waktu Normal");
    
    if((debit1>1.50) && (debit1<2.00)){
      stepperOutA.moveTo(-25);
      stepperOutA.runToPosition();}
    if((debit2>1.50) && (debit2<2.00)){
      stepperOutB.moveTo(-25);
      stepperOutB.runToPosition();}
    
    if(debit2>debit1) {
      if(banding>0.10) {
        stepperMain.moveTo(25);
        stepperMain.runToPosition();
        MPP = 25;
        
        for (MPP; banding != 0 ; MPP -= 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.print(MPP);*/
          waterFlowSensor();
          CadBranch();
          output1();
          output2();
          floatSwitch2 ();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);

          if(MPP>120) {
            MPP = 25;
          }

          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding>0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
        }}

        if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          waterFlowSensor();
          CadBranch();
          output1();
          output2();
          floatSwitch2 ();
          delay(1000);}     
    
        if((banding>0.10) && (banding<=0.20)) {
          for (MPP; banding != 0 ; MPP -= 3) {
            stepperMain.moveTo(MPP);
            stepperMain.runToPosition();
            Serial.print("Posisi Motor=");
            Serial.print(",");
            Serial.println(MPP);
            waterFlowSensor();
            CadBranch();
            output1();
            output2();
            floatSwitch2 ();
            fs.floatswitch_loop();
            WriteDataToThingSpeak();  
            readDataFromThingspeak();
            delay(1000);
            
          if(MPP > 120) {
            MPP = 25;}
            
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
          }}
    }
    
    if(debit1>debit2) {
      if(banding > 0.10) {
        stepperMain.moveTo(25);
        stepperMain.runToPosition();
        MPP=25;
      
        for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.println(MPP);*/
          waterFlowSensor();
          CadBranch();
          output1();
          output2();
          floatSwitch2();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);
    
          if(MPP > 120) {
            MPP = 25;}
        
          if(!digitalRead(limitSwitchMain)) {
            MPP = 25;}

          if((banding > 0) && (banding < 0.10)) {
            stepperMain.stop();
            break;}
       }}
       
        if((banding>0) && (banding<0.10)) {
          stepperMain.stop();
          waterFlowSensor();
          CadBranch();
          output1();
          output2();
          floatSwitch2();
          delay(1000);}

        if((banding>0.10) && (banding<=0.20)) {
          for (MPP; banding != 0 ; MPP += 3) {
          stepperMain.moveTo(MPP);
          stepperMain.runToPosition();
          /*Serial.print("Posisi Motor=");
          Serial.print(",");
          Serial.print(MPP);*/
          waterFlowSensor();
          CadBranch();
          output1();
          output2();
          floatSwitch2 ();
          fs.floatswitch_loop();
          WriteDataToThingSpeak();  
          readDataFromThingspeak();
          delay(1000);

        if(MPP > 120) {
          MPP = 25;}
        
        if(!digitalRead(limitSwitchMain)) {
          MPP = 25;}

        if((banding > 0) && (banding < 0.10)) {
          stepperMain.stop();
          break;}
    }}
    }
  }
}
