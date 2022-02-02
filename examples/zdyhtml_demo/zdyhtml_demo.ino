/**
    BGWiFiConfig示例代码
    开发板： NodeMCU
    实现功能：BGWiFiConfig库自定义配网页面的实现
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
 **/
 
#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

String html = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig</title></head><body>")
              + String("<form action=\"htmlresult\">WiFi名称:<br><input type=\"text\" name=\"ssid\">")
              + String("<br>WiFi密码:<br><input type=\"text\" name=\"pwd\"><br><br>")
              + String("<input type=\"submit\" value=\"开始配网\"></form>")
              + String("<p>如果你点击提交，表单数据会被发送到htmlresult处理，setZDYhtmlret函数设置回调页面</p>")
              + String("<p>BGWiFiConfig自定义页面示例程序</body></html>");
String htmlret = String("<html><head><meta charset=\"utf-8\"></head><body>htmlresult回调</body></html>");

void setup() {
  Serial.begin(115200);
  pinMode(D3, INPUT);
  wifipw.setZDYhtml(html);
  wifipw.setZDYhtmlret(htmlret);
  wifipw.begin();
}

void loop() {
  wifipw.Loop();
  if (!digitalRead(D3))
    wifipw.clearWiFi();
}