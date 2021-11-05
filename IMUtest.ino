/*.
  Range   : +/- 2 g
  Scale   : 16384 = 1 g = 9.806m/s^2 <=> 1m/s^2 = 1670.8137
 */

#include <IMU.h>

cIMU    IMU;

float accSum=0;
float accFilt=0;
int i=0;
  
void setup()
{
  Serial.begin(57600);
  IMU.begin();

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
  if( (millis()-tTime[0]) >= 5 ){
    tTime[0] = millis();
    if(i<1){
      accSum = accSum + IMU.accRaw[0];        // ACC X
      i++;
    }else{
      accFilt = (accSum/10.0 + 10.0)/1670.8137; //tested offset while stationary and converted to m/s^2
      i=0;
      accSum=0;
      filt_time = millis()-tTime[2];
      /*Serial.print(imu_time);
      Serial.print(" \t");
      Serial.print(filt_time);
      Serial.print(" \t");*/
      Serial.println(accFilt);
      tTime[2]=millis();
    }
  }
}
