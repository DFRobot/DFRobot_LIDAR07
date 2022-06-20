/*!
 * @file DFRobot_LIDAR07.h
 * @brief Define the infrastructure for the DFRobot_LIDAR07 class
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @SKU SEN0427
 * @licence The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version V1.0
 * @date 2021-04-16
 * @url  https://github.com/DFRobot/DFRobot_LIDAR07
 */
#ifndef __DFROBOT_LIDAR07_H__
#define __DFROBOT_LIDAR07_H__
#include <Arduino.h>
#include <Wire.h>
//Enable this macro, we can see the operation of this program in detail
 
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define LIDAR07_IIC      0
#define LIDAR07_UART     1
class DFROBOT_LIDAR07{
public: 
  #define LIDAR07_VERSION  0
  #define LIDAR07_MEASURE  1
  typedef enum{
    eLidar07Single = 0,   /**< A single collection*/
    eLidar07Continuous = 1,     /**< Continuous acquisition*/
  }eLIDAR07CollectMode_t;

  typedef struct packet{
    uint8_t head;
    uint8_t command;
    uint8_t data[4];
    uint8_t checkData[4];
  }sSendPacket_t;

  /**
   * @brief  Constructor, initialize some variables of super class and create space for all sorts of data packets 
   */
  DFROBOT_LIDAR07(uint8_t type);

  /**
   * @brief  Destructor, release the created space of the various data packets of supperclass  
   */
  ~DFROBOT_LIDAR07();
  /**
   * @brief  Detect whether the sensor is in normal communication and configure the sensor to single acquisition mode
   * @return true Indicate communication and sensor operating mode setting is successful，false Indicate communication is failed or get wrong information 
   */
  bool begin();

  /**
   * @brief Get the version information of the sensor
   * @return Return 32-bit data, low 16 bits are the minor version number, high 16 bits are the major version number
   */
  uint32_t getVersion();

  /**
   * @brief  Start measurement
   */
  void startMeasure();
  
  /**
   * @brief  Get the collected data
   * @return true (Successful) ， false (Failed)
   */
  bool getValue();

  /**
   * @brief  Set specific measurement interval period
   * @param  frqe This parameter is valid only for continuous measurement, and the measurement period is set in ms. Minimum setting is 10MS (i.e. 100Hz)
   * @return true (Successful) ， false (Failed)
   */
  bool setConMeasureFreq(uint32_t frqe);

  /**
   * @brief  Set the sensor working mode
   * @param  mode The way data are collected
   * @n      eLidar07Single  A single collection
   * @n      eLidar07Continuous  Continuous acquisition
   * @return true (Successful) ， false (Failed)
   */
  bool setMeasureMode(eLIDAR07CollectMode_t mode);

  /**
   * @brief  Get the measured distance data, range: 0.2m-12m
   * @return Distance, unit: mm
   */
  uint16_t getDistanceMM();

  /**
   * @brief  Get signal amplitude
   * @return signal amplitude
   */
  uint16_t getSignalAmplitude();

  /**
   * @brief  Enable filter function, this sensor has a lightweight filtering function inside
   * @return true (successful), false (failed)
   */
  bool startFilter();

  /**
   * @brief  Disable filter, this sensor has a lightweight filtering function inside
   * @return true (successful), false (failed)
   */
  bool stopFilter();

protected:

  /**
   * @brief  Stop measurement, after running this command, the sensor will stop working. And next time we use this sensor, we need to power it on again.
   */
  void stopMeasure();

  /**
   * @brief  Fetch the data we need from buff according to type
   * @param  type The data to be fetched  LIDAR07_VERSION：version number of the sensor LIDAR07_MEASURE： measurement data
   */
  void readValue(uint8_t *buff,uint8_t type);

  /**
   * @brief  Write commands to the sensor, here is a pure virtual function, implemented by a subclass
   * @param  buff Pointer to command
   * @param  len    Address length
   */
  virtual void write(void* pBuf, size_t size) = 0;

  /**
   * @brief  Read sensor data
   * @param  buff Pointer to data cache
   * @param  len The bytes of data to be read
   * @param  reg Address of the register
   * @return CRC check value of data packets
   */
  virtual uint32_t read(void* pBuf, size_t size,uint8_t reg) = 0;

protected:
  uint16_t distance;
  uint16_t amplitude;
  uint32_t version;
  uint8_t _type;
  sSendPacket_t *readVersionPacket;
  sSendPacket_t *setIntervalPacket;
  sSendPacket_t *setModePacket;
  sSendPacket_t *startPacket;
  sSendPacket_t *stopPacket;
  sSendPacket_t *startFilterPacket;
  sSendPacket_t *stopFilterPacket;
  sSendPacket_t *setFreq;
};

class DFROBOT_LIDAR07_IIC:public DFROBOT_LIDAR07 {
public:
  #define LIDAR07_IIC_ADDR 0x70
  /**
   * @brief  Constructor, pass parameters to the super class, way to enable supperclass -- crc32Check()、readValue – choose the specific IIC branch.
   * @param  wire  Reference to TwoWire
   */
  DFROBOT_LIDAR07_IIC(TwoWire &wire = Wire);
  ~DFROBOT_LIDAR07_IIC(){};

  /**
   * @brief  Assignment of various command packages and initialization of IIC, and call the begin() of the parent class to detect whether the sensor is in normal 
   * @n       communication and configure the sensor to be in single acquisition mode
   * @return true means communication and sensor operating mode setting is successful, false means communication failed or get wrong information 
   */
  bool begin();
private:
  /**
   * @brief  Write command to the sensor
   * @param  buff Pointer to command
   * @param  len Address length
   */
  void write(void* pBuf, size_t size);

  /**
   * @brief  Read sensor data
   * @param  buff Pointer to data cache
   * @param  len The bytes of data to be read
   * @param  reg Address of the register
   * @return CRC check value of data packets
   */
  uint32_t read(void* pBuf, size_t size,uint8_t reg);
  TwoWire * _pWire = NULL;
  uint8_t _deviceAddr;
};

class DFROBOT_LIDAR07_UART:public DFROBOT_LIDAR07 {
public:
  /**
   * @brief    Constructor, pass parameters to the super class, way to enable supperclass -- crc32Check()、readValue – choose the specific UART branch.
   */
  DFROBOT_LIDAR07_UART();
  ~DFROBOT_LIDAR07_UART(){};

  /**
   * @brief Assignment of various command packages, and call the begin() of the parent class to detect whether the sensor is in normal communication
   * @n     and configure the sensor to be in single acquisition mode  
   * @param  s A reference to Stream class
   * @return true means communication and sensor operating mode setting is successful，false means communication failed or get wrong information 
   */
  bool begin(Stream &s);
private:
  /**
   * @brief  Write command to the sensor
   * @param  buff Pointer to command
   * @param  len Address length
   */
  void write(void* pBuf, size_t size);

  /**
   * @brief  Read sensor data
   * @param  buff Pointer to data cache
   * @param  len The bytes of data to be read
   * @param  reg Address of the register
   * @return CRC check value of data packets
   */
  uint32_t read(void* pBuf, size_t size,uint8_t reg);
  Stream * _s = NULL;
};
#endif

