/*!
 * @file DFRobot_LIDAR07.h
 * @brief Define the infrastructure for the DFRobot_LIDAR07 class
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version V1.0
 * @date 2021-04-16
 * @url  https://github.com/DFRobot/DFRobot_LIDAR07
 */
#ifndef __DFRobot_LIDAR07_H__
#define __DFRobot_LIDAR07_H__
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
class DFRobot_LIDAR07{
public: 
  #define LIDAR07_VERSION  0
  #define LIDAR07_MEASURE  1

/**
 * @enum eLIDAR07CollectMode_t
 * @brief Set the sensor working mode
 */
  typedef enum{
    eLidar07Single = 0,         /**< A single collection*/
    eLidar07Continuous = 1,     /**< Continuous acquisition*/
  }eLIDAR07CollectMode_t;

/**
 * @struct sSendPacket_t
 * @brief Used to transmit data
 */
  typedef struct packet{
    uint8_t head;
    uint8_t command;
    uint8_t data[4];
    uint8_t checkData[4];
  }sSendPacket_t;

  /**
   * @fn DFRobot_LIDAR07
   * @brief  Constructor, initialize some variables of super class and create space for all sorts of data packets 
   */
  DFRobot_LIDAR07(uint8_t type);

  /**
   * @fn ~DFRobot_LIDAR07
   * @brief  Destructor, release the created space of the various data packets of supperclass  
   */
  ~DFRobot_LIDAR07();

  /**
   * @fn begin
   * @brief  Detect whether the sensor is in normal communication and configure the sensor to single acquisition mode
   * @return true Indicate communication and sensor operating mode setting is successful，false Indicate communication is failed or get wrong information 
   */
  bool begin();

  /**
   * @fn getVersion
   * @brief Get the version information of the sensor
   * @return Return 32-bit data, low 16 bits are the minor version number, high 16 bits are the major version number
   */
  uint32_t getVersion();

  /**
   * @fn startMeasure
   * @brief  Start measurement
   */
  void startMeasure();
  
  /**
   * @fn getValue
   * @brief  Get the collected data
   * @return Whether the correct data has been obtained
   * @retval true (Successful)
   * @retval false (Failed)
   */
  bool getValue();

  /**
   * @fn setConMeasureFreq
   * @brief  Set specific measurement interval period
   * @param  frqe This parameter is valid only for continuous measurement, and the measurement period is set in ms. Minimum setting is 10MS (i.e. 100Hz)
   * @return Whether the frequency is set successfully
   * @retval true (Successful) 
   * @retval false (Failed)
   */
  bool setConMeasureFreq(uint32_t frqe);

  /**
   * @fn setMeasureMode
   * @brief  Set the sensor working mode
   * @param  mode The way data are collected
   * @n      eLidar07Single  A single collection
   * @n      eLidar07Continuous  Continuous acquisition
   * @return Check whether the working mode is set successfully 
   * @retval true (Successful) 
   * @retval false (Failed)
   */
  bool setMeasureMode(eLIDAR07CollectMode_t mode);

  /**
   * @fn getDistanceMM
   * @brief  Get the measured distance data, range: 0.2m-12m
   * @return Distance, unit: mm
   */
  uint16_t getDistanceMM();

  /**
   * @fn getSignalAmplitude
   * @brief  Get signal amplitude
   * @return signal amplitude
   */
  uint16_t getSignalAmplitude();

  /**
   * @fn startFilter
   * @brief  Enable filter function, this sensor has a lightweight filtering function inside
   * @return Whether the filter was set successfully
   * @retval true (successful)
   * @retval false (failed)
   */
  bool startFilter();

  /**
   * @fn stopFilter
   * @brief  Disable filter, this sensor has a lightweight filtering function inside
   * @return Whether the filter is successfully closed
   * @retval true (successful)
   * @retval false (failed)
   */
  bool stopFilter();

protected:

  /**
   * @fn stopMeasure
   * @brief  Stop measurement, after running this command, the sensor will stop working. And next time we use this sensor, we need to power it on again.
   */
  void stopMeasure();

  /**
   * @fn readValue
   * @brief  Fetch the data we need from buff according to type
   * @param  type The data to be fetched  LIDAR07_VERSION：version number of the sensor LIDAR07_MEASURE： measurement data
   */
  void readValue(uint8_t *buff,uint8_t type);

  /**
   * @fn write
   * @brief  Write commands to the sensor, here is a pure virtual function, implemented by a subclass
   * @param  buff Pointer to command
   * @param  len    Address length
   */
  virtual void write(void* pBuf, size_t size) = 0;

  /**
   * @fn read
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

class DFRobot_LIDAR07_IIC:public DFRobot_LIDAR07 {
public:
  #define LIDAR07_IIC_ADDR 0x70
  /**
   * @fn DFRobot_LIDAR07_IIC
   * @brief  Constructor, pass parameters to the super class, way to enable supperclass -- crc32Check()、readValue – choose the specific IIC branch.
   * @param  wire  Reference to TwoWire
   */
  DFRobot_LIDAR07_IIC(TwoWire &wire = Wire);
  ~DFRobot_LIDAR07_IIC(){};

  /**
   * @fn begin
   * @brief  Assignment of various command packages and initialization of IIC, and call the begin() of the parent class to detect whether the sensor is in normal 
   * @n       communication and configure the sensor to be in single acquisition mode
   * @return true means communication and sensor operating mode setting is successful, false means communication failed or get wrong information 
   */
  bool begin();
private:
  /**
   * @fn write
   * @brief  Write command to the sensor
   * @param  buff Pointer to command
   * @param  len Address length
   */
  void write(void* pBuf, size_t size);

  /**
   * @fn read
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

class DFRobot_LIDAR07_UART:public DFRobot_LIDAR07 {
public:
  /**
   * @brief    Constructor, pass parameters to the super class, way to enable supperclass -- crc32Check()、readValue – choose the specific UART branch.
   */
  DFRobot_LIDAR07_UART();
  ~DFRobot_LIDAR07_UART(){};

  /**
   * @fn begin
   * @brief Assignment of various command packages, and call the begin() of the parent class to detect whether the sensor is in normal communication
   * @n     and configure the sensor to be in single acquisition mode  
   * @param  s A reference to Stream class
   * @return true means communication and sensor operating mode setting is successful，false means communication failed or get wrong information 
   */
  bool begin(Stream &s);
private:
  /**
   * @fn write
   * @brief  Write command to the sensor
   * @param  buff Pointer to command
   * @param  len Address length
   */
  void write(void* pBuf, size_t size);

  /**
   * @fn read
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

