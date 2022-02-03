/**
    BGWiFiConfig示例代码
    开发板： NodeMCU
    实现功能：最简单的BGWiFiConfig库使用
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/

#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

void setup() {
  Serial.begin(115200);
  pinMode(D3, INPUT);
  wifipw.begin();
}

void loop() {
  wifipw.Loop();
  if (!digitalRead(D3))
    wifipw.clearWiFi();
}
