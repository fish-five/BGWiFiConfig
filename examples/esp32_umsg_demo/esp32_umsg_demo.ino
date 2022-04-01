/**
    BGWiFiConfig示例代码
    开发板： NodeMCU32S
    实现功能：BGWiFiConfig库UMSG(配网时自定义数据)功能演示。
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
 **/

#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;

String html = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig</title></head><body>")
              + String("<form action=\"htmlresult\" method=\"post\">")
              + String("<br>WiFi名称:<br><input type=\"text\" name=\"ssid\">")
              + String("<br>WiFi密码:<br><input type=\"text\" name=\"pwd\">")
              + String("<br>UMSG数据组1:<br><input type=\"text\" name=\"umsg1\">")
              + String("<br>UMSG数据组2:<br><input type=\"text\" name=\"umsg2\">")
              + String("<br>UMSG数据组3:<br><input type=\"text\" name=\"umsg3\">")
              + String("<br>UMSG数据组4:<br><input type=\"text\" name=\"umsg4\">")
              + String("<br>UMSG数据组5:<br><input type=\"text\" name=\"umsg5\">")
              + String("<br>UMSG数据组6:<br><input type=\"text\" name=\"umsg6\">")
              + String("<br>UMSG数据组7:<br><input type=\"text\" name=\"umsg7\">")
              + String("<br>UMSG数据组8:<br><input type=\"text\" name=\"umsg8\">")
              + String("<br>UMSG数据组9:<br><input type=\"text\" name=\"umsg9\">")
              + String("<br>UMSG数据组10:<br><input type=\"text\" name=\"umsg10\">")
              + String("<br>UMSG数据组11:<br><input type=\"text\" name=\"umsg11\">")
              + String("<br>UMSG数据组12:<br><input type=\"text\" name=\"umsg12\">")
              + String("<br>模式选择：<input type=\"radio\" name=\"mode\" value=\"1\" checked>mode:1")
              + String("&nbsp;&nbsp;<input type=\"radio\" name=\"mode\" value=\"2\" >mode:2 <br><br>")
              + String("<input type=\"submit\" value=\"开始配网\"></form>")
              + String("<p>如果你点击提交，表单数据会被发送到htmlresult处理，setZDYhtmlret函数设置回调页面</p>")
              + String("<p>BGWiFiConfig自定义数据组UMSG示例程序</body></html>");
String htmlret = String("<html><head><meta charset=\"utf-8\"></head><body>htmlresult回调</body></html>");

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT);
  wifipw.setZDYhtml(html);
  wifipw.setZDYhtmlret(htmlret);
  wifipw.setNumUMSG(12);
  wifipw.begin();
  for (int i = 1; i < 13; i++) {
    String msg = wifipw.readUMSG(i);
    if (msg != "NULL") {
      Serial.println();
      Serial.print("打印数据组[");
      Serial.print(i);
      Serial.print("]-->");
      Serial.println(wifipw.readUMSG(i));
    }
  }
}

void loop() {
  wifipw.Loop();
  if (!digitalRead(0))
    wifipw.clearWiFi();
}
