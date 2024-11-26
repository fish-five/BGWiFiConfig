/**
    BGWiFiConfig示例代码
    开发板： ESP32
    实现功能：新增兼容模式，优化解决部分手机配网WiFi自动断连问题，
    启用兼容模式配网IP将从192.168.22.22变为192.168.22.1。
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
  wifipw.setConFailReset(3, 10,true);
  wifipw.useSpaceWiFi(true);
  wifipw.autoStart(true);
  /**setCompatibleMode
  1.xCMTag为true：启用兼容模式，配网IP为：192.168.22.1
  2.xCMTag为false：关闭兼容模式，配网IP为：192.168.22.22
  ***/
  wifipw.setCompatibleMode(true);      
  wifipw.begin();
}

void loop() {
  wifipw.Loop();
}