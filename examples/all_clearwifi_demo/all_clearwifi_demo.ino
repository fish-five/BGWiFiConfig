/**
    BGWiFiConfig示例代码
    开发板： ESP8266和ESP32通用
    实现功能：清除已配置的WiFi信息。其实该功能BGWiFiConfig库一直都有，一个函数就能解决问题，
            但是有小伙伴并没有认真阅读文档所以没有发现，此次特意写了个demo，以后就不会写这种
            demo了，请小伙伴多看文档，新功能也会在文档告诉你怎么使用，请一定要多看文档！！！
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/

#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("开始清除配网信息");
  wifipw.clearWiFi();
  Serial.println("配网信息清除成功");
}

void loop() {
}
