
/*
DS3231_set.pde
Eric Ayars
4/11

Test of set-time routines for a DS3231 RTC

*/

#include <Wire.h>
#include <DS3231.h>
#include <avr/sleep.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define ALARMS_COUNT 5

DS3231 clk;
RTCDateTime dt;
SoftwareSerial mySerial(2, 7);

bool isAlarm = false;
byte openAngle = 100;
String alarms[5] = {"07:00","0","0","0","0"};
byte currentAlarmIndex = -1;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  clk.begin();
  clk.enableOutput(false);
  clearRTCalarms();
  setInterruptionMode();
  setNextAlarm();
  setInterrupt();
}

void clearRTCalarms() {
    clk.armAlarm1(false);
    clk.clearAlarm1();
}

void setInterruptionMode() {
  Wire.beginTranswermission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00000111);
  Wire.endTranswermission();
}

String getalarms() {
    String res = "";
    RTCAlarmTime at = clk.getAlarm1();
    String a = clk.dateFormat("H:i", at);
    int i = 0;
    while((i < ALARMS_COUNT) && (alarms[i] != "0")){
      if(i>0){
        res += "\r\n";
      }
      if(alarms[i] == a){
        res += "** ";
      }
      res += i;
      res += " - " + alarms[i];
      i++;
    }
    return res;
}

bool addAlarm(String ti) {
    if(alarms[ALARMS_COUNT-1] != "0"){
      return false;
    }
    byte i = 0;
    bool res = false;
    bool needSetNAl = false;
      for (i=0; i < ALARMS_COUNT; i++) {
        String curT = getTime(true);
        if(alarms[i] == "0"){
          alarms[i] = ti;
          res = true;
          needSetNAl = i == 0;  
          if(i > 0
            && strcmp(curT.c_str(), alarms[i].c_str()) < 0
            && strcmp(curT.c_str(), alarms[i-1].c_str()) > 0) {
              needSetNAl = true;
            }
          break;
        }
        //if new alarm is earlier then current
        if(strcmp(ti.c_str(), alarms[i].c_str()) < 0){
          String t = alarms[i];
          alarms[i] = ti;
          shiftArrayToRight(alarms, ALARMS_COUNT, i+1);
          alarms[i+1] = t;
          //if alarm was set to current alarm
          if(currentAlarmIndex == i){
            //check if current time is before new alarm
            if((strcmp(curT.c_str(), alarms[i].c_str()) < 0)
              || (strcmp(curT.c_str(), alarms[i+1].c_str()) > 0)) {
              currentAlarmIndex-=1;
              needSetNAl = true;
            } else {
              currentAlarmIndex+=1;
              needSetNAl = true;
            }
          }
          res = true;
          break;
        }
      }

    if(needSetNAl){
        setNextAlarm();
    }
    return res;
}

void shiftArrayToRight(String *arr, int len, int start) {
  for(int j = len-1; j > start; j--){
    arr[j] = arr[j-1];
  }
}

bool deleteAlarm(int p) {
    if(p < 0 || p > ALARMS_COUNT-1){
      return false;
    }
    byte i = 0;
    for (i=p; i < ALARMS_COUNT-1; i++) {
        alarms[i] = alarms[i+1];
    }
    alarms[ALARMS_COUNT-1] = "0";
    if(p == currentAlarmIndex){
      if((p == 0) && (alarms[0] == "0")) {
        clk.clearAlarm1();
        currentAlarmIndex = -1;
      } else {
        currentAlarmIndex-= 1;
        setNextAlarm();
      }
    } else if(p < currentAlarmIndex) {
      currentAlarmIndex-= 1;
    }
    return true;
}

String getP(){
  return String(openAngle);
}

bool setP(String s){
  openAngle = atoi(s.c_str());
  return true;
}

String getTime(bool sh) {
  dt = clk.getDateTime();
  return clk.dateFormat(sh ? "H:i" : "d-m-Y H:i:s",  dt);
}

