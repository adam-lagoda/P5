/*.
  Range   : +/- 2 g
  Scale   : 16384 = 1 g = 9.806m/s^2 <=> 1m/s^2 = 1670.8137
 */
#include <DynamixelWorkbench.h>
#include <IMU.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  57600
#define DXL_ID_1  1
#define DXL_ID_2  2

#define STRING_BUF_NUM 64

cIMU    IMU;
DynamixelWorkbench dxl_wb;

float accConv =  0.0;
float accConv1=  0.0;
float accConv2=  0.0;
float accConv3=  0.0;
float accConv4=  0.0;
float accConv5=  0.0;
float accConv6=  0.0;
float accConv7=  0.0;
float accFilt=0.0;
float accFilt1=  0.0;
float accFilt2=  0.0;
float accFilt3=  0.0;
float accFilt4=  0.0;
float accFilt5=  0.0;
float accFilt6=  0.0;
float accFilt7=  0.0;

float velocity = 0.0;
float xpostion = 0.0;


static uint32_t currTime = 0;
static uint32_t lastTime = 0;

//float a[] = {0.028, 0.053, 0.071, 0.053, 0.028};
//float b[] = {1.000,-2.026, 2.148,-1.159, 0.279};
float a[] = {                               
0.000000000000022168066489779340973528935,  
0.000000000000177344531918234727788231481,  
0.000000000000620705861713821597745908117,  
0.000000000001241411723427643195491816234,  
0.000000000001551764654284554145826064095,  
0.000000000001241411723427642993543424498,  
0.000000000000620705861713821496771712249,  
0.000000000000177344531918234727788231481,  
0.000000000000022168066489779340973528935

};
float b[] = {  
  1                                        ,
 -7.796067792105057847606985887978225946426,
 26.593212048246140710716645116917788982391,
-51.840484417610895206962595693767070770264,
 63.166677341106058918285270920023322105408,
-49.263731316549048244723962852731347084045,
 24.015217039319459502166864695027470588684,
 -6.690325149349195754666652646847069263458,
  0.815502246948219711164540512982057407498                            
};

String cmd[STRING_BUF_NUM];

void split(String data, char separator, String* temp);

int speed1 = 0;
int speed2 = 0;

uint8_t dxl_id_1 = DXL_ID_1;
uint8_t dxl_id_2 = DXL_ID_2;
uint16_t model_number = 0;
  
void setup(){
  Serial.begin(57600);
  while(!Serial); // Wait for Opening Serial Monitor
  IMU.begin();
  
  const char *log;
  bool result1 = false;
  bool result2 = false;

  result1 = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result1 == false){
    Serial.println(log);
    Serial.println("Failed to init");
  }else{
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }

  result1 = dxl_wb.ping(dxl_id_1, &model_number, &log);
  if (result1 == false){
    Serial.println(log);
    Serial.println("Failed to ping");
  }else{
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id_1);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  
  result2 = dxl_wb.ping(dxl_id_2, &model_number, &log);
  if (result2 == false){
    Serial.println(log);
    Serial.println("Failed to ping");
  }else{
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id_2);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }

  result1 = dxl_wb.wheelMode(dxl_id_1, 0, &log);
  result2 = dxl_wb.wheelMode(dxl_id_2, 0, &log);
  if (result1 == false || result2 == false){
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
  else{
    Serial.println("Succeed to change wheel mode"); 
  }
}

void calcSpeed(float filtered, long current, long last){
  float momentarySpeed = filtered *(current-last)/1000; //    m/s^2 *ms/1000
  velocity += momentarySpeed;
}
void calcPosition(float velocityx, long current, long last){
  float xdelta = velocityx *(current-last)/1000*100;
  xpostion += xdelta;
}

void loop()
{
  static uint32_t tTime[3];
  static uint32_t imu_time = 0;
  static uint32_t filt_time = 0;
  
  tTime[1]=micros();
  if( IMU.update() > 0 ){
    imu_time = micros()-tTime[1];
  }
  if( (millis()-tTime[0]) >= 10 ){
    currTime = millis();
    tTime[0] = millis();
    if(millis()<7000){
      accFilt = 0.0;
      accConv = 0.0;
    }else{
      accConv = (IMU.accRaw[0]); //ACC X 
    }
    accFilt=a[0]*accFilt +a[1]*accFilt1 +a[2]*accFilt2 +a[3]*accFilt3 + a[4]*accFilt4 +a[5]*accFilt5 + a[6]*accFilt6 + a[7]*accFilt7 +
            b[0]*accConv +b[1]*accConv1 +b[2]*accConv2 +b[3]*accConv3 + b[4]*accConv4 + b[5]*accConv5 + b[6]*accConv6 + b[7]*accConv7;
    accConv1=accConv;
    accConv2=accConv1;
    accConv3=accConv2;
    accConv4=accConv3;
    accConv5=accConv4;
    accConv6=accConv5;
    accConv7=accConv6;
    accFilt1=accFilt;
    accFilt2=accFilt1;
    accFilt3=accFilt2;
    accFilt4=accFilt3;
    accFilt5=accFilt4;
    accFilt5=accFilt5;
    accFilt7=accFilt6;
    accFilt = accFilt/1670.8137;  //converted to m/s^2
    accConv = accConv/1670.8137;

    if(accFilt > -0.05 && accFilt < 0.05){ 
      accFilt = 0.0;
    }
    
    calcSpeed(accFilt, currTime, lastTime);
    calcPosition(velocity, currTime, lastTime);
    Serial.print(currTime);
    Serial.print(" \t");
    Serial.print(accFilt);
    Serial.print(" \t");
    Serial.print(xpostion);
    Serial.print(" \t");
    Serial.print(velocity);
    Serial.print(" \t");
    Serial.println(IMU.rpy[2]);
    
    filt_time = millis()-tTime[2];
    tTime[2]=millis();
    lastTime = currTime;
    
    if (Serial.available()){
    String read_string = Serial.readStringUntil('\n');
    Serial.println("[CMD] : " + String(read_string));
  
    read_string.trim();
    split(read_string, ' ', cmd);

    if (cmd[0] == "wheel"){
      speed1 = cmd[1].toInt();
      speed2 = cmd[2].toInt();
    }else if(cmd[0] == "1"){
      IMU.SEN.acc_cali_start();
      while( IMU.SEN.acc_cali_get_done() == false ){
        IMU.update();
    }
    }
    dxl_wb.goalVelocity(dxl_id_1, (int32_t)speed1);
    dxl_wb.goalVelocity(dxl_id_2, (int32_t)speed2);
  }
}
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
