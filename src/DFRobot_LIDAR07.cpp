/**
 * @file DFRobot_LIDAR07.cpp
 * @brief Implementation of DFRobot_LIDAR07 class
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-04-16
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_LIDAR07
 */
#include "DFRobot_LIDAR07.h"
#include "dfrobot_crc.h"
/* CRC余式表 */


DFRobot_LIDAR07 :: DFRobot_LIDAR07(uint8_t type)
{
  distance = 0;
  amplitude = 0;
  version = 0;

  readVersionPacket = new sSendPacket_t;
  setIntervalPacket = new sSendPacket_t;
  setModePacket = new sSendPacket_t;
  startPacket = new sSendPacket_t;
  stopPacket = new sSendPacket_t;
  startFilterPacket = new sSendPacket_t;
  stopFilterPacket = new sSendPacket_t;
  setFreq = new sSendPacket_t;
  if(type == 0){
    _type = LIDAR07_IIC;
  } else{
    _type = LIDAR07_UART;
  }
}

DFRobot_LIDAR07 ::~DFRobot_LIDAR07()
{
  delete readVersionPacket;
  delete setIntervalPacket;
  delete setModePacket;
  delete startPacket; 
  delete stopPacket;
  delete startFilterPacket;
  delete stopFilterPacket;
  delete setFreq;
  readVersionPacket = NULL;
  setIntervalPacket = NULL;
  setModePacket = NULL;
  startPacket = NULL;
  stopPacket = NULL;
  startFilterPacket = NULL;
  stopFilterPacket = NULL; 
  setFreq= NULL;
}

bool DFRobot_LIDAR07:: begin()
{
  bool ret = false;
  uint8_t buff[12]={0};
  read((void *)buff,12,0);//这里是为了清除串口缓存
  setMeasureMode(eLidar07Single);
  read((void *)buff,12,0);//这里是为了清除串口缓存
  write((void *)readVersionPacket,sizeof(sSendPacket_t));
  delay(20);
  uint32_t crc=read((void *)buff,12,readVersionPacket->command);
  uint32_t checkData = (uint32_t)buff[8] | ((uint32_t)buff[9]<<8) | ((uint32_t)buff[10]<<16) | ((uint32_t)buff[11]<<24);
  if(crc==checkData){
      readValue(buff,LIDAR07_VERSION);
      ret = true;
  }
  return  ret;
}

bool DFRobot_LIDAR07:: setMeasureMode(eLIDAR07CollectMode_t mode)
{
  uint8_t buff[12];
  bool ret = false;
  if(mode==eLidar07Continuous){
      setModePacket->data[0]=0x01;
      uint32_t modeCrcData=0;
      if(_type == LIDAR07_UART){
        modeCrcData=doCrc32MPEG2Calculate((uint8_t*)setModePacket,6);
      }else{
        modeCrcData=doCrc32MPEG2Calculate((uint8_t*)(&(setModePacket->command)),5);
      }
      setModePacket->checkData[0]=modeCrcData &0xFF;
      setModePacket->checkData[1]=(modeCrcData>>8) &0xFF;
      setModePacket->checkData[2]=(modeCrcData>>16) &0xFF;
      setModePacket->checkData[3]=(modeCrcData>>24) &0xFF;
  }
  write((void *)setModePacket,sizeof(sSendPacket_t));
  delay(20);
  uint32_t crc=read((void *)buff,12,setModePacket->command);
  uint32_t checkData = (uint32_t)buff[8] | ((uint32_t)buff[9]<<8) | ((uint32_t)buff[10]<<16) | ((uint32_t)buff[11]<<24);
  if(crc==checkData){
      ret = true;
  }
  return ret;
}
bool DFRobot_LIDAR07::setConMeasureFreq(uint32_t frqe)
{
  uint8_t buff[12];
  bool ret = false;
  uint32_t freqCrcData=0;
  setFreq->data[0]=frqe & 0xFF;
  setFreq->data[1]=(frqe>>8) & 0xFF;
  setFreq->data[2]=(frqe>>16) & 0xFF;
  setFreq->data[3]=(frqe>>24) & 0xFF;
  if(_type == LIDAR07_UART){
    freqCrcData=doCrc32MPEG2Calculate((uint8_t*)setFreq,6);
  }else{
    freqCrcData=doCrc32MPEG2Calculate((uint8_t*)(&(setFreq->command)),5);
  }
  setFreq->checkData[0]=freqCrcData &0xFF;
  setFreq->checkData[1]=(freqCrcData>>8) &0xFF;
  setFreq->checkData[2]=(freqCrcData>>16) &0xFF;
  setFreq->checkData[3]=(freqCrcData>>24) &0xFF;
  write((void *)setFreq,sizeof(sSendPacket_t));
  delay(20);
  uint32_t crc=read((void *)buff,12,setFreq->command);
  uint32_t checkData = (uint32_t)buff[8] | ((uint32_t)buff[9]<<8) | ((uint32_t)buff[10]<<16) | ((uint32_t)buff[11]<<24);
  if(crc==checkData){
      ret = true;
  }
  return ret;
}