bool setTime(String dt) {
  int d[5];
  parseTime(dt, d, 5);
  clk.setDateTime(d[0], d[1], d[2], d[3], d[4], 0);
  return true; 
}

void setNextAlarm() {
    int nextAlarmIndex = currentAlarmIndex+1;
    if(nextAlarmIndex > ALARMS_COUNT-1) {
        nextAlarmIndex = 0;
    }
    if(alarms[nextAlarmIndex] != "0") {
        currentAlarmIndex = nextAlarmIndex;
        int time[2];
        parseTime(alarms[currentAlarmIndex], time, 2);
        // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
        clk.setAlarm1(0, time[0], time[1], 0, DS3231_MATCH_H_M_S);
    } else {
        currentAlarmIndex = -1;
        if(alarms[0] != "0") {
          setNextAlarm();
        }
    }
}

bool setActiveAlarm(int index) {
  if((index < 0) || (index > (ALARMS_COUNT - 1)) || (alarms[index] == "0"))  {
    return false;
  }
  currentAlarmIndex = index - 1;
  setNextAlarm();
  return true;
}

void splitString(String input, char delimeter, String *result) {
    int li = 0;
    int c = 0;
   for (int i = 0; i < input.length(); i++) {
        if (input.charAt(i) == delimeter) {
          result[c] = input.substring(li, i);
          li = i + 1;
          c++;
        }

        // If we're at the end of the string (no more commas to stop us)
        if (i == input.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          result[c] = input.substring(li, i+1);
        }
      }
}

void parseTime(String time2, int *result, int parts){
  String res[5];
  splitString(time2, ':', res);
  
  for(int i = 0; i < parts; i++){
    result[i] = atoi(res[i].c_str());
  }
}

void parseMessage(String message, String *res){
  splitString(message, '-', res);
}

void feed() {
    Servo servo;
    servo.attach(5);
    servo.write(openAngle);
    delay(1000);
    servo.write(0);
    delay(1000);
    servo.detach();
}

void setInterrupt()         // here we put the arduino to sleep
{
    delay(500);
    attachInterrupt(digitalPinToInterrupt(3),wakeUpAlarm, FALLING); // use interrupt 0 (pin 2) and run function                            
    delay(500);                         
}

void wakeUpAlarm()        // here the interrupt is handled after wakeup
{
  Serial.println("wake");
  isAlarm = true;
}

void answer(String d, bool softSer) {
  if(softSer) {
    mySerial.println(d + "\r\n\r\n");
  } else {
    Serial.println(d + "\r\n\r\n");
  }
}

void loop()
{
  if(isAlarm){
    clk.clearAlarm1();
    isAlarm = false;
    feed();
    setNextAlarm();
  } else {
   String mes = "";
   bool softwareSerial = false;
   if (Serial.available()) {
      mes = Serial.readString();
   }
   if (mySerial.available()) {
      mes = mySerial.readString();
      softwareSerial = true;
   }
   if (mes != "")
     {
       String d[2];
       parseMessage(mes,d);
       String com = d[0];
       String f = "f";
       String s = "t";
       if((com == "wake") || (com == "ake")){
          answer(s, softwareSerial);
       } else if(com == "sT"){
          answer(setTime(d[1]) ? s : f, softwareSerial);
       } else if(com == "T"){
          answer(getTime(false), softwareSerial);
       } else if(com == "adA"){
          answer(addAlarm(d[1]) ? s : f, softwareSerial);
       } else if(com == "dA"){
          answer(deleteAlarm(atoi(d[1].c_str())) ? s : f, softwareSerial);
       } else if(com == "as"){
          answer(getalarms(), softwareSerial);
       } else if(com == "f"){
          feed();
          answer(s, softwareSerial);
       } else if(com == "sAA"){
          answer(setActiveAlarm(atoi(d[1].c_str())) ? s : f, softwareSerial);
       } else if(com == "P") {
          answer(getP(), softwareSerial);
       } else if(com == "sP") {
          answer(setP(d[1]) ? s : f, softwareSerial);
       } else {
          answer("", softwareSerial);
       }
     }
  }
}
/*
DS3231_set.pde
Eric Ayars
4/11

Test of set-time routines for a DS3231 RTC

*/

