#include <PubSubClient.h>
//-------------Thiết lập----------------
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

//--------------------------------------
//---- Khai báo wifi-------------
const char* ssid = "3Cs_LAB";
const char* pass = "3cslabsince2015";
//-----Khai báo thông số MQTT-----------
const char* mqtt_server = "mqtt.ngoinhaiot.com"; 
const char* mqtt_user = "tranthanh1602"; 
const char* mqtt_pass = "tranthanh1602";
int mqtt_port = 1111; // ESP kết nối với esp qua cổng tcp
String topicSub = "tranthanh1602/sub"; // Nhận dữ liệu từ MQTT
String topicPub = "tranthanh1602/pub"; // Gửi dữ liệu 
//---------------------------------------
WiFiClient espClient; 
PubSubClient client(espClient); 
//---------------------------------------
#define RX D6
#define TX D5
SoftwareSerial mySerial(RX, TX); 
String inString = ""; 
int nhietdo = 0; 
int doam = 0; 
int uv = 0; 
int aqi = 0; 
String G = "G"; 
String H = "H"; 
bool check = false; 
unsigned long last = millis(); 
String DataJson = ""; 
String Data_mqtt = ""; 
//----------Gọi các hàm con---------------
void ConnectMqttBroker();
void callback(char* topic, byte* payload, unsigned int length); 
void reconnect(); 
//-----------------------------------------
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  while(!Serial);
  mySerial.begin(9600); 
  while(!mySerial);
  last = millis(); 
  
  //---------Thiết lập Wifi-----------------
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect(); 
  WiFi.begin(ssid, pass); 
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500); 
    Serial.print(".");
  }
  Serial.println(); 
  Serial.println("Connected WiFi"); 
  ConnectMqttBroker();
  last = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  Read_Uart(); 
  // Kiểm tra kết nối với Mqtt
  if(!client.connected())
  {
    reconnect(); 
  }
  // Duy trì kết nối 
  client.loop(); 
  // Gửi yêu cầu lấy dữ liệu xuống Arduino mega
  if(millis() - last >= 3000)
  {
    yeucau(); 
    Json(String(nhietdo), String(doam), String(uv), String(aqi)); 
    last = millis(); 
  }
}
//==============================================
//-------------Viết Hàm Con Dưới đây------------
//------------void yeucau begin-----------------
void yeucau()
{
  String gui = G + H; 
  mySerial.println(gui); 
  mySerial.flush(); 
  Serial.print("Data gửi xuống Arduino Mega: "); 
  Serial.println(gui);
    
}
//-----------void yeucau end--------------------
//-----------void Read_Uart() begin-------------
void Read_Uart()
{
  while(mySerial.available())
  {
    //----------Code begin here-----------------
    char inChar = (char)mySerial.read(); 
    inString += inChar; 
    if(inChar == '\n')
    {
      check = true; 
    }
    if(check)
    {
      Serial.print("Dữ liệu nhận được từ Arduino: "); 
      Serial.println(inString); 
      // Dữ liệu có dạng A[nhietdo]B[doam]C[uv]D[aqi]E
      int TimA, TimB, TimC, TimD, TimE = -1; 
      TimA = inString.indexOf("A"); 
      TimB = inString.indexOf("B"); 
      TimC = inString.indexOf("C"); 
      TimD = inString.indexOf("D"); 
      TimE = inString.indexOf("E"); 
      if(TimA >= 0 && TimB >= 0 && TimC >= 0 && TimD >= 0 && TimE)
      {
        String data_nhietdo = inString.substring(TimA + 1, TimB); 
        String data_doam = inString.substring(TimB + 1, TimC);
        String data_uv = inString.substring(TimC + 1, TimD);
        String data_aqi = inString.substring(TimD + 1, TimE);
        nhietdo = data_nhietdo.toInt(); 
        doam = data_doam.toInt(); 
        uv = data_uv.toInt(); 
        aqi = data_aqi.toInt(); 
        /*
        Serial.print("Nhiệt đô là: ");
        Serial.println(nhietdo); 
        Serial.print("Độ ẩm là: ");
        Serial.println(doam);
        */
      }
      //----------Reset Data--------------------
      
      check = false;
      inString = ""; 
      
    }
    //----------Code end here-------------------
  }
}
//-----------void Read_Uart() end---------------
//-----------void Json() begin------------------
void Json(String t, String h, String u, String a)
{
  // Hàm chuyển dữ liệu về chuẩn Json rồi gửi lên Server
  DataJson = ""; 
  // Data có dạng {"nhietdo":"data_nhietdo", "doam":"data_doam"}; 
  DataJson = "{\"nhietdo\":\""+String(t)+"\", \"doam\":\""+String(h)+"\", \"uv\":\""+String(u)+"\", \"aqi\":\""+String(a)+"\"}";
  Serial.print("Data Json: "); 
  Serial.println(DataJson); 
  // Gửi data tới Mqtt 
  // Topic là kí tự char. ép kiểu bằng c_str
  client.publish(topicPub.c_str(), DataJson.c_str()); 
  
}
//-----------void Json End----------------------
//-----------void ConnectMqttBroker() begin-----
void ConnectMqttBroker()
{
  client.setServer(mqtt_server, mqtt_port); 
  delay(10); 
  client.setCallback(callback); // => Đọc dữ liệu từ server
  delay(10); 
}
/*
 *      ||
 *      ||
 *      Hàm callback viết dưới đây 
 */
void callback(char* topic, byte* payload, unsigned int length)
{
  for(int i = 0; i < length; i++)
  {
    Data_mqtt += (char)payload[i]; 
  }
  Serial.print("Data nhận được: "); 
  Serial.println(Data_mqtt); 
  delay(100); 
  Data_mqtt = ""; 
}
//-----------void ConnectMqttBroker() end-----
//-----------void reconnect()  begin-----
void reconnect() 
{
  while(!client.connected())
  {
    String clientID = "ESP8266Client_1"; 
    if(client.connect(clientID.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected MQTT NgoinhaIoT.com"); 
      client.subscribe(topicSub.c_str()); // Nhận dữ liệu
    }
    else
    {
      Serial.print("failed, rc = "); 
      Serial.println(client.state()); 
      Serial.println("Kết nối lại sau 5 giây"); 
      delay(5000); 
    }
  }
}
//-----------void reconnect()  end-----
