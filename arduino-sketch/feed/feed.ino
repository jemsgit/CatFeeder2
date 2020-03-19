
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

DS3231 clock;
RTCDateTime dt;
RTCAlarmTime at;
Servo myservo;
SoftwareSerial mySerial(2, 6);

bool isAl = false;
bool ULis = false;
int clAn = 160;
int opAn = 60;
String message;
String defA = "04:00";
int alarmsCount = 5;
String alarms[5] = {"0","0","0","0","0"};
int currentAlarmIndex = -1;
int slStM = -1;
int PIN_SQW = 3;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  clock.begin();
  clearRTCAlarms();
  setInterruptionMode();
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2020, 3, 3, 20, 00, 0);
  setDefaultAlarm();
  setNAl();
  // Attach Interrput 0. In Arduino UNO connect DS3231 INT to Arduino Pin 2
}

void clearRTCAlarms() {
    clock.armAlarm1(false);
    clock.armAlarm2(false);
    clock.clearAlarm1();
    clock.clearAlarm2();
}

void setDefaultAlarm() {
  alarms[0] = defA;
}

void setInterruptionMode() {
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00000111);
  Wire.endTransmission();
}

String getAlarms() {
    String res = "";
    at = clock.getAlarm1();
    String a = clock.dateFormat("H:i", at);
    int i = 0;
    while((i < alarmsCount) && (alarms[i] != "0")){
      if(alarms[i] == a){
        res += "** ";
      }
      res += i;
      res += " - " + alarms[i] + "\r\n";
      i++;
    }
    return res;
}

bool addAlarm(String time) {
    int i = 0;
    bool result = false;
    bool needTosetNAl = false;
    //all alarms are set
    if(alarms[alarmsCount-1] != "0"){
      return result;
    }
      for (i=0; i < alarmsCount; i++) {
        String currentTime = getShortTime();
        if(alarms[i] == "0"){
          alarms[i] = time;
          result = true;
          needTosetNAl = i == 0;
          //if current time if after last alarm and before new alarm
          if(i > 0
            && strcmp(currentTime.c_str(), alarms[i].c_str()) < 0
            && strcmp(currentTime.c_str(), alarms[i-1].c_str()) > 0) {
              needTosetNAl = true;
            }
          break;
        }
        //if new alarm is earlier then current
        if(strcmp(time.c_str(), alarms[i].c_str()) < 0){
          String temp = alarms[i];
          alarms[i] = time;
          shiftArrayToRight(alarms, alarmsCount, i+1);
          alarms[i+1] = temp;
          //if alarm was set to current alarm
          if(currentAlarmIndex == i){
            //check if current time is before new alarm
            if(strcmp(currentTime.c_str(), alarms[i].c_str()) < 0) {
              currentAlarmIndex-=1;
              needTosetNAl = true;
            } else {
              currentAlarmIndex+=1;
            }
          }
          result = true;
          break;
        }
      }

    if(needTosetNAl){
        setNAl();
    }
    return result;
}

void shiftArrayToRight(String *arr, int len, int start) {
  for(int j = len-1; j > start; j--){
    arr[j] = arr[j-1];
  }
}

bool deleteAlarm(int p) {
    int i = 0;
    if(p < 0 || p > alarmsCount-1){
      return false;
    }
    
    for (i=p; i < alarmsCount-1; i++) {
        alarms[i] = alarms[i+1];
    }
    alarms[alarmsCount-1] = "0";
    if(p == currentAlarmIndex){
      setNAl();
    }
    return true;
}

String getTime() {
  dt = clock.getDateTime();
  return clock.dateFormat("d-m-Y H:i:s",  dt);
}

String getShortTime() {
  dt = clock.getDateTime();
  return clock.dateFormat("H:i", dt);  
}

int getMinute() {
    dt = clock.getDateTime();
    char *m = clock.dateFormat("i", dt);
    return atoi(m);
}

bool setTime(String dt) {
  int d[5];
  parseTime(dt, d, 5);
  clock.setDateTime(d[0], d[1], d[2], d[3], d[4], 0);
  return true; 
}

void setNAl() {
    int nextAlarmIndex = currentAlarmIndex+1;
    if(nextAlarmIndex > alarmsCount-1) {
        nextAlarmIndex = 0;
    }
    if(alarms[nextAlarmIndex] != "0") {
          currentAlarmIndex = nextAlarmIndex;
          int time[2];
          parseTime(alarms[currentAlarmIndex], time, 2);
          // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
          clock.setAlarm1(0, time[0], time[1], 0, DS3231_MATCH_H_M_S);
    } else {
      //Serial.println("set NExt - else");
        currentAlarmIndex = -1;
        if(alarms[0] != "0") {
          setNAl();
        }
    }
}