#include <Wire.h>
#include <DS3231.h>
#include <avr/sleep.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define ALARMS_COUNT 5

DS3231 clk;
RTCDateTime dt;
SoftwareSerial mySerial(2, 7);

bool isAlarm = false;
byte openAngle = 100;
String alarms[5] = {"07:00","0","0","0","0"};
byte currentAlarmIndex = -1;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  clk.begin();
  clk.enableOutput(false);
  clearRTCalarms();
  setInterruptionMode();
  setNextAlarm();
  setInterrupt();
}

void clearRTCalarms() {
    clk.armAlarm1(false);
    clk.clearAlarm1();
}

void setInterruptionMode() {
  Wire.beginTranswermission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00000111);
  Wire.endTranswermission();
}

String getalarms() {
    String res = "";
    RTCAlarmTime at = clk.getAlarm1();
    String a = clk.dateFormat("H:i", at);
    int i = 0;
    while((i < ALARMS_COUNT) && (alarms[i] != "0")){
      if(i>0){
        res += "\r\n";
      }
      if(alarms[i] == a){
        res += "** ";
      }
      res += i;
      res += " - " + alarms[i];
      i++;
    }
    return res;
}

bool addAlarm(String ti) {
    if(alarms[ALARMS_COUNT-1] != "0"){
      return false;
    }
    byte i = 0;
    bool res = false;
    bool needSetNAl = false;
      for (i=0; i < ALARMS_COUNT; i++) {
        String curT = getTime(true);
        if(alarms[i] == "0"){
          alarms[i] = ti;
          res = true;
          needSetNAl = i == 0;  
          if(i > 0
            && strcmp(curT.c_str(), alarms[i].c_str()) < 0
            && strcmp(curT.c_str(), alarms[i-1].c_str()) > 0) {
              needSetNAl = true;
            }
          break;
        }
        //if new alarm is earlier then current
        if(strcmp(ti.c_str(), alarms[i].c_str()) < 0){
          String t = alarms[i];
          alarms[i] = ti;
          shiftArrayToRight(alarms, ALARMS_COUNT, i+1);
          alarms[i+1] = t;
          //if alarm was set to current alarm
          if(currentAlarmIndex == i){
            //check if current time is before new alarm
            if((strcmp(curT.c_str(), alarms[i].c_str()) < 0)
              || (strcmp(curT.c_str(), alarms[i+1].c_str()) > 0)) {
              currentAlarmIndex-=1;
              needSetNAl = true;
            } else {
              currentAlarmIndex+=1;
              needSetNAl = true;
            }
          }
          res = true;
          break;
        }
      }

    if(needSetNAl){
        setNextAlarm();
    }
    return res;
}

void shiftArrayToRight(String *arr, int len, int start) {
  for(int j = len-1; j > start; j--){
    arr[j] = arr[j-1];
  }
}

bool deleteAlarm(int p) {
    if(p < 0 || p > ALARMS_COUNT-1){
      return false;
    }
    byte i = 0;
    for (i=p; i < ALARMS_COUNT-1; i++) {
        alarms[i] = alarms[i+1];
    }
    alarms[ALARMS_COUNT-1] = "0";
    if(p == currentAlarmIndex){
      if((p == 0) && (alarms[0] == "0")) {
        clk.clearAlarm1();
        currentAlarmIndex = -1;
      } else {
        currentAlarmIndex-= 1;
        setNextAlarm();
      }
    } else if(p < currentAlarmIndex) {
      currentAlarmIndex-= 1;
    }
    return true;
}

String getP(){
  return String(openAngle);
}

bool setP(String s){
  openAngle = atoi(s.c_str());
  return true;
}

String getTime(bool sh) {
  dt = clk.getDateTime();
  return clk.dateFormat(sh ? "H:i" : "d-m-Y H:i:s",  dt);
}

