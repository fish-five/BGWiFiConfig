#include "BGWiFiConfig.h"
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
WebServer WFconfigserver(2022);
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
ESP8266WebServer WFconfigserver(2022);
#endif

bool BGWiFiConfig::booloffSerial = false;
String BGWiFiConfig::mhtml = "";
String BGWiFiConfig::mhtmlresult = "";
String BGWiFiConfig::runTAG = "";


void BGWiFiConfig::Loop() {
  if (TAG != "OFF")
    WFconfigserver.handleClient();
}

void BGWiFiConfig:: clearWiFi() {
  SPIFFS.format();
  WiFi.disconnect();
}

void BGWiFiConfig:: setPWWiFi(String ssid, String pwd) {
  APssid = ssid;
  APpwd = pwd;
}

void BGWiFiConfig:: setWiFiTimeOut(int sectime) {
  SECtime = sectime;
}

void BGWiFiConfig:: setZDYhtml(String html) {
  mhtml = html;
}

void BGWiFiConfig:: setZDYhtmlret(String html) {
  mhtmlresult = html;
}

void BGWiFiConfig:: outWiFiSET(bool tag) {
  booloutwifiset = tag;
}

void BGWiFiConfig:: offSerial(bool tag) {
  booloffSerial = tag;
}

String BGWiFiConfig:: retRUNTAG() {
  if (runTAG != "") {
    return runTAG;
  }
  return "获取失败,该函数必需放于begin()函数之后";
}

String BGWiFiConfig:: retWiFiSET() {
  String str =  FS_R();
  str.trim();
  if (str.length() > 8)
    return str.substring(8);
  return "未查询到配置信息";
}

void BGWiFiConfig:: begin() {
  Serial.println();
  Serial.println();
  if (SPIFFS.begin()) {
    StrCL(FS_R());
    if (TAG == "OFF") {
      if (MODE == "2") {
        STA_M2(SSID, PWD, IP, GATEWAY, SUBNET, DNS);
        debugPZ();
      } else {
        STA_M1(SSID, PWD);
        debugPZ();
      }
    } else {
      mySerial("欢迎使用BGWiFiConfig配网程序！！", true);
      Serial.println();
      APstart();
      WFconfigserver.on("/result", WRresult);
      WFconfigserver.on("/api", WRapi);
      WFconfigserver.on("/html", WRhtml);
      WFconfigserver.on("/htmlresult", WRhtmlresult);
      WFconfigserver.on("/", WRindex);
      WFconfigserver.begin();
      Serial.println();
      #ifdef ESP32
        mySerial("配网系统已就绪，预计配网时间为12秒，可以开始配网了<<<", true);
      #else
        mySerial("配网系统已就绪，预计配网时间为15秒，可以开始配网了<<<", true);
      #endif
      runTAG = "配网程序开始运行";
    }
  } else {
    mySerial("配网程序启动失败！！", true);
  }
}
 void BGWiFiConfig::mySerial(String str, bool nend) {
  if (!booloffSerial) {
    if (nend)
      Serial.println(str);
    else
      Serial.print(str);
  }
}

void BGWiFiConfig:: debugPZ() {
  if (booloutwifiset) {
    Serial.println("WiFi配网信息：>>>");
    Serial.print("TAG:");
    Serial.println(TAG);
    Serial.print("MODE:");
    Serial.println(MODE);
    Serial.print("SSID:");
    Serial.println(SSID);
    Serial.print("PWD:");
    Serial.println(PWD);
    Serial.print("IP:");
    Serial.println(IP);
    Serial.print("GATEWAY：");
    Serial.println(GATEWAY);
    Serial.print("SUBNET:");
    Serial.println(SUBNET);
    Serial.print("DNS:");
    Serial.println(DNS);
    Serial.println("<<<end<<<");
  }
}

void BGWiFiConfig:: APstart() {
  WiFi.softAPConfig(StrToIP("192.168.22.22"),
                    StrToIP("192.168.22.1"),
                    StrToIP("255.255.255.0"));
  if (APssid != "")
    WiFi.softAP(APssid.c_str(), APpwd.c_str());
  else
    WiFi.softAP("WiFi配网");
  mySerial("配网信息>>>", true);
  mySerial("配网WiFi:", false);
  mySerial(WiFi.softAPSSID(), true);
  mySerial("配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022", true);
  mySerial("配网接口：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022/api", true);
  mySerial("自定义配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022/html", true);
  mySerial("<<<<<end<<<", true);
}


