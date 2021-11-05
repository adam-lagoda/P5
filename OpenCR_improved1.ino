/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) */

#include <DynamixelWorkbench.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  57600
#define DXL_ID_1  1
#define DXL_ID_2  2

#define STRING_BUF_NUM 64
String cmd[STRING_BUF_NUM];

void split(String data, char separator, String* temp);

int speed1 = 0;
int speed2 = 0;

uint8_t dxl_id_1 = DXL_ID_1;
uint8_t dxl_id_2 = DXL_ID_2;
uint16_t model_number = 0;

DynamixelWorkbench dxl_wb;

void setup() 
{
  Serial.begin(57600);
  while(!Serial); // Wait for Opening Serial Monitor

  const char *log;
  bool result1 = false;
  bool result2 = false;

  result1 = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result1 == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }

  result1 = dxl_wb.ping(dxl_id_1, &model_number, &log);
  if (result1 == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id_1);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  
  result2 = dxl_wb.ping(dxl_id_2, &model_number, &log);
  if (result2 == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id_2);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }

  result1 = dxl_wb.wheelMode(dxl_id_1, 0, &log);
  result2 = dxl_wb.wheelMode(dxl_id_2, 0, &log);
  if (result1 == false || result2 == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
  else{
    Serial.println("Succeed to change wheel mode"); 
  }
}

void loop() {
  if (Serial.available()){
    String read_string = Serial.readStringUntil('\n');
    Serial.println("[CMD] : " + String(read_string));
  
    read_string.trim();
    split(read_string, ' ', cmd);

    if (cmd[0] == "wheel"){
        speed1 = cmd[1].toInt();
        speed2 = cmd[2].toInt();
    }
  }
  Serial.print("Wheel 1 speed: ");
  Serial.println(speed1);
  Serial.print("Wheel 2 speed: ");
  Serial.println(speed2);
    
  dxl_wb.goalVelocity(dxl_id_1, (int32_t)speed1);
  dxl_wb.goalVelocity(dxl_id_2, (int32_t)speed2);
}

void split(String data, char separator, String* temp){
  int cnt = 0;
  int get_index = 0;

  String copy = data;
  
  while(true)
  {
    get_index = copy.indexOf(separator);

    if(-1 != get_index)
    {
      temp[cnt] = copy.substring(0, get_index);

      copy = copy.substring(get_index + 1);
    }
    else
    {
      temp[cnt] = copy.substring(0, copy.length());
      break;
    }
    ++cnt;
  }
}


 /*for (int i=0; i<100; i++){
      dxl_wb.goalVelocity(dxl_id_1, (int32_t)-i);
      dxl_wb.goalVelocity(dxl_id_2, (int32_t)-i);
      delay(100);
      }

      for (int i=0; i<100; i++){
      dxl_wb.goalVelocity(dxl_id_1, (int32_t)i);
      dxl_wb.goalVelocity(dxl_id_2, (int32_t)i);
      delay(100);
      }*/
