/**
    BGWiFiConfig示例代码Beta
    开发板： NodeMCU-32S
    实现功能：BGWiFiConfig库接入点灯科技示例代码（以blinker示例代码Hello_WiFi为例）
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
    Other：
         1.点灯科技(blinker)文档：https://diandeng.tech/doc/getting-start-esp32-wifi
         2.BGWiFiConfig库接入blinker示例代码如下，用的是blinker的Hello_WiFi程序，
         如需使用请确保BGWiFiConfig库在v1.0.6版本及以上，此示例代码为Beta程序，如有问题请反馈，但
         精力有限，只受理BGWiFiConfig库问题，遇到问题，请先看文档！看文档！！看文档！！！
 **/

#define BLINKER_WIFI
#include <Blinker.h>

#include <BGWiFiConfig.h>
BGWiFiConfig wifipw;
#define key 0   //按钮清除配网信息，其他esp32/8266开发板改这个中断io口即可。

String html = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig</title></head><body>")
              + String("<form action=\"htmlresult\" method=\"post\">")
              + String("<br>WiFi名称:<br><input type=\"text\" name=\"ssid\">")
              + String("<br>WiFi密码:<br><input type=\"text\" name=\"pwd\">")
              + String("<br>auth:<br><input type=\"text\" name=\"umsg1\">")
              + String("<br>模式选择：<input type=\"radio\" name=\"mode\" value=\"1\" checked>mode:1")
              + String("&nbsp;&nbsp;<input type=\"radio\" name=\"mode\" value=\"2\" >mode:2 <br><br>")
              + String("<input type=\"submit\" value=\"开始配网\"></form>")
              + String("<p>如果你点击提交，表单数据会被发送到htmlresult处理，setZDYhtmlret函数设置回调页面</p>")
              + String("<p>BGWiFiConfig接入Blinker示例代码Beta</body></html>");
String htmlret = String("<html><head><meta charset=\"utf-8\"></head><body>配置成功，请打开blinker软件，等待开发板连接WiFi。</body></html>");

// Blinker示例程序Hello_WiFi内容
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");
int counter = 0;
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
  counter++;
  Number1.print(counter);
}

void keyCwifi() {
  wifipw.clearWiFi();
  Serial.println("WiFi信息清除成功！");
}

void setup() {
  Serial.begin(115200);
  pinMode(key, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(key), keyCwifi, CHANGE);

  wifipw.offConnectWiFi(true);
  wifipw.autoStart(true);
  wifipw.setZDYhtml(html);
  wifipw.setZDYhtmlret(htmlret);
  wifipw.setNumUMSG(1);
  wifipw.begin();

  if (wifipw.OK()) {
    Serial.println(">>> blinker start debug <<<");
    Serial.println(wifipw.readUMSG(1));
    Serial.println(wifipw.readWiFi(0));
    Serial.println(wifipw.readWiFi(1));
    Serial.println(">>> end <<<");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Blinker.begin( wifipw.readUMSG(1).c_str(), wifipw.readWiFi(0).c_str(), wifipw.readWiFi(1).c_str());
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
  }

}

void loop() {
  wifipw.Loop();
  if (wifipw.OK())
    Blinker.run();
}
