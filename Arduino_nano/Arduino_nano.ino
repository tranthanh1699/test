#include <Wire.h>
#include <Adafruit_AM2315.h>
#include "DHT.h"
#include "AQI.h"
#include "UV.h"
Adafruit_AM2315 am2315;
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
String A = "A"; 
String B = "B"; 
String C = "C"; 
String D = "D"; 
String E = "E"; 
String inString = ""; 
String outString = ""; 
unsigned long last = 0; 
bool check = false; 
// Khu vực lưu biến gửi lên server 
int nhietdo = 0; 
int doam = 0; 
int uv = 0; 
int aqi = 0; 
//---------------
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  while(!Serial); 
  Serial1.begin(9600); 
  while(!Serial1); 
  Serial.println("Project Demo"); 
  dht.begin();
  last = millis(); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Read_Uart();
}
//=============================================
//---------Hàm Con Viết Dưới Đây---------------

//-------------void Read_Uart() begin----------
void Read_Uart()
{
  //--------Code begin here------------
  while(Serial1.available())
  {
    char inChar = (char)Serial1.read(); 
    inString += inChar; 
    if(inChar == '\n')
    {
      check = true; 
    }
    //----------Code xử lý data bắt đầu----
    if(check)
    {
      Serial.print("Dữ liệu nhận từ ESP: "); 
      Serial.println(inString); 
      int TimG, TimH = -1; 
      TimG = inString.indexOf("G"); 
      TimH = inString.indexOf("H"); 
      if(TimG >= 0 && TimH >= 0)
      {
        Send_Uart(); 
      }
      
      //------Reset lại biến------------
      //------Không viết thêm code dưới đây---
      inString = ""; 
      check = false; 
    }
    //------Code xử lý data kết thúc-------
  }
  //--------End Code-------------------
}
//-----------void Read_Uart() end--------------
//-----------void Send_Uart() begin------------
void Send_Uart()
{
  nhietdo = dht.readTemperature();
  doam = dht.readHumidity();
  if (isnan(nhietdo) || isnan(doam)) 
  {
    Serial.println("Failed to read from DHT");
  }
  uv++;
  aqi = aqi + 2;  
  String dataS = ""; 
  dataS = A + nhietdo + B + doam + C  + uv + D + aqi + E; 
  Serial1.println(dataS); 
  Serial1.flush(); 
  Serial.print("Dữ liệu gửi lên ESP: "); 
  Serial.println(dataS); 
}
//-----------void Send_Uart() end--------------
