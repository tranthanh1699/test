//-----------------------Viết cho một số loại cảm biến---------------------
// Hàm đọc và tính toán aqi rồi gửi lên server
void Air_host(int *t, int *h, int *u, int *aqi)
{
  // Đọc cảm biến nhiệt độ độ ẩm
  // suy nghĩ về việc dùng dht22

  // Đọc cảm biến UV
  int sensorValue = 0;
  sensorValue = analogRead(A0);                        //connect UV sensor to Analog 0   
  int voltage = (sensorValue * (5.0 / 1023.0))*1000;  //Voltage in miliVolts
  if(voltage<50)
  {
    *u = 0; 
  }else if (voltage>50 && voltage<=227)
  {
    *u = 0; 
  }else if (voltage>227 && voltage<=318)
  {
    *u = 1; 
  }
  else if (voltage>318 && voltage<=408)
  {
    *u = 2; 
  }else if (voltage>408 && voltage<=503)
  {
    *u = 3; 
  }
  else if (voltage>503 && voltage<=606)
  {
    *u = 4; 
  }else if (voltage>606 && voltage<=696)
  {
    *u = 5; 
  }else if (voltage>696 && voltage<=795)
  {
    *u = 6;
  }else if (voltage>795 && voltage<=881)
  {
    *u = 7;
  }
  else if (voltage>881 && voltage<=976)
  {
    *u = 8;
  }
  else if (voltage>976 && voltage<=1079)
  {
    *u = 9;
  }
  else if (voltage>1079 && voltage<=1170)
  {
    *u = 10;
  }else if (voltage>1170)
  {
    *u = 11;
  }
  /*
   * Thiết lập đọc full trạng thái của cảm biến 
   * Dải đo NO   0.05 - 10 ppm   
   * Dải đo CO 1 - 1000 ppm
   * Dải NH3 1-300 ppm
   *  Đọc bằng tín hiệu Analog
   */
   int a = analogRead(A1); 
   int b = analogRead(A2); 
   int c = analogRead(A3); 
   float NO = map(a, 0, 1023, 0.05, 10); 
   int CO = map(b, 0, 1023, 1, 1000); 
   int NH3 = map(c, 0, 1023, 1, 300); 
   // Tính toán AQI
   
   
  
  
}
