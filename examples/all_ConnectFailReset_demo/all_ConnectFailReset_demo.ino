/**
    BGWiFiConfig示例代码
    开发板： ESP32C3
    实现功能：BGWiFiConfig库新增：setConFailReset和useSpaceWiFi函数。
             setConFailReset函数：在指定次数及重连时间内，WiFi连接失败，可自动重置配网。
             useSpaceWiFi函数：传入true时，配置的WiFi名称和密码将支持空格。
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/
#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

void setup() {
  Serial.begin(115200);
  delay(1000);
  /** setConFailReset
  1.xReConNum:重连次数(总次数不记第1次)。
  2.xReConTime:超时时间(单位:秒)。
  3.xIsReboot:重置配网后是否自动重启进入配网程序。
  ***/
  wifipw.setConFailReset(3,10,false);
  /**useSpaceWiFi
  传入true时，配置的WiFi名称和密码将支持空格。
  传入fasle或不使用该函数，将默认过滤WiFi空格。
  ***/
  wifipw.useSpaceWiFi(true);
  /**autoStart
  写入配网信息后，将自动重启并连接配置好的WiFi。
  ***/
  wifipw.autoStart(true);
  wifipw.begin();
}

void loop() {
  wifipw.Loop();
}