void DFRobot_LIDAR07::startMeasure()
{
  write((void *)startPacket,sizeof(sSendPacket_t));
  delay(20);
}

uint16_t DFRobot_LIDAR07::getDistanceMM()
{
  return distance;
}

uint16_t DFRobot_LIDAR07::getSignalAmplitude()
{
  return amplitude;
}

uint32_t DFRobot_LIDAR07:: getVersion()
{
  return version;
}

void DFRobot_LIDAR07::stopMeasure()
{
  write((void *)stopPacket,sizeof(sSendPacket_t));
  uint8_t buf[12];
  read((void *)buf,12,0);
}

bool DFRobot_LIDAR07::startFilter()
{
  uint8_t buff[12];
  bool ret = false;
  write((void *)startFilterPacket,sizeof(sSendPacket_t));
  delay(20);
  uint32_t crc=read((void *)buff,12,startFilterPacket->command);
  uint32_t checkData = (uint32_t)buff[8] | ((uint32_t)buff[9]<<8) | ((uint32_t)buff[10]<<16) | ((uint32_t)buff[11]<<24);
  if(crc==checkData){
      ret = true;
  }
  return ret;
}

bool DFRobot_LIDAR07::stopFilter()
{
  uint8_t buff[12];
  bool ret = false;
  write((void *)stopFilterPacket,sizeof(sSendPacket_t));
  delay(20);
  uint32_t crc=read((void *)buff,12,stopFilterPacket->command);
  uint32_t checkData = (uint32_t)buff[8] | ((uint32_t)buff[9]<<8) | ((uint32_t)buff[10]<<16) | ((uint32_t)buff[11]<<24);
  if(crc==checkData){
      ret = true;
  }
  return ret;
}

void DFRobot_LIDAR07::readValue(uint8_t *buff,uint8_t type)
{

  if(type == LIDAR07_VERSION){
    version = (uint32_t)buff[4] | ((uint32_t)buff[5]<<8) | ((uint32_t)buff[6]<<16) | ((uint32_t)buff[7]<<24);
  } else if(type == LIDAR07_MEASURE){
    distance = (uint16_t)buff[4] | ((uint16_t)buff[5]<<8);
    amplitude = (uint16_t)buff[8] | ((uint16_t)buff[9]<<8);
  }

}

DFRobot_LIDAR07_UART::DFRobot_LIDAR07_UART():DFRobot_LIDAR07(LIDAR07_UART)
{
}

