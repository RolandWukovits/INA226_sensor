// Hi, this is not complete code. It is just to show how to setup and integrate the current sensor into your sketch!


// I2C
#include <Wire.h>
// INA226 shunt sensor by Wolfgang Ewald
#include <INA226_WE.h>

#define I2C_ADDRESS 0x40
INA226_WE ina226 = INA226_WE(I2C_ADDRESS); 

float shuntVolt;
int shuntMaxmV=75;                       // enter your Shunt mV rating
int shuntMaxAmps=100;                    // enter your Shunt current rating in A
float current;
float holdcurrent;
float lastcurrent; 
float lastAhigh;
float lastAlow;
unsigned long Aloopstartmillis;
unsigned long Aloopinterval=650;         // interval of loop in mSec  

// Moving Average current
const int AnumReadings=7;          // how many values are part of MA
float Areadings[AnumReadings];      
int AreadIndex=0;              
float total;   

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
 //INA226
  ina226.init();
  ina226.setAverage(AVERAGE_64); // choose how many samples to take each measurement (1,4,16,64,128,256,512,1024)
  ina226.setMeasureMode(TRIGGERED);   // Triggered measurements
  ina226.setCurrentRange(MA_400);  //choose gain according your shunt voltage (400,800) mA max
  ina226.setCorrectionFactor(1.47); // set correction factor to compensate filter loss according other amp-meter

  Aloopstartmillis=millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:

// current loop
 if (millis()>=(Aloopstartmillis+Aloopinterval)){
  Aloopstartmillis=millis();
  getCurrent();
  Serial.println(current);
 }


}


void getCurrent(){

  shuntVolt=0;

  ina226.startSingleMeasurement();
  ina226.readAndClearFlags();
  shuntVolt = ina226.getShuntVoltage_mV();

  holdcurrent=(shuntVolt/shuntMaxmV)*shuntMaxAmps; 


   // Moving Average
   total = total-Areadings[AreadIndex];
   Areadings[AreadIndex]=current2;
   total = total+Areadings[AreadIndex];
   AreadIndex++;

   if (AreadIndex>=AnumReadings){
     AreadIndex = 0;
   }

    current = total/AnumReadings;
    
    current=current*100;                   // round to 2 decimals
    current=round(current);
    current=current/100;
 
}
