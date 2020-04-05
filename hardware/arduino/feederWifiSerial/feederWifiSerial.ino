
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

#define ALC 5

DS3231 clk;
RTCDateTime dt;
SoftwareSerial mySer(2, 6);

bool iA = false;
byte opAn = 100;
String als[5] = {"04:00","0","0","0","0"};
byte curAlInd = -1;

void setup()
{
  Serial.begin(9600);
  mySer.begin(9600);
  clk.begin();
  clearRTCals();
  setIntMode();
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clk.setDateTime(2020, 3, 3, 20, 00, 0);
  setNAl();
  setInterrupt();
  // Attach Interrput 0. In Arduino UNO connect DS3231 INT to Arduino Pin 2
}

void clearRTCals() {
    clk.armAlarm1(false);
    clk.clearAlarm1();
}

void setIntMode() {
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00000111);
  Wire.endTransmission();
}

String getals() {
    String res = "";
    RTCAlarmTime at = clk.getAlarm1();
    String a = clk.dateFormat("H:i", at);
    int i = 0;
    while((i < ALC) && (als[i] != "0")){
      if(als[i] == a){
        res += "** ";
      }
      res += i;
      res += " - " + als[i] + "\r\n";
      i++;
    }
    return res;
}

bool addAlarm(String ti) {
    if(als[ALC-1] != "0"){
      return false;
    }
    byte i = 0;
    bool res = false;
    bool needSetNAl = false;
      for (i=0; i < ALC; i++) {
        String curT = getTime(true);
        if(als[i] == "0"){
          als[i] = ti;
          res = true;
          needSetNAl = i == 0;  
          if(i > 0
            && strcmp(curT.c_str(), als[i].c_str()) < 0
            && strcmp(curT.c_str(), als[i-1].c_str()) > 0) {
              needSetNAl = true;
            }
          break;
        }
        //if new alarm is earlier then current
        if(strcmp(ti.c_str(), als[i].c_str()) < 0){
          String t = als[i];
          als[i] = ti;
          shiftArrayToRight(als, ALC, i+1);
          als[i+1] = t;
          //if alarm was set to current alarm
          if(curAlInd == i){
            //check if current time is before new alarm
            if((strcmp(curT.c_str(), als[i].c_str()) < 0)
              || (strcmp(curT.c_str(), als[i+1].c_str()) > 0)) {
              curAlInd-=1;
              needSetNAl = true;
            } else {
              curAlInd+=1;
            }
          }
          res = true;
          break;
        }
      }

    if(needSetNAl){
        setNAl();
    }
    return res;
}

void shiftArrayToRight(String *arr, int len, int start) {
  for(int j = len-1; j > start; j--){
    arr[j] = arr[j-1];
  }
}

bool delAlr(int p) {
    if(p < 0 || p > ALC-1){
      return false;
    }
    byte i = 0;
    for (i=p; i < ALC-1; i++) {
        als[i] = als[i+1];
    }
    als[ALC-1] = "0";
    if(p == curAlInd){
      if((p == 0) && (als[1] == "0")) {
        clk.clearAlarm1();
        curAlInd = -1;
      } else {
        setNAl();
      }
    }
    return true;
}

String getP(){
  return String(opAn);
}

bool setP(String s){
  opAn = atoi(s.c_str());
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

void setNAl() {
    int nextAlIn = curAlInd+1;
    if(nextAlIn > ALC-1) {
        nextAlIn = 0;
    }
    if(als[nextAlIn] != "0") {
        curAlInd = nextAlIn;
        int time[2];
        parseTime(als[curAlInd], time, 2);
        // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
        clk.setAlarm1(0, time[0], time[1], 0, DS3231_MATCH_H_M_S);
    } else {
        curAlInd = -1;
        if(als[0] != "0") {
          setNAl();
        }
    }
}

bool setActiveAlarm(int index) {
  if((index < 0) || (index > (ALC - 1)) || (als[index] == "0"))  {
    return false;
  }
  curAlInd = index - 1;
  setNAl();
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

void parseM(String message, String *res){
  splitString(message, '-', res);
}

void feed() {
    Servo servo;
    servo.attach(5);
    servo.write(opAn);
    delay(1000);
    servo.write(0);
    delay(1000);
    servo.detach();
}

void setInterrupt()         // here we put the arduino to sleep
{
    delay(100);                     // so sleep is possible. just a safety pin
    attachInterrupt(digitalPinToInterrupt(3),wakeUpAlarm, RISING); // use interrupt 0 (pin 2) and run function                            
    delay(500);                         
}

void wakeUpAlarm()        // here the interrupt is handled after wakeup
{
  iA = true;
}

void ans(String d, bool softSer) {
  if(softSer) {
    mySer.println(d + "\r\n\r\n");
  } else {
    Serial.println(d + "\r\n\r\n");
  }
}

void loop()
{
  if(iA){
    clk.clearAlarm1();
    iA = false;
    feed();
    setNAl();
  } else {
   String mes = "";
   bool sofSer = false;
   if (Serial.available()) {
      mes = Serial.readString();
      mySer.println("1");
      mySer.println(mes);
   }
   if (mySer.available()) {
      mes = mySer.readString();
      sofSer = true;
   }
   if (mes != "")
     {
       String d[2];
       parseM(mes,d);
       String com = d[0];
       String f = "f";
       String s = "t";
       if((com == "wake") || (com == "ake")){
          ans(s, sofSer);
       } else if(com == "sT"){
          ans(setTime(d[1]) ? s : f, sofSer);
       } else if(com == "T"){
          ans(getTime(false), sofSer);
       } else if(com == "adA"){
          ans(addAlarm(d[1]) ? s : f, sofSer);
       } else if(com == "dA"){
          ans(delAlr(atoi(d[1].c_str())) ? s : f, sofSer);
       } else if(com == "as"){
          ans(getals(), sofSer);
       } else if(com == "f"){
          feed();
          ans(s, sofSer);
       } else if(com == "sAA"){
          ans(setActiveAlarm(atoi(d[1].c_str())) ? s : f, sofSer);
       } else if(com == "P") {
          ans(getP(), sofSer);
       } else if(com == "sP") {
          ans(setP(d[1]) ? s : f, sofSer);
       } else {
          ans("", sofSer);
       }
     }
  }
}