bool setActiveAlarm(int index) {
  if((index < 0) || (index > (alarmsCount - 1)) || (alarms[index] == "0"))  {
    return false;
  } else {
    currentAlarmIndex = index - 1;
    setNAl();
    return true;
  }
}

void splitString(String input, char delimeter, String *result) {
    int li = 0;
    int c = 0;
   for (int i = 0; i < input.length(); i++) {
        // Loop through each character and check if it's a comma
        if (input.charAt(i) == delimeter) {
         // Grab the piece from the last index up to the current position and store it
          result[c] = input.substring(li, i);
          Serial.print(li);
          Serial.print(i);
          Serial.print(result[c]);
          // Update the last position and add 1, so it starts from the next character
          li = i + 1;
          // Increase the position in the array that we store into
          c++;
          
        }

        // If we're at the end of the string (no more commas to stop us)
        if (i == input.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          result[c] = input.substring(li, i+1);
        }
      }
      Serial.println("End");Serial.println("goMAtchLast");Serial.println("goMAtchLast");
}

void parseTime(String time2, int *result, int parts){
  int maxResLen = 5;
  Serial.print("TIME");
  Serial.println(time2);
  String res[maxResLen];
  splitString(time2, ':', res);
  
  for(int i = 0; i < parts; i++){
    Serial.println(res[i]);
    result[i] = atoi(res[i].c_str());
  }
}

void parseM(String message, String *res){
  splitString(message, '-', res);
}

bool feed() {
    myservo.attach(5); // attaches the servo on pin 5 to the servo object
    myservo.write(opAn); //открываем кормушку
    delay(1000); //ждем 1 секунду
    myservo.write(clAn); //закрываем
    delay(1000); //ждем 1 секунду
    myservo.detach();
    return true;
}

void sleepNow()         // here we put the arduino to sleep
{
    Serial.println("sleep");
    delay(500);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register                        // so sleep is possible. just a safety pin
    attachInterrupt(digitalPinToInterrupt(3),wakeUpAlarm, RISING); // use interrupt 0 (pin 2) and run function
    attachInterrupt(digitalPinToInterrupt(2),wakeUpUART, FALLING);  // wakeUpNow when pin 2 gets LOW
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP   
    sleep_disable();         // first thing after waking from sleep:
    detachInterrupt(0);
    detachInterrupt(1);// disable sleep...                             
    delay(500);
    Serial.print("woked");                         
}

void wakeUpAlarm()        // here the interrupt is handled after wakeup
{
  isAl = true;
}

void wakeUpUART() 
{
  ULis = true;
  isAl = false; 
}

void updateSleepTime() {
  int curM = getMinute();
  slStM = curM + 3;
  if(slStM >= 60){
     slStM -= 60;
  }  
}

void senPac(String d) {
  mySerial.println(d + "\r\n\r\n");
  Serial.println(d + "\r\n\r\n");
}

void loop()
{
  if(isAl){
    clock.clearAlarm1();
    isAl = false;
    feed();
    setNAl();
  } else if(ULis){
    if(slStM < 0){
      updateSleepTime();
    }
   if (mySerial.available())
     {  
       message = mySerial.readString();//прочитать из порта
       String data[2];
       parseM(message,data);
       String command = data[0];
       String params = data[1];
 
       if(command == "wake"){
          updateSleepTime();
       }
       if(command == "setTime"){
          senPac(setTime(params) ? "true" : "false");
       } else if(command == "getTime"){
          senPac(getTime());
       } else if(command == "addAlarm"){
          senPac(addAlarm(params) ? "true" : "false");
       } else if(command == "deleteAlarm"){
          senPac(deleteAlarm(atoi(params.c_str())) ? "true" : "false");
       } else if(command == "getAlarms"){
          senPac(getAlarms());
       } else if(command == "feed"){
          senPac(feed() ? "true" : "false");
       } else if(command == "setActiveAlarm"){
          senPac(setActiveAlarm(atoi(params.c_str())) ? "true" : "false");
       }
       Serial.println(command); //отправить в другой порт
     }
     if(getMinute() >= slStM){
        ULis = false;
        slStM = -1;
     }
  } else {
    sleepNow();
  }
  
}
