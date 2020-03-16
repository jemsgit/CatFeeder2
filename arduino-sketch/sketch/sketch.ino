
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
SoftwareSerial mySerial(10, 11);

bool isAlarm = false;
bool UARTListen = true;
int closeAngle = 160;
int openAngle = 60;
String message;
String defaultAlarm = "04:00";
int alarmsCount = 5;
String alarms[5] = {"0","0","0","0","0"};
int currentAlarmIndex = -1;
int sleepStartMinute = -1;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();
 
  // Disarm alarms and clear alarms for this example, because alarms is battery backed.
  // Under normal conditions, the settings should be reset after power and restart microcontroller.
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();
  setInterruptionMode();
  // Manual (Year, Month, Day, Hour, Minute, Second)
  //clock.setDateTime(2018, 7, 7, 20, 18, 0);
  alarms[0] = defaultAlarm;
  setNextAlarm();
  //clock.setAlarm1(0, 4, 0, 0, DS3231_MATCH_H_M_S);
  
  // Attach Interrput 0. In Arduino UNO connect DS3231 INT to Arduino Pin 2
}

void setInterruptionMode() {
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00000111);
  Wire.endTransmission();
}

String getAlarms() {
    String result = "";
    at = clock.getAlarm1();
    String alarm = clock.dateFormat("H:i", at);
    int i = 0;
    while((i < alarmsCount) && (alarms[i] != "0")){
      if(alarms[i] == alarm){
        result += "** ";
      }
      result += i;
      result += " - " + alarms[i] + "\r\n";
      i++;
    }
    return result;
}

bool addAlarm(String time) {
    int i = 0;
    bool result = false;
    bool needTosetNextAlarm = false;
    if(alarms[alarmsCount-1] != "0"){
      return result;
    }
    if(alarms[0] == "0"){
      needTosetNextAlarm = true;
    }
    for (i=0; i < alarmsCount; i++) {
      if(alarms[i] == "0") {
        alarms[i] = time;
        result = true;
        break;
      } else if(strcmp(time.c_str(), alarms[i].c_str()) < 0){
        String temp = alarms[i];
        alarms[i] = time;
        for(int j = alarmsCount-1; j > i+1; j--){
          alarms[j] = alarms[j-1];
        }
        alarms[i+1] = temp;
        result = true;
        if(currentAlarmIndex > -1){
          needTosetNextAlarm = true;
          currentAlarmIndex-=1;
        }
        break;
      }
    }
    if(needTosetNextAlarm){
        setNextAlarm();
    }
    return result;
}

bool deleteAlarm(int position) {
    int i = 0;
    if(position < 0 || position > alarmsCount-1){
      return false;
    }
    
    for (i=position; i < alarmsCount-1; i++) {
        alarms[i] = alarms[i+1];
    }
    alarms[alarmsCount-1] = "0";
    if(position == currentAlarmIndex){
      setNextAlarm();
    }
    return true;
}

String getTime() {
  dt = clock.getDateTime();
  return clock.dateFormat("d-m-Y H:i:s - l", dt);
}

int getMinute() {
    dt = clock.getDateTime();
    char *minute = clock.dateFormat("i", dt);
    return atoi(minute);
}

bool setTime(String daetTime) {
  int date[5];
  parseTime(daetTime, date, 5);
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(date[0], date[1], date[2], date[3], date[4], 0);
  return true; 
}

void setNextAlarm() {
    //Serial.println("set NExt");
    int nextAlarmIndex = currentAlarmIndex+1;
    if(nextAlarmIndex > alarmsCount-1) {
        nextAlarmIndex = 0;
    }
    if(alarms[nextAlarmIndex] != "0") {
      //Serial.println("set NExt - 1");
          currentAlarmIndex = nextAlarmIndex;
          int time[2];
          parseTime(alarms[currentAlarmIndex], time, 2);
          // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
          clock.setAlarm1(0, time[0], time[1], 0, DS3231_MATCH_H_M_S);
    } else {
      //Serial.println("set NExt - else");
        currentAlarmIndex = -1;
        if(alarms[0] != "0") {
          setNextAlarm();
        }
    }
}