void BGWiFiConfig:: StrCL(String str) {
  if (str != "NULL") {
    int tagNum = str.indexOf("tag=");
    int modeNum = str.indexOf("mode=");
    int ssidNum = str.indexOf("ssid=");
    int pwdNum = str.indexOf("pwd=");
    TAG = str.substring(tagNum + 4, modeNum - 1);
    MODE = str.substring(modeNum + 5, ssidNum - 1);
    SSID = str.substring(ssidNum + 5, pwdNum - 1);
    if (MODE == "2") {
      int ipNum = str.indexOf("ip=");
      int gatewayNum = str.indexOf("gateway=");
      int subnetNum = str.indexOf("subnet=");
      int dnsNum = str.indexOf("dns=");
      PWD = str.substring(pwdNum + 4, ipNum - 1);
      IP = str.substring(ipNum + 3, gatewayNum - 1);
      GATEWAY = str.substring(gatewayNum + 8, subnetNum - 1);
      SUBNET = str.substring(subnetNum + 7, dnsNum - 1);
      DNS = str.substring(dnsNum + 4);
    } else {
      PWD = str.substring(pwdNum + 4);
    }
  }
}

void BGWiFiConfig:: STA_M1(String Mname, String Mssid) {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(Mname.c_str(), Mssid.c_str());
  runTAG = "开始连接WiFi[" + SSID + "]";
  mySerial("Mode1模式：", true);
  mySerial("开始连接WiFi[", false);
  mySerial(SSID, false);
  mySerial("]:", false);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); mySerial(",", false);
    i++;
    mySerial(String(i), false);
    if (i > SECtime - 1) {
      Serial.println();
      runTAG = "连接失败，未成功连接WiFi[" + SSID + "]";
      mySerial("WiFi连接超时,请重试", true);
      break;
    }
  }
  if (WiFi.isConnected()) {
    Serial.println();
    runTAG = "已成功连接WiFi[" + SSID + "]";
    mySerial("连接成功", true);
    mySerial("本机IP:", false);
    mySerial(WiFi.localIP().toString(), true);
  }
  mySerial(">>>>> end <<<<<", true);
  Serial.println();
}