bool setTime(String dt) {
  int d[5];
  parseTime(dt, d, 5);
  clk.setDateTime(d[0], d[1], d[2], d[3], d[4], 0);
  return true; 
}

void setNextAlarm() {
    int nextAlarmIndex = currentAlarmIndex+1;
    if(nextAlarmIndex > ALARMS_COUNT-1) {
        nextAlarmIndex = 0;
    }
    if(alarms[nextAlarmIndex] != "0") {
        currentAlarmIndex = nextAlarmIndex;
        int time[2];
        parseTime(alarms[currentAlarmIndex], time, 2);
        // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
        clk.setAlarm1(0, time[0], time[1], 0, DS3231_MATCH_H_M_S);
    } else {
        currentAlarmIndex = -1;
        if(alarms[0] != "0") {
          setNextAlarm();
        }
    }
}

bool setActiveAlarm(int index) {
  if((index < 0) || (index > (ALARMS_COUNT - 1)) || (alarms[index] == "0"))  {
    return false;
  }
  currentAlarmIndex = index - 1;
  setNextAlarm();
  return true;
}

void splitString(String input, char delimeter, String *result) {
    int li = 0;
    int c = 0;
   for (int i = 0; i < input.length(); i++) {
        if (input.charAt(i) == delimeter) {
          result[c] = input.substring(li, i);
          li = i + 1;
          c++;
        }

        // If we're at the end of the string (no more commas to stop us)
        if (i == input.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          result[c] = input.substring(li, i+1);
        }
      }
}

void parseTime(String time2, int *result, int parts){
  String res[5];
  splitString(time2, ':', res);
  
  for(int i = 0; i < parts; i++){
    result[i] = atoi(res[i].c_str());
  }
}

void parseMessage(String message, String *res){
  splitString(message, '-', res);
}

void feed() {
    Servo servo;
    servo.attach(5);
    servo.write(openAngle);
    delay(1000);
    servo.write(0);
    delay(1000);
    servo.detach();
}

void setInterrupt()         // here we put the arduino to sleep
{
    delay(500);
    attachInterrupt(digitalPinToInterrupt(3),wakeUpAlarm, FALLING); // use interrupt 0 (pin 2) and run function                            
    delay(500);                         
}

void wakeUpAlarm()        // here the interrupt is handled after wakeup
{
  Serial.println("wake");
  isAlarm = true;
}

void answer(String d, bool softSer) {
  if(softSer) {
    mySerial.println(d + "\r\n\r\n");
  } else {
    Serial.println(d + "\r\n\r\n");
  }
}

void loop()
{
  if(isAlarm){
    clk.clearAlarm1();
    isAlarm = false;
    feed();
    setNextAlarm();
  } else {
   String message = "";
   bool softwareSerial = false;
   if (Serial.available()) {
      message = Serial.readString();
   }
   if (mySerial.available()) {
      message = mySerial.readString();
      softwareSerial = true;
   }
   if (message != "")
     {
       String d[2];
       parseMessage(message,d);
       String com = d[0];
       String f = "f";
       String s = "t";
       if((com == "wake") || (com == "ake")){
          answer(s, softwareSerial);
       } else if(com == "sT"){
          answer(setTime(d[1]) ? s : f, softwareSerial);
       } else if(com == "T"){
          answer(getTime(false), softwareSerial);
       } else if(com == "adA"){
          answer(addAlarm(d[1]) ? s : f, softwareSerial);
       } else if(com == "dA"){
          answer(deleteAlarm(atoi(d[1].c_str())) ? s : f, softwareSerial);
       } else if(com == "as"){
          answer(getalarms(), softwareSerial);
       } else if(com == "f"){
          feed();
          answer(s, softwareSerial);
       } else if(com == "sAA"){
          answer(setActiveAlarm(atoi(d[1].c_str())) ? s : f, softwareSerial);
       } else if(com == "P") {
          answer(getP(), softwareSerial);
       } else if(com == "sP") {
          answer(setP(d[1]) ? s : f, softwareSerial);
       } else {
          answer("", softwareSerial);
       }
     }
  }
}
