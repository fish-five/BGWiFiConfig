/**
    BGWiFiConfig示例代码
    开发板： ESP32S3
    实现功能：使用LittleFS，本demo建议配合微信小程序配网（小程序名：配网库）使用。
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/
#define useLittleFS
#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

void setup() {
  Serial.begin(115200);
  delay(1000);
  wifipw.setConFailReset(2, 10,true);
  wifipw.useSpaceWiFi(true);
  wifipw.autoStart(true);
  wifipw.setCompatibleMode(true);      
  wifipw.begin();
}

void loop() {
  wifipw.Loop();
}