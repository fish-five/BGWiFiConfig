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
bool BGWiFiConfig::boolautostart = false;
String BGWiFiConfig::mhtml = "";
String BGWiFiConfig::mhtmlresult = "";
String BGWiFiConfig::runTAG = "";
String BGWiFiConfig::StrsUMSG[13] = {"NULL"};
int BGWiFiConfig::UMSGnum = 0;


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

void BGWiFiConfig:: setNumUMSG(int umsgnum) {
  UMSGnum = umsgnum;
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

void BGWiFiConfig:: autoStart(bool tag) {
  boolautostart = tag;
}

void BGWiFiConfig:: delay_rst() {
  /**
    #ifdef ESP32
    unsigned long onetime = millis();
    while (true) {
      unsigned long times = millis();
      if (times - onetime > 2000)
        break;
    }
    #else
      delay(2000);
    #endif
  */
  delay(2000);
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
    if (TAG == "OFF" && !booloffconnectwifi) {
      if (UMSGnum > 0 && UMSGnum < 13 )
        StrCL_UMSG(FS_R_UMSG());
      if (MODE == "2") {
        STA_M2(SSID, PWD, IP, GATEWAY, SUBNET, DNS);
        debugPZ();
      } else {
        STA_M1(SSID, PWD);
        debugPZ();
      }
    } else if (TAG == "OFF" && booloffconnectwifi) {
      mySerial("....user wificode start....", true);
      if (UMSGnum > 0 && UMSGnum < 13 )
        StrCL_UMSG(FS_R_UMSG());
      debugPZ();
    }
    else {
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

  if (UMSGnum > 0) {
    String UMSGNAME[12] = {"umsg1", "umsg2", "umsg3", "umsg4", "umsg5", "umsg6",
                           "umsg7", "umsg8", "umsg9", "umsg10", "umsg11", "umsg12"
                          };
    String retStr2 = "";
    for (int i = 0; i < UMSGnum; i++) {
      StrsUMSG[i] = WFconfigserver.arg(UMSGNAME[i]);
      StrsUMSG[i].trim();
      if (StrsUMSG[i] != "" && StrsUMSG[i] != " " && StrsUMSG[i] != NULL && StrsUMSG[i] != "NULL") {
        retStr2 += UMSGNAME[i] + "=" + StrsUMSG[i];
      } else {
        retStr2 += UMSGNAME[i] + "=" + "[err:7000]设置的数据组与实际数据组数量不等！！";
      }
    }
    retStr2.trim();
    retStr2 = retStr2 + "=umsg=";
    if (FS_W_UMSG(retStr2))
    {
      mySerial(">>UMSG Write OK", true);
      mySerial(">>自定义信息已写入：", false);
      mySerial(String(UMSGnum), false);
      mySerial("条", true);
      runTAG = "自定义信息已写入：" + String(UMSGnum) + "条";
    }
  }

  if (FS_W(retStr)) {
    if (mhtmlresult != "") {
      WFconfigserver.send(200, "text/html", mhtmlresult);
      if (boolautostart) {
        delay_rst();
        ESP.restart();
      }
    }
    else {
      if (boolautostart) {
        WFconfigserver.send(200, "text/plain", "ok,zdyhtml,mode=" + mode + ",The board has rebooted!");
        delay_rst();
        ESP.restart();
      } else {
        WFconfigserver.send(200, "text/plain", "ok,zdyhtml,mode=" + mode + ",Please restart the board!");
      }
    }
  }
}

void BGWiFiConfig:: WRindex() {
  String time = "";
#ifdef ESP32
  time = "12";
#else
  time = "15";
#endif
  String ret = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
               + String("<center><form action=\"result\" method=\"post\">")
               + String( "<h3>配网</h3>")
               + String( "<h4>WiFi名称：<input type=\"text\" name=\"ssid\"/></h4>")
               + String( " <h4>WiFi密码：<input type=\"text\" name=\"pwd\"/></h4>")
               + String( " <p>&nbsp;&nbsp;>>预计需要" + time + "秒</p>")
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

  String ret2 = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
                + String("<center>")
                + String( "<h2>配网写入结果</h2>")
                + String( "<h2>已配置WiFi名称：" + ssid + "</h2>")
                + String( "<h2>已配置WiFi密码：" +  pwd + "</h2>")
                + String( "<p>已调用autoStart()函数，已自动重启开发板，请观察串口输出！！</p>")
                + String( "<input type=\"submit\" value=\"返回配网页面\" onclick=\"javascript:history.back();\"></center></body></html>");

  if ( FS_W(retStr)) {
    if (boolautostart) {
      WFconfigserver.send(200, "text/html", ret2);
      delay_rst();
      ESP.restart();
    } else {
      WFconfigserver.send(200, "text/html", ret);
    }
  }
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

  if (UMSGnum > 0) {
    String UMSGNAME[12] = {"umsg1", "umsg2", "umsg3", "umsg4", "umsg5", "umsg6",
                           "umsg7", "umsg8", "umsg9", "umsg10", "umsg11", "umsg12"
                          };
    String retStr2 = "";
    for (int i = 0; i < UMSGnum; i++) {
      StrsUMSG[i] = WFconfigserver.arg(UMSGNAME[i]);
      StrsUMSG[i].trim();
      if (StrsUMSG[i] != "" && StrsUMSG[i] != " " && StrsUMSG[i] != NULL && StrsUMSG[i] != "NULL") {
        retStr2 += UMSGNAME[i] + "=" + StrsUMSG[i];
      } else {
        retStr2 += UMSGNAME[i] + "=" + "[err:7000]设置的数据组与实际数据组数量不等！！";
      }
    }
    retStr2.trim();
    retStr2 = retStr2 + "=umsg=";
    if (FS_W_UMSG(retStr2))
    {
      mySerial(">>UMSG Write OK", true);
      mySerial(">>自定义信息已写入：", false);
      mySerial(String(UMSGnum), false);
      mySerial("条", true);
      runTAG = "自定义信息已写入：" + String(UMSGnum) + "条";
    }
  }

  if (FS_W(retStr)) {
    if (boolautostart) {
      WFconfigserver.send(200, "text/plain", "ok,mode=" + mode + ",The board has rebooted!");
      delay_rst();
      ESP.restart();
    } else {
      WFconfigserver.send(200, "text/plain", "ok,mode=" + mode + ",Please restart the board!");
    }
  }
}


IPAddress BGWiFiConfig:: StrToIP(String str) {
  IPAddress ipadd;
  ipadd.fromString(str);
  return ipadd;
}

bool BGWiFiConfig:: FS_W(String str) {
  if (UMSGnum < 1)
    SPIFFS.format();
  File dataFile = SPIFFS.open("/bgwificonfig/wifiset.txt", "w");
  dataFile.print(str);
  dataFile.close();
  Serial.println();
  mySerial(">>", true);
  if (boolautostart) {
    mySerial(">>配网信息已写入，开发板将自动重启并连接WiFi，请观察串口信息！！", true);
  } else {
    mySerial(">>配网信息已写入，请重启开发板，连接WiFi！！", true);
  }
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

bool BGWiFiConfig:: FS_W_UMSG(String str) {
  SPIFFS.format();
  File dataFile = SPIFFS.open("/bgwificonfig/umsg.txt", "w");
  dataFile.print(str);
  dataFile.close();
  Serial.println();
  mySerial(">>", true);
  return true;
}

String BGWiFiConfig:: FS_R_UMSG() {
  if (!SPIFFS.exists("/bgwificonfig/umsg.txt")) {
    return "NULL";
  }
  File dataFile = SPIFFS.open("/bgwificonfig/umsg.txt", "r");
  String retStr ;
  for (int i = 0; i < dataFile.size(); i++) {
    retStr += (char)dataFile.read();
  }
  dataFile.close();
  return retStr;
}

void BGWiFiConfig:: StrCL_UMSG(String str) {
  if (str != "NULL" && str != "" && str != NULL && UMSGnum > 0 && UMSGnum < 13) {
    str.trim();
    String umsgname[13] = {
      "umsg1=", "umsg2=", "umsg3=",
      "umsg4=", "umsg5=", "umsg6=",
      "umsg7=", "umsg8=", "umsg9=",
      "umsg10=", "umsg11=", "umsg12=", "=umsg="
    };
    int umsgkey[13] = { -1};
    for (int i = 0; i < UMSGnum + 1; i++) {
      umsgkey[i] = str.indexOf(umsgname[i]);
    }
    for (int j = 0; j < UMSGnum - 1; j++) {
      StrsUMSG[j] = str.substring(umsgkey[j] + umsgname[j].length(), umsgkey[j + 1]);
    }
    StrsUMSG[UMSGnum - 1] = str.substring(umsgkey[UMSGnum - 1] + umsgname[UMSGnum - 1].length(), str.indexOf(umsgname[12]) );
  }
}

String BGWiFiConfig:: readUMSG(int i) {
  if (UMSGnum > 0 && i < 13 && i > 0 && TAG == "OFF") {
    StrsUMSG[i - 1].trim();
    if (StrsUMSG[i - 1] != NULL && StrsUMSG[i - 1] != "" && StrsUMSG[i - 1] != "NULL")
      return StrsUMSG[i - 1];
  }
  return "NULL";
}

void BGWiFiConfig:: offConnectWiFi(bool tag) {
  booloffconnectwifi = tag;
}

String BGWiFiConfig:: readWiFi(int i) {
  String str = FS_R();
  if (i == 0) {
    return str.substring(str.indexOf(",ssid=") + 6, str.indexOf(",pwd="));
  } else if (i == 1) {
    return str.substring(str.indexOf(",pwd=") + 5, str.indexOf(",ip="));
  }
  return "NULL";
}
