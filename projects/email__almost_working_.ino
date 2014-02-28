#include "SIM900.h"
 
#include <SoftwareSerial.h>
 
#include "inetGSM.h"
 
InetGSM inet;
 
int k=0;
 
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
 
}
 
};
 
void loop()
 
{
// 
//serialhwread();
// 
//serialswread();
// 
//};
// 
//void serialhwread(){
// 
//i=0;
// 
//if (Serial.available() > 0){            
// 
//  while (Serial.available() > 0) {
// 
//    inSerial[i]=(Serial.read());
// 
//    delay(10);
// 
//    i++;      
// 
//  }
// 
//  inSerial[i]='\0';
// 
//  if(!strcmp(inSerial,"/END")){
// 
//    Serial.println("_");
// 
//    inSerial[0]=0x1a;
// 
//    inSerial[1]='\0';
// 
//    gsm.SimpleWriteln(inSerial);
// 
//  }
// 
//  //Send a saved AT command using serial port.
// 
//  if(!strcmp(inSerial,"EMAIL")){
 
    inet.openmail("smtp.yandex.ru", "igorkovr@yandex.ru", "kovr4130", "IgorKovr@yandex.ru"," IgorKovr@gmail.com ", "Arduino");
 
    gsm.SimpleWrite("MESSAGGIO DI PROVA");

    inet.closemail();
 
//  }
// 
//  else{
// 
//    Serial.println(inSerial);
// 
//    gsm.SimpleWriteln(inSerial);
// 
//  }    
// 
//  inSerial[0]='\0';
// 
//}
// 
}
 
void serialswread(){
 
gsm.SimpleRead();
 
}