bool DFRobot_LIDAR07_UART ::begin(Stream &s)
{
  readVersionPacket->head = 0xF5;
  readVersionPacket->command = 0x43;
  readVersionPacket->data[0] = 0x00;
  readVersionPacket->data[1] = 0x00;
  readVersionPacket->data[2] = 0x00;
  readVersionPacket->data[3] = 0x00;
  readVersionPacket->checkData[0] = 0xAC;
  readVersionPacket->checkData[1] = 0x45;
  readVersionPacket->checkData[2] = 0x62;
  readVersionPacket->checkData[3] = 0x3B;

  setModePacket->head = 0xF5;
  setModePacket->command = 0xE1;
  setModePacket->data[0] = 0x00;
  setModePacket->data[1] = 0x00;
  setModePacket->data[2] = 0x00;
  setModePacket->data[3] = 0x00;
  setModePacket->checkData[0] = 0xA5;
  setModePacket->checkData[1] = 0x8D;
  setModePacket->checkData[2] = 0x89;
  setModePacket->checkData[3] = 0xA7;
  
  startPacket->head = 0xF5;
  startPacket->command = 0xE0;
  startPacket->data[0] = 0x01;
  startPacket->data[1] = 0x00;
  startPacket->data[2] = 0x00;
  startPacket->data[3] = 0x00;
  startPacket->checkData[0] = 0x9F;
  startPacket->checkData[1] = 0x70;
  startPacket->checkData[2] = 0xE9;
  startPacket->checkData[3] = 0x32;

  stopPacket->head = 0xF5;
  stopPacket->command = 0xE0;
  stopPacket->data[0] = 0x00;
  stopPacket->data[1] = 0x00;
  stopPacket->data[2] = 0x00;
  stopPacket->data[3] = 0x00;
  stopPacket->checkData[0] = 0x28;
  stopPacket->checkData[1] = 0xEA;
  stopPacket->checkData[2] = 0x84;
  stopPacket->checkData[3] = 0xEE;

  startFilterPacket->head = 0xF5;
  startFilterPacket->command = 0xD9;
  startFilterPacket->data[0] = 0x01;
  startFilterPacket->data[1] = 0x00;
  startFilterPacket->data[2] = 0x00;
  startFilterPacket->data[3] = 0x00;
  startFilterPacket->checkData[0] = 0xB7;
  startFilterPacket->checkData[1] = 0x1F;
  startFilterPacket->checkData[2] = 0xBA;
  startFilterPacket->checkData[3] = 0xBA;
   
  stopFilterPacket->head = 0xF5;
  stopFilterPacket->command = 0xD9;
  stopFilterPacket->data[0] = 0x00;
  stopFilterPacket->data[1] = 0x00;
  stopFilterPacket->data[2] = 0x00;
  stopFilterPacket->data[3] = 0x00;
  stopFilterPacket->checkData[0] = 0x00;
  stopFilterPacket->checkData[1] = 0x85;
  stopFilterPacket->checkData[2] = 0xD7;
  stopFilterPacket->checkData[3] = 0x66;

  setFreq->head = 0xF5;
  setFreq->command = 0xE2;
  setFreq->data[0] = 0x00;
  setFreq->data[1] = 0x00;
  setFreq->data[2] = 0x00;
  setFreq->data[3] = 0x00;
  setFreq->checkData[0] = 0x00;
  setFreq->checkData[1] = 0x00;
  setFreq->checkData[2] = 0x00;
  setFreq->checkData[3] = 0x00;

  _s = &s;
  return DFRobot_LIDAR07:: begin();
}


void DFRobot_LIDAR07_UART:: write(void* pBuf, size_t size)
{
  uint8_t * _pBuf = (uint8_t *)pBuf;
  this->_s->write((const uint8_t *)_pBuf,size);
}

bool DFRobot_LIDAR07:: getValue()
{
  uint8_t buff[24];
  uint32_t checkData,crc;
  bool ret = false;
  uint8_t len =20;
  crc=read((void *)buff,24,startPacket->command);
  checkData = (uint32_t)buff[len] | ((uint32_t)buff[len+1]<<8) | ((uint32_t)buff[len+2]<<16) | ((uint32_t)buff[len+3]<<24);
  if(crc==checkData){
      readValue(buff,LIDAR07_MEASURE);
      ret = true;
  }
  return ret;
}

uint32_t DFRobot_LIDAR07_UART:: read(void* pBuf, size_t size,uint8_t reg)
{
  uint8_t count=0;
    uint8_t data;
  uint8_t * _pBuf = (uint8_t *)pBuf;
  int len=size;
  uint32_t crc=0xFFFFFFFF;
  if(reg==0){
        while(this->_s->available()){
            this->_s->read();
        }
        return crc;
  }
//   while(!this->_s->available()){
//     yield();
//   }
  while(this->_s->available()){
      uint8_t temp=(uint8_t)this->_s->read();
      if(temp==0xFA){
          crc= 0xFFFFFFFF;
          *_pBuf=temp;
          crc = (crc << 8) ^ crc32MPEG2Table[(crc >> 24 ^ *_pBuf) & 0xff];
          _pBuf++;
          len--;
          while(len>0){
            if(this->_s->available()){
                *_pBuf=(uint8_t)this->_s->read();
                if(*_pBuf!=reg && (len ==(size -1))){
                    return 0;
                }
                if(len>4){
                    crc = (crc << 8) ^ crc32MPEG2Table[(crc >> 24 ^ *_pBuf) & 0xff];
                }
                len--;
                _pBuf++;
            }
          }
          break;
      }
  }
  
  return crc;
}

DFRobot_LIDAR07_IIC::DFRobot_LIDAR07_IIC(TwoWire &wire):DFRobot_LIDAR07(LIDAR07_IIC)
{
  _deviceAddr = LIDAR07_IIC_ADDR;
  _pWire = &wire;
}

