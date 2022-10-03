/**
    BGWiFiConfig示例代码
    开发板： NodeMCU
    实现功能：BGWiFiConfig库自定义OTA页面及结合配网使用及宏定义开启eeshow模式
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/

#define eeshow     
#define eeshowM 1       
#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

String mhtml =
  String("<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\">") +
  String("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">") +
  String("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no\">") +
  String("<title>BGWiFiConfig自定义OTA</title></head><body><h3>自定义OTA</h3></div>") +
  String("<form method='POST' action='/otaupdate' enctype='multipart/form-data'><input type='file' name='otaupdate'>") +
  String("<br><br><input type='submit' value='开始升级'></form></div></body></html>");

void setup() {
  Serial.begin(115200);
  pinMode(D3, INPUT);
  wifipw.begin();
  wifipw.setOTAhtml(mhtml);
  wifipw.OTAbegin();
}

void loop() {
  wifipw.Loop();
  wifipw.OTALoop();
  if (!digitalRead(D3))
    wifipw.clearWiFi();
}