bool setActiveAlarm(int index) {
  if((index < 0) || (index > (alarmsCount - 1)) || (alarms[index] == "0"))  {
    return false;
  } else {
    currentAlarmIndex = index - 1;
    setNextAlarm();
    return true;
  }
}

void splitString(String input, String delimeter, String *result) {
    int lastIndex = 0;
    int counter = 0;
   for (int i = 0; i < input.length(); i++) {
        // Loop through each character and check if it's a comma
        if (input.substring(i, i+1) == delimeter) {
         // Grab the piece from the last index up to the current position and store it
          result[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          // Increase the position in the array that we store into
          counter++;
        }

        // If we're at the end of the string (no more commas to stop us)
        if (i == input.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          result[counter] = input.substring(lastIndex, i+1);
          Serial.println(result[counter]);
        }
      }
}

void parseTime(String time, int *result, int parts){
  String res[2];
  splitString(time, ":", res);
  int timeParts[2] = {0, 0};
  for(int i = 0; i < parts; i++){
    result[i] = atoi(res[i].c_str());
  }
  if((result[0] > 23) || (result[0] < 0)){
    result[0] = 00;  
  }
  if((result[1] > 59) || (result[1] < 0)){
    result[0] = 00;  
  }
}

void parseMessage(String message, String *res){
  splitString(message, "-", res);
}

bool feed() {
    myservo.attach(5); // attaches the servo on pin 5 to the servo object
    myservo.write(openAngle); //открываем кормушку
    delay(1000); //ждем 1 секунду
    myservo.write(closeAngle); //закрываем
    delay(1000); //ждем 1 секунду
    myservo.detach();
    return true;
}

void sleepNow()         // here we put the arduino to sleep
{
    Serial.println("go to sleep");
    delay(500);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register                        // so sleep is possible. just a safety pin
    attachInterrupt(0,wakeUpAlarm, FALLING); // use interrupt 0 (pin 2) and run function
    attachInterrupt(1,wakeUpUART, FALLING);                                   // wakeUpNow when pin 2 gets LOW
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP   
    sleep_disable();         // first thing after waking from sleep:
    detachInterrupt(0);
    detachInterrupt(1);// disable sleep...                              
    delay(500);                         
}

void wakeUpAlarm()        // here the interrupt is handled after wakeup
{
  isAlarm = true;
}

void wakeUpUART() 
{
  UARTListen = true;
}

void updateSleepTime() {
  int currentMinute = getMinute();
  sleepStartMinute = currentMinute + 3;
  if(sleepStartMinute >= 60){
     sleepStartMinute -= 60;
  }  
}

void sendPackage(String data) {
  mySerial.println(data + "\r\n\r\n");
  Serial.println(data + "\r\n\r\n");
}

void loop()
{
  if(isAlarm){
    Serial.println("alarm");
    clock.clearAlarm1();
    isAlarm = false;
    feed();
    setNextAlarm();
  } else if(UARTListen){
    if(sleepStartMinute < 0){
      updateSleepTime();
    }
   if (mySerial.available())
     {  
       message = mySerial.readString();//прочитать из порта
       String data[2];
       parseMessage(message,data);
       String command = data[0];
       String params = data[1];
       sendPackage(getTime());
       if(command == "wake"){
          updateSleepTime();
          sendPackage("woke");
       }
       if(command == "setTime"){
          sendPackage(setTime(params) ? "true" : "false");
       } else if(command == "getTime"){
          sendPackage(getTime());
       } else if(command == "addAlarm"){
          sendPackage(addAlarm(params) ? "true" : "false");
       } else if(command == "deleteAlarm"){
          sendPackage(deleteAlarm(atoi(params.c_str())) ? "true" : "false");
       } else if(command == "getAlarms"){
          sendPackage(getAlarms());
       } else if(command == "feed"){
          sendPackage(feed() ? "true" : "false");
       } else if(command == "setActiveAlarm"){
          sendPackage(setActiveAlarm(atoi(params.c_str())) ? "true" : "false");
       }
       Serial.println(command); //отправить в другой порт
     }
    if (Serial.available())
     {  
       message = Serial.readString();//прочитать из порта
       mySerial.println(message); //отправить в другой порт - debug
     }
     if(getMinute() >= sleepStartMinute){
        UARTListen = false;
        sleepStartMinute = -1;
     }
  } else {
    sleepNow();
  }
  
}
