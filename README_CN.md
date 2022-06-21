# **DFRobot_LIDAR07**
- [English Version](./README.md)

LIDAR07-100W 测距模组是一个基 于单点TOF技术，采用850nm LED光源， 配合独特的光学、结构、电路设计而成的 低成本测距模组，实现中短距离的测距需 求，测距范围为 0.2m～12m，结合相应的 滤波算法处理，可以得到极低的测量噪声。

![SEN0413svg1](./resources/images/SEN0413svg1.png)

## 产品链接(https://www.dfrobot.com.cn/goods-3112.html)
```
SKU：SEN0413
```

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

该库提供了两种收集方法，一种是单次收集，另一种是连续收集。连续采集方式下，可设置采集频率。

## 库安装

这里提供两种使用本库的方法：
1. 打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_LIDAR07"并安装本库.
2. 首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.


## 方法

```C++
  /**
   * @fn begin
   * @brief  检测传感器通信是否正常，将传感器设置为单采集模式
   * @return true通信和传感器工作模式设置成功。false通信失败或获取错误信息
   */
  bool begin();

  /**
   * @fn getVersion
   * @brief 获取传感器的版本信息
   * @return 返回32位数据，低16位是副版本号，高16位是主版本号
   */
  uint32_t getVersion();

  /**
   * @fn startMeasure
   * @brief  开始测量
   */
  void startMeasure();
  
  /**
   * @fn getValue
   * @brief  获取收集的数据
   * @return 是否获取了正确的数据
   * @retval true(成功)
   * @retval false(失败)
   */
  bool getValue();

  /**
   * @fn setConMeasureFreq
   * @brief  设置具体的测量间隔时间
   * @param  frqe该参数仅在连续测量时有效，测量周期设置为ms。最小设置为10MS(即100Hz)。
   * @return 设置频率是否成功
   * @retval true(成功)
   * @retval false(失败)
   */
  bool setConMeasureFreq(uint32_t frqe);

  /**
   *  @fn setMeasureMode
   * @brief  设置传感器工作模式
   * @param  mode 收集数据的方式
   * @n      eLidar07Single  单次
   * @n      eLidar07Continuous  连续
   * @return 查看工作模式设置是否成功
   * @retval true(成功)
   * @retval false(失败)
   */
  bool setMeasureMode(eLIDAR07CollectMode_t mode);

  /**
   * @fn getDistanceMM
   * @brief  获得测量距离数据，范围:0.2m-12m
   * @return 距离,单位:毫米
   */
  uint16_t getDistanceMM();

  /**
   * @fn getSignalAmplitude
   * @brief  得到信号幅度
   * @return 信号幅度
   */
  uint16_t getSignalAmplitude();

  /**
   * @fn startFilter
   * @brief  使能过滤功能，该传感器内部具有轻量级的过滤功能
   * @return 过滤器设置是否成功
   * @retval true(成功)
   * @retval false(失败)
   */
  bool startFilter();

  /**
   * @fn stopFilter
   * @brief  关闭过滤器
   * @return 关闭过滤器是否成功
   * @retval true(成功)
   * @retval false(失败)
   */
  bool stopFilter();
```

## 兼容性

### 串口通讯

|  主板               | 通过 | 未通过 | 未测试 | 备注 |
| ------------------ | :--: | :----: | :----: | ---- |
| Arduino uno        |      |   √    |        |      |
| FireBeetle esp32   |  √   |        |        |      |
| FireBeetle esp8266 |  √   |        |        |      |
| FireBeetle m0      |  √   |        |        |      |
| Leonardo           |  √   |        |        |      |
| Microbit           |      |   √    |        |      |
| Arduino MEGA2560   |  √   |        |        |      |
### I2C通讯

| 主板               | 通过 | 未通过 | 未测试 | 备注 |
| ------------------ | :--: | :----: | :----: | ---- |
| Arduino uno        |  √   |        |        |      |
| FireBeetle esp32   |  √   |        |        |      |
| FireBeetle esp8266 |  √   |        |        |      |
| FireBeetle m0      |  √   |        |        |      |
| Leonardo           |  √   |        |        |      |
| Microbit           |      |   √    |        |      |
| Arduino MEGA2560   |  √   |        |        |      |

## 历史

- 2021/04/16 - 1.0.0 版本
- 2022/06/20 - 2.0.0 版本

## 创作者

杨峰撰写(feng.yang@dfrobot.com)，(欢迎浏览我们的[网站](https://www.dfrobot.com/))