void BGWiFiConfig:: STA_M2(String Mname, String Mssid, String Mlocal_IP, String Mgateway, String Msubnet, String Mdns) {
  WiFi.disconnect();
  if (!WiFi.config(StrToIP(Mlocal_IP), StrToIP(Mgateway), StrToIP(Msubnet), StrToIP(Mdns))) {
    mySerial("WiFi设置失败", true);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(Mname.c_str(), Mssid.c_str());
  runTAG = "开始连接WiFi[" + SSID + "]";
  mySerial("Mode2模式：", true);
  mySerial("开始连接WiFi[", false);
  mySerial(SSID, false);
  mySerial("]:", false);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); mySerial(",", false);
    i++;
    mySerial(String(i), false);
    if (i > SECtime - 1) {
      Serial.println();
      runTAG = "连接失败，未成功连接WiFi[" + SSID + "]";
      mySerial("WiFi连接超时,请重试", true);
      break;
    }
  }
  if (WiFi.isConnected()) {
    Serial.println();
    runTAG = "已成功连接WiFi[" + SSID + "]";
    mySerial("连接成功", true);
    mySerial("本机IP:", false);
    mySerial(WiFi.localIP().toString(), true);
  }
  mySerial(">>>>> end <<<<<", true);
  Serial.println();
}


 void BGWiFiConfig:: WRhtml() {
  WFconfigserver.send(200, "text/html", mhtml);
}
 void BGWiFiConfig:: WRhtmlresult() {
  String retStr;
  String mode = WFconfigserver.arg("mode");
  String ssid = WFconfigserver.arg("ssid");
  String pwd = WFconfigserver.arg("pwd");
  if (mode == "2") {
    String ip = WFconfigserver.arg("ip");
    String gateway = WFconfigserver.arg("gateway");
    String subnet = WFconfigserver.arg("subnet");
    String dns = WFconfigserver.arg("dns");
    retStr = "tag=OFF,mode=" + mode + ",ssid=" + ssid + ",pwd=" + pwd + ",ip=" + ip
             + ",gateway=" + gateway + ",subnet=" + subnet + ",dns=" + dns;
  } else {
    retStr = "tag=OFF,mode=" + mode + ",ssid=" + ssid + ",pwd=" + pwd;
  }
  if (FS_W(retStr)) {
    if (mhtmlresult != "")
      WFconfigserver.send(200, "text/html", mhtmlresult);
    else
      WFconfigserver.send(200, "text/plain", "ok,zdyhtml,mode=" + mode + ",Please restart the board!");
  }
}

 void BGWiFiConfig:: WRindex() {
  String ret = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
               + String("<center><form action=\"result\" method=\"post\">")
               + String( "<h3>配网</h3>")
               + String( "<h4>WiFi名称：<input type=\"text\" name=\"ssid\"/></h4>")
               + String( " <h4>WiFi密码：<input type=\"text\" name=\"pwd\"/></h4>")
               + String( " <p>&nbsp;&nbsp;>>预计需要15秒</p>")
               + String( "<input type=\"submit\" value=\" 开始配网 \"></form></center></body></html>");
  WFconfigserver.send(200, "text/html", ret);
}

 void BGWiFiConfig:: WRresult() {
  String ssid = WFconfigserver.arg("ssid");
  String pwd = WFconfigserver.arg("pwd");
  String retStr = "tag=OFF,mode=1,ssid=" + ssid + ",pwd=" + pwd;
  String ret = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
               + String("<center>")
               + String( "<h2>配网写入结果</h2>")
               + String( "<h2>已配置WiFi名称：" + ssid + "</h2>")
               + String( "<h2>已配置WiFi密码:" +  pwd + "</h2>")
               + String( "<p>确认无误后，请退出页面，并重启开发板</p>")
               + String( "<input type=\"submit\" value=\"返回配网页面\" onclick=\"javascript:history.back();\"></center></body></html>");
  if ( FS_W(retStr))
    WFconfigserver.send(200, "text/html", ret);

}

 void BGWiFiConfig:: WRapi() {
  String retStr;
  String mode = WFconfigserver.arg("mode");
  String ssid = WFconfigserver.arg("ssid");
  String pwd = WFconfigserver.arg("pwd");
  if (mode == "2") {
    String ip = WFconfigserver.arg("ip");
    String gateway = WFconfigserver.arg("gateway");
    String subnet = WFconfigserver.arg("subnet");
    String dns = WFconfigserver.arg("dns");
    retStr = "tag=OFF,mode=" + mode + ",ssid=" + ssid + ",pwd=" + pwd + ",ip=" + ip
             + ",gateway=" + gateway + ",subnet=" + subnet + ",dns=" + dns;
  } else {
    retStr = "tag=OFF,mode=" + mode + ",ssid=" + ssid + ",pwd=" + pwd;
  }
  if (FS_W(retStr))
    WFconfigserver.send(200, "text/plain", "ok,mode=" + mode + ",Please restart the board!");
}


IPAddress BGWiFiConfig:: StrToIP(String str) {
  IPAddress ipadd;
  ipadd.fromString(str);
  return ipadd;
}

 bool BGWiFiConfig:: FS_W(String str) {
  SPIFFS.format();
  File dataFile = SPIFFS.open("/bgwificonfig/wifiset.txt", "w");
  dataFile.print(str);
  dataFile.close();
  Serial.println();
  mySerial(">>", true);
  mySerial(">>配网信息已写入，请重启开发板，连接WiFi！！", true);
    runTAG = "配网信息已写入";
  return true;
}

String BGWiFiConfig:: FS_R() {
  if (!SPIFFS.exists("/bgwificonfig/wifiset.txt")) {
    return "NULL";
  }
  File dataFile = SPIFFS.open("/bgwificonfig/wifiset.txt", "r");
  String retStr;
  for (int i = 0; i < dataFile.size(); i++) {
    retStr += (char)dataFile.read();
  }
  dataFile.close();
  return retStr;
}
