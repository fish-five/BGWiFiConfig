/**
    BGWiFiConfig示例代码
    开发板： ESP8266和ESP32通用
    实现功能：BGWiFiConfig只使用OTA功能
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/

/**setOTAWiFiAP函数，设置AP模式，
   生成一个WiFi名为OTA升级，
   密码为空的WiFi，并等待用户连接;**/

/**setOTAWiFiSTA函数，设置STA模式，
   主动连接WiFi名为esp32，
   密码为123456789的WiFi**/

/**setOTAWiFiAPSTA函数，
   设置AP模式与STA模式共存**/


#define onlyota           /**仅使用OTA功能,必须定义onlyota*/
#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

void setup() {
  Serial.begin(115200);
  /** 以下三种连接方式任选一种即可 */
  wifipw.setOTAWiFiAP("OTA升级","","192.168.33.33");
  //wifipw.setOTAWiFiSTA("esp32","123456789");
  //wifipw.setOTAWiFiAPSTA("OTA升级", "", "esp32", "123456789");
  /*****end*****/
  //wifipw.setOTATimeOut(30); /**设置就绪超时时间，非必须*/
  wifipw.OTAbegin();
  //Serial.println(wifipw.getOTAIP()); /**获取OTA升级IP，非必须*/
}

void loop() {
  wifipw.OTALoop();
}
