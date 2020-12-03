#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h> // font libaray

WebSocketClient webSocketClient;
WiFiClient client;
SPIDMD dmd(5, 1);
int TIMEOUT = 2000;

int i = 0;
const uint8_t *FONT = SystemFont5x7;
DMD_TextBox box(dmd, 0, 0, 32, 16); // x, y change text posistion [ Set Box (dmd, x, y, Height, Width)]

String message = "TEST";

const char* ssid     = "123";
const char* password = "123";
char path[] = "/";
char host[] = "ip_addres_of_server";

void setup() {
  Serial.begin(9600);
  connectWiFi();
  connectWS();
  delay(100);
  dmd.selectFont(FONT); // Font used
  dmd.begin();
}

void connectWS() {
  if(client.connect(host, 8084)) {
    webSocketClient.path = path;
    webSocketClient.host = host;
    webSocketClient.handshake(client);
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  delay(1000);
}

void wsServerHandler() {
  String data;
  if (client.connected()) {
    webSocketClient.getData(data);
    if (data.length() > 0) {
      processMessage(data);
    }  
  } else {
    connectWS();
  }
}

void sendResult(String data) {
  if (client.connected()) {
    webSocketClient.sendData(data);
  }
}

void processMessage(String message) {
  String result[3] = {"", "", ""};
  parseMessage(message, result);
  String commandResult = runCommand(result[0], result[1]);
  if(result[2] != "") {
    //if massage has id
    commandResult = "id:" + result[2] + commandResult;
  }
  sendResult(commandResult);
}

String runCommand(String command, String params) {
  if(command == "print" || command == "ping") {
    setPrintMessage(params);
    return "true";
  } else {
    Serial.print(command + "-" + params);
    return getAnswer();
  }
}

String getAnswer() {
  unsigned long start = millis();
  String result = "";
  while(!Serial.available()) {
    delay(50);
    if(millis() - start > TIMEOUT) {
      result = "f";
      break;
    }
  }
  if(result == "") {
    result = Serial.readString();
  }
  return result;
  
}

void checkSerialData() {
  if(Serial.available()) {
    sendResult(Serial.readString());
  }
}

void setPrintMessage(String text) {
  message = text;
  dmd.clearScreen();
}

void parseMessage(String message, String *result) {
  splitString(message, '-', result);
  if(result[1] != ""){
    String par[2] = {"", ""};
    splitString(result[1], '+', par);
    if(par[1] != "") {
      result[1] = par[0];
      result[2] = par[1];
    }
  }  
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


void printmessage(String message) {
  box.clear();
  dmd.drawString(0, 6, message);
  int length = dmd.stringWidth(message, FONT);
  i = 0;
  while(i < length) {
    dmd.marqueeScrollX(-1);
    delay(50);
    i++;
  }  
}

void scrolling() {
  int length = message.length();
  while(length > 22) {
    String st = message.substring(0, 22);
    message = message.substring(22);
    printmessage(st);
    length = message.length();
  }
  if(length > 0) {
    printmessage(message);
    length = 0;
    message = "";
  }
}

void loop() {
  scrolling();
  wsServerHandler();
  checkSerialData(); 
}
