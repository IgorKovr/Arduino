#include "SIM900.h"
 
#include <SoftwareSerial.h>
 
#include "inetGSM.h"

// setup  DHT server
#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE 11   // DHT 11 
#include <SD.h>
DHT dht(DHTPIN, DHTTYPE);
float temp;
float *humidity;
 
InetGSM inet;
 
int timeM = 1;
int timeH = 14;
int year=2014;
int month=02;
int day=28;
 
 char responce[250];

void setup()
{
    Serial.begin(9600);
    Serial.println("GSM Shield testing.");
    
    boolean started=false;
    
    if (gsm.begin(4800)){
      Serial.println("\nstatus=READY");
      started=true;  
    }
    else 
      Serial.println("\nstatus=IDLE");
     
    if(started){
      if (inet.attachGPRS("internet", "", ""))
        Serial.println("status=ATTACHED");
      else 
        Serial.println("status=ERROR");
      delay(1000);
    }
    
    Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(53, OUTPUT);
    
      if (!SD.begin(53)) {
    Serial.println("initialization SD failed!");
    return;
  }
  Serial.println("initialization SD done.");
  
  
  File myFile;
  myFile = SD.open("1.jpg");
  
    gsm.httpPOSTWithFile("api.xively.com", 80,"/v2/feeds/1799206591.csv", "", myFile, responce, 50)
  
       gsm.WhileSimpleReadToStr(responce);
     Serial.println(responce);
  
};
 
void loop()
 
{
   float h;
   humidity = &h;
   temp = readSensor(humidity);
  
  int t1 = temp;
  int h1 = *humidity;
   
   getTime();
   
   if (inet.attachGPRS("internet", "", "")){
     sendData(t1, h1);
  }
//     gsm.WhileSimpleRead();
     gsm.WhileSimpleReadToStr(responce);
     Serial.println(responce);
//    delay(60000);
};


void getTime(){
    timeM++;
  if (timeM> 60){
   timeM=0; 
   timeH++; 
  }
  
 if (timeH> 24){
   timeH=0; 
  }
};

void sendData(int temp, int humidity){
  Serial.println("Sending Data...");
  
  char chMonth[3];
  char chDay[3];
  char chMinutes[3];
  char chHour[3];
  char chSeconds[3];
  
  formatTimeDigits(chMonth, month);
  formatTimeDigits(chDay, day);
  formatTimeDigits(chMinutes, timeM);
  formatTimeDigits(chHour, timeH);
  
  String tempStr = "temp,";
  
  tempStr = tempStr + "2014-"+ chMonth + "-"+ chDay + "T" + chHour + ":" + chMinutes + ":40.285954Z," + temp + "\n";
  tempStr = tempStr + "humidity," + "2014-"+ chMonth + "-"+ chDay + "T"  + chHour + ":" + chMinutes + ":40.285954Z," + humidity + "   ";
  
  char sendStr[tempStr.length()]; 
  tempStr.toCharArray(sendStr, tempStr.length());
  Serial.println(tempStr);
   inet.httpPUT("api.xively.com",     80, "/v2/feeds/1799206591.csv", sendStr, "", responce, 50);
   delay(1000);
}

float readSensor(float *humid){
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  temp = t;
//  humidity = h;
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
//    Serial.print("Humidity: "); 
//    Serial.print(h);
//    Serial.print(" %\n");
//    Serial.print("Temperature: "); 
//    Serial.print(t);
//    Serial.println(" *C");
  } 
  *humid = h;
  return t; // Warning float to int cast
}

void formatTimeDigits(char strOut[3], int num)
{
  strOut[0] = '0' + (num / 10);
  strOut[1] = '0' + (num % 10);
  strOut[2] = '\0';
}