bool DFRobot_LIDAR07_IIC ::begin()
{
  readVersionPacket->head = 0x00;
  readVersionPacket->command = 0x43;
  readVersionPacket->data[0] = 0x00;
  readVersionPacket->data[1] = 0x00;
  readVersionPacket->data[2] = 0x00;
  readVersionPacket->data[3] = 0x00;
  readVersionPacket->checkData[0] = 0x55;
  readVersionPacket->checkData[1] = 0x10;
  readVersionPacket->checkData[2] = 0xCD;
  readVersionPacket->checkData[3] = 0x9A;
   
  setModePacket->head = 0x00;
  setModePacket->command = 0xE1;
  setModePacket->data[0] = 0x00;
  setModePacket->data[1] = 0x00;
  setModePacket->data[2] = 0x00;
  setModePacket->data[3] = 0x00;
  setModePacket->checkData[0] = 0x5C;
  setModePacket->checkData[1] = 0xD8;
  setModePacket->checkData[2] = 0x26;
  setModePacket->checkData[3] = 0x06;

  startPacket->head = 0x00;
  startPacket->command = 0xE0;
  startPacket->data[0] = 0x01;
  startPacket->data[1] = 0x00;
  startPacket->data[2] = 0x00;
  startPacket->data[3] = 0x00;
  startPacket->checkData[0] = 0x66;
  startPacket->checkData[1] = 0x25;
  startPacket->checkData[2] = 0x46;
  startPacket->checkData[3] = 0x93;

  stopPacket->head = 0x00;
  stopPacket->command = 0xE0;
  stopPacket->data[0] = 0x00;
  stopPacket->data[1] = 0x00;
  stopPacket->data[2] = 0x00;
  stopPacket->data[3] = 0x00;
  stopPacket->checkData[0] = 0xD1;
  stopPacket->checkData[1] = 0xBF;
  stopPacket->checkData[2] = 0x2B;
  stopPacket->checkData[3] = 0x4F;

  startFilterPacket->head = 0x00;
  startFilterPacket->command = 0xD9;
  startFilterPacket->data[0] = 0x01;
  startFilterPacket->data[1] = 0x00;
  startFilterPacket->data[2] = 0x00;
  startFilterPacket->data[3] = 0x00;
  startFilterPacket->checkData[0] = 0x4E;
  startFilterPacket->checkData[1] = 0x4A;
  startFilterPacket->checkData[2] = 0x15;
  startFilterPacket->checkData[3] = 0x1B;
   
  stopFilterPacket->head = 0x00;
  stopFilterPacket->command = 0xD9;
  stopFilterPacket->data[0] = 0x00;
  stopFilterPacket->data[1] = 0x00;
  stopFilterPacket->data[2] = 0x00;
  stopFilterPacket->data[3] = 0x00;
  stopFilterPacket->checkData[0] = 0xF9;
  stopFilterPacket->checkData[1] = 0xD0;
  stopFilterPacket->checkData[2] = 0x78;
  stopFilterPacket->checkData[3] = 0xC7;

  setFreq->head = 0x00;
  setFreq->command = 0xE2;
  setFreq->data[0] = 0x00;
  setFreq->data[1] = 0x00;
  setFreq->data[2] = 0x00;
  setFreq->data[3] = 0x00;
  setFreq->checkData[0] = 0x00;
  setFreq->checkData[1] = 0x00;
  setFreq->checkData[2] = 0x00;
  setFreq->checkData[3] = 0x00;

  _pWire->begin();
  return DFRobot_LIDAR07:: begin();
}

void DFRobot_LIDAR07_IIC::write(void* pBuf, size_t size)
{
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  while(_pWire->available()){
    _pWire->read();
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  for(uint16_t i = 1; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint32_t DFRobot_LIDAR07_IIC::read(void* pBuf, size_t size,uint8_t reg)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  if(reg==0){
      while(_pWire->available()){
        _pWire->read();
      }
      return 0xFFFFFFFF;
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  uint32_t crc=0xFFFFFFFF;
  uint8_t firstData = 0xFA;
  size_t len = size-4;
  _pWire->requestFrom(_deviceAddr, (uint8_t) size-1);
  crc = (crc << 8) ^ crc32MPEG2Table[(crc >> 24 ^ firstData) & 0xff];
  _pBuf[0]=0xFA; 
  for(uint16_t i = 1; i < size; i++){
    _pBuf[i] = _pWire->read();
    if(len>i){
        crc = (crc << 8) ^ crc32MPEG2Table[(crc >> 24 ^ _pBuf[i]) & 0xff];
    }
  }
  return crc;
}

