/**
 * @file continuousMeasurement.ino
 * @brief This example demonstrates the continuous measurement function of LIDAR07, ranging from 0.2m to 12m(operating voltage :5 V)
 * @n Connection rules: PIN1-PIN8 are in the front of the sensor from right to left. 
 * @n          PIN1----------------------SCL--------------------Maincontroller SCL（IIC mode）
 * @n          PIN2----------------------SDA--------------------Maincontroller SDA（IIC mode）
 * @n          PIN3----------------------Light source power supply ground--------------Maincontroller GND
 * @n          PIN4----------------------Light source power supply(5V)--------Maincontroller VCC
 * @n          PIN5----------------------TX---------------------Maincontroller RX pin, which is set to be used for serial communication with the sensor (UART mode)
 * @n          PIN6----------------------RX---------------------Maincontroller TX pin, which is set to be used for serial communication with the sensor (UART mode)
 * @n          PIN7----------------------Module main power supply ground------------Maincontroller GND
 * @n          PIN8----------------------Module main power supply(5V)------Maincontroller VCC
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2022-06-20
 * @url https://github.com/DFRobot/DFRobot_LIDAR07
 */
#include"DFRobot_LIDAR07.h"

//If using IIC mode, please enable macro USE_IIC
#define USE_IIC
#ifdef USE_IIC  
DFRobot_LIDAR07_IIC  LIDAR07;
#endif

//If using UART mode, please enable macro USE_UART. The USE_UART is enabled by default. The two modes USE_IIC and USE_UART can’t be used at the same time.
//#define USE_UART
#ifdef USE_UART

#if defined(ESP8266)||defined(ARDUINO_AVR_UNO)
#include <SoftwareSerial.h>
SoftwareSerial  mySerial(4,12);//GPIO4 is corresponding to RX on main control board, GPIO12 is corresponding to TX on main control board
/**!
 * The TX of esp32 Serial1 is GPIO10, and the RX is GPIO9 
 * The TX of mega2560 Serial1 is GPIO18, and the RX is GPIO19
 * The TX of M0 Serial1 is GPIO1, and the RX is GPIO0
 * The TX of leonardo Serial1 is GPIO1, and the RX is GPIO0
 */
#endif
DFRobot_LIDAR07_UART  LIDAR07;  
#endif

void setup() {
  uint32_t version;
  Serial.begin(115200);
#ifdef USE_IIC
  while(!LIDAR07.begin()){
    Serial.println("The sensor returned data validation error");
    delay(1000);
  }
#endif

#ifdef USE_UART
#if defined(ESP8266)||defined(ARDUINO_AVR_UNO)
  mySerial.begin(115200);
  while(!LIDAR07.begin(mySerial)){
    Serial.println("The sensor returned data validation error");
    delay(1000);
  }
#else
  Serial1.begin(115200);
  while(!LIDAR07.begin(Serial1)){
    Serial.println("The sensor returned data validation error");
    delay(1000);
  }
#endif
#endif

  version = LIDAR07.getVersion();
  Serial.print("VERSION: ");
  Serial.print((version>>24)&0xFF,HEX);
  Serial.print(".");Serial.print((version>>16)&0xFF,HEX);
  Serial.print(".");Serial.print((version>>8)&0xFF,HEX);
  Serial.print(".");Serial.println((version)&0xFF,HEX);

  //After enabling the filter, it can be stopped by calling LIDAR07.stopFilter()
  while(!LIDAR07.startFilter());

  /**
   * @brief  Configure the sensor to single acquisition mode
   * @param  mode The way data are collected
   * @n      eLidar07Single  A single collection
   * @n      eLidar07Continuous  Continuous acquisition
   * @return true (Successful) ， false (Failed)
   */
  while(!LIDAR07.setMeasureMode(LIDAR07.eLidar07Continuous));

  /**
   * @brief  Set specific measurement interval period
   * @param  frqe This parameter is valid only for continuous measurement, and the measurement period is set in ms. Minimum setting is 10MS (i.e. 100Hz)
   * @return true (Successful) ， false (Failed)
   */
  while(!LIDAR07.setConMeasureFreq(100));
  
  //Open measurement (in single measurement mode, it will automatically close after sampling).To stop collection, use stopMeasure()
  LIDAR07.startMeasure();
}

void loop() {

  //Get the collected data
  if(LIDAR07.getValue()){
    Serial.print("Distance:");Serial.print(LIDAR07.getDistanceMM());Serial.println(" mm");
    Serial.print("Amplitude:");Serial.println(LIDAR07.getSignalAmplitude());
  }
}