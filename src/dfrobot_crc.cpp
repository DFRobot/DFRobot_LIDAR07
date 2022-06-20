#include "dfrobot_crc.h"
/*
CRC parameter model:
  Name : CRC-32/MPEG-2
  Width : 32
  Poly : 04C11DB7
  Init : FFFFFFFF
  RefIn : False
  RefOut : False
  XorOut : 00000000
*/
//Look-up table method ----CRC-32/MPEG-2
uint32_t doCrc32MPEG2Table(uint8_t *ptr, int len)
{
    uint32_t crc = 0xFFFFFFFF;

    while(len--){
        crc = (crc << 8) ^ crc32MPEG2Table[(crc >> 24 ^ *ptr++) & 0xff];
    }

    return crc;
}
//Direct calculation
uint32_t doCrc32MPEG2Calculate(uint8_t *ptr,uint8_t len)
{  
    uint8_t data;
    uint32_t crc = 0xFFFFFFFF;
    int i;

    for (; len > 0; len--){
        data = *ptr++;
        crc = crc ^ ((uint32_t)data<<24);
        for (i = 0; i < 8; i++){
            if (crc & 0x80000000)
                crc = (crc << 1) ^0x04C11DB7;
            else
                crc <<= 1;
        }
    }

    crc = crc^0x00;
    return(crc);
}