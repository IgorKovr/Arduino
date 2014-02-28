
#include "SIM900.h"
 
#include <SoftwareSerial.h>
 
#include "inetGSM.h"
 
InetGSM inet;

#define xiv
 
int k=0;
 
int j=0;
 
char msg[1000];
 
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
 
//  numdata=inet.httpPOST("www.evildeejay.it",     80, "/test/test.php", "name=Marco&age=24",msg, 50);
//  numdata=inet.httpGET("api.xively.com", 80, "/v2/feeds/1799206591.csv", "X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O", msg, 1000);
  
  
  inet.httpPUT("api.xively.com", 80, "/v2/feeds/1799206591.csv", "switch,2014-02-25T23:42:17.285954Z,1" , "X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O", msg, 1000);
  Serial.println(msg);
  
//  delay(1000);
  
}
 
};
 
void loop()
 
{
  
//  getXivelyData();
  serialswread();
 
};
 
void serialswread(){
 
gsm.SimpleRead();
 
}

//void getXivelyData(){
//  if (inet.attachGPRS("internet", "", "")){
//     numdata=inet.httpGET("api.xively.com", 80, "/v2/feeds/1799206591.json", 
//    "X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O", msg, 10);
//    Serial.println(numdata);
//  }
//}
