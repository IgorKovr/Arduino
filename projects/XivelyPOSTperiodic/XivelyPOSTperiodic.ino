#include "SIM900.h"
 
#include <SoftwareSerial.h>
 
#include "inetGSM.h"
 
InetGSM inet;
 
int k=0;
 
int timeM = 45;
int timeH = 19;
int j=0;
 
char msg[150];
 
boolean found=false;
 
char data;
 
int numdata;
 
char inSerial[50];
 
int i=0;
 
boolean started=false;
 
void setup()
 
{
 
Serial.begin(9600);
 
Serial.println("GSM Shield testing.");
 
if (gsm.begin(4800)){
 
  Serial.println("\nstatus=READY");
 
  started=true;  
 
}
 
else Serial.println("\nstatus=IDLE");
 
if(started){
 
  if (inet.attachGPRS("internet", "", ""))
 
    Serial.println("status=ATTACHED");
 
  else Serial.println("status=ERROR");
 
  delay(1000);
 
//  numdata=inet.httpPOST("www.evildeejay.it", 80, "/test/test.php", "name=Marco&age=24",msg, 50);
//  numdata=inet.httpPUT("api.xively.com",     80, "/v2/feeds/1799206591.csv", "temp,2014-03-27T17:40:00.00,22.2", "", msg, 50);
//  inet.httpPUT("api.xively.com", 80, "/v2/feeds/1799206591.csv", "switch,2014-02-25T23:42:17.285954Z,0" , "X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O", msg, 50);
//    inet.httpGET("www.google.com", 80, "/", "", msg, 50);
 
}
 
};
 
void loop()
 
{
  timeM++;
  if (timeM> 60){
   timeM=0; 
   timeH++; 
  }
  
 if (timeH> 24){
   timeH=0; 
  }

  sendData(22, 20);
  serialswread();
//  readResponce();
  delay(20000);
};

void readResponce(){
//    for (i=0;i<=200000;i++){
//      serialswread();
//  }
}

void sendData(int temp, int humidity){
  
//  sensor1,2014-02-27T18:23:21.880457Z,0
//switch,2014-02-27T18:24:01.285954Z,0


  Serial.println("Sending data...");
  String tempStr = "temp,";
  tempStr = tempStr + "2014-02-27T";
  tempStr = tempStr + timeH;
  tempStr = tempStr + ":";
  tempStr = tempStr + timeM;
  tempStr = tempStr + ":40.285954Z,";
  tempStr = tempStr + temp + "   ";
  char sendStr[tempStr.length()]; 
  tempStr.toCharArray(sendStr, tempStr.length());
  Serial.println(tempStr);
   numdata=inet.httpPUT("api.xively.com",     80, "/v2/feeds/1799206591.csv", sendStr, "", msg, 50);
   delay(1000);
}
 
void serialswread(){
 
gsm.SimpleRead();
 
}
