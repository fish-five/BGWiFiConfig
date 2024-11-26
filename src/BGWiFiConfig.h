#ifndef _BGWiFiConfig_H_
#define _BGWiFiConfig_H_
#include <Arduino.h>
#include <DNSServer.h>

#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <Update.h>
#include <SPIFFS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#endif

#ifdef exe
#include "DEFPORT.h"
#else
#ifdef eeshow
#define DEFPORT 80
#ifdef eeshowM
#if eeshowM == 1
#define DEFPORTPWMODE 1
#elif eeshowM == 2
#define DEFPORTPWMODE 2
#else
#define DEFPORTPWMODE 0
#endif
#else
#define DEFPORTPWMODE 0
#endif
#define DEFPORTHTML "NULL"
#else
#define DEFPORT 2022
#define DEFPORTPWMODE 0
#define DEFPORTHTML "NULL"
#endif
#endif

#ifdef onlyota
#define onlyotaTAG true
#else
#define onlyotaTAG false
#endif

class BGWiFiConfig {
private:
  String TAG, MODE, SSID, PWD, IP, GATEWAY, SUBNET, DNS;
  String APssid, APpwd;
  static String StrsUMSG[13];
  static String StrApiRet[2];
  static String mhtml, mhtmlresult, runTAG, OTAserverIndex;
  int SECtime = 30;
  int OTASECtime = 3600;
  int gReConNum = 0;
  static int UMSGnum;
  static int gICompatibleM;
  bool booloutwifiset = false;
  bool booloffconnectwifi = false;
  bool OTASECtimeTAG = false;
  bool boolConFailResetTag = false;
  bool boolConFailResetIsReboot = false;
  static bool boolSpaceWiFiTag;
  static bool boolautostart;
  static bool booloffSerial;
  static void mySerial(String str, bool nend);
  IPAddress otapip[3];
  void debugPZ();
  void APstart();
  void StrCL(String str);
  void StrCL_UMSG(String str);
  void STA_M1(String Mname, String Mssid);
  void STA_M2(String Mname, String Mssid, String Mlocal_IP, String Mgateway, String Msubnet, String Mdns);
  static void delay_rst();
  static void WRhtml();
  static void WRhtmlresult();
  static void WRindex();
  static void WRresult();
  static void WRapi();
  static void WRindexDH();
  IPAddress StrToIP(String str);
  static bool FS_W(String str);
  String FS_R();
  static bool FS_W_UMSG(String str);
  String FS_R_UMSG();
  void clOTAPIP(String ip);
  void STA_M3_FailReset();
  IPAddress getPWIP();
  static String getPWIPDH();

public:
  void begin();
  void Loop();
  void clearWiFi();
  void setPWWiFi(String ssid, String pwd);
  void setWiFiTimeOut(int sectime);
  void setZDYhtml(String html);
  void setZDYhtmlret(String html);
  void outWiFiSET(bool tag);
  void offSerial(bool tag);
  void autoStart(bool tag);
  void offConnectWiFi(bool tag);
  void setNumUMSG(int i);
  String readUMSG(int i);
  String readWiFi(int i);
  String retRUNTAG();
  String retWiFiSET();
  bool OK(bool tag = false);
  String& setApiRet(int i);
  void OTAbegin();
  void OTALoop();
  String getOTAIP();
  void setOTAhtml(String html);
  void setOTATimeOut(int sectime);
  void setOTAWiFiSTA(String ssid, String pwd);
  void setOTAWiFiAP(String ssid, String pwd, String ip = "192.168.33.33");
  void setOTAWiFiAPSTA(String APssid, String APpwd, String STAssid, String STApwd, String ip = "192.168.33.33");
  void setConFailReset(int xReConNum = 0, int xReConTime = 30, bool xIsReboot = false);
  void useSpaceWiFi(bool xSpaceWiFiTag = false);
  void setCompatibleMode(bool xCMTag = true);
  void setCompatibleModeTest(bool xCMTag = true);
};
#endif


/**-
  cpp function ach
  -**/
DNSServer WFconfigDNSserver;
#ifdef ESP32
#if DEFPORT == 80
WebServer WFconfigserver(80);
#else
WebServer WFconfigserver(2022);
#endif
#else
#if DEFPORT == 80
ESP8266WebServer WFconfigserver(80);
#else
ESP8266WebServer WFconfigserver(2022);
#endif
#endif

#ifdef ESP32
WebServer OTAserver(80);
#else
ESP8266WebServer OTAserver(80);
#endif

int BGWiFiConfig::gICompatibleM = 1;
bool BGWiFiConfig::booloffSerial = false;
bool BGWiFiConfig::boolautostart = false;
bool BGWiFiConfig::boolSpaceWiFiTag = false;
String BGWiFiConfig::mhtml = "";
String BGWiFiConfig::mhtmlresult = "";
String BGWiFiConfig::runTAG = "";
String BGWiFiConfig::StrsUMSG[13] = { "NULL" };
String BGWiFiConfig::StrApiRet[2] = { "NULL" };
int BGWiFiConfig::UMSGnum = 0;
String BGWiFiConfig::OTAserverIndex =
  String("<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\">") + String("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">") + String("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no\">") + String("<title>BGWiFiConfigOTA</title><style>.center {margin: auto;width: 20%;") + String("border: 10px solid #20B2AA;padding: 100px;}</style></head>") + String("<body><div class=\"center\"><div style=\"text-align: center;\"><h3>OTA在线升级(bin文件)</h3></div>") + String("<form method='POST' action='/otaupdate' enctype='multipart/form-data'><input type='file' name='otaupdate'>") + String("<br><br><input type='submit' value='开始升级'></form></div></body></html>");


void BGWiFiConfig::Loop() {
  if (TAG != "OFF") {
    WFconfigserver.handleClient();
    WFconfigDNSserver.processNextRequest();
  }
}

void BGWiFiConfig::clearWiFi() {
  SPIFFS.format();
  WiFi.disconnect();
}

void BGWiFiConfig::setPWWiFi(String ssid, String pwd) {
  APssid = ssid;
  APpwd = pwd;
}

void BGWiFiConfig::setWiFiTimeOut(int sectime) {
  SECtime = sectime;
}

void BGWiFiConfig::setNumUMSG(int umsgnum) {
  UMSGnum = umsgnum;
}

void BGWiFiConfig::setZDYhtml(String html) {
  mhtml = html;
}

void BGWiFiConfig::setZDYhtmlret(String html) {
  mhtmlresult = html;
}

void BGWiFiConfig::outWiFiSET(bool tag) {
  booloutwifiset = tag;
}

void BGWiFiConfig::offSerial(bool tag) {
  booloffSerial = tag;
}

void BGWiFiConfig::autoStart(bool tag) {
  boolautostart = tag;
}

void BGWiFiConfig::delay_rst() {
  /**
    #ifdef ESP32
    unsigned long onetime=millis();
    while (true) {
      unsigned long times=millis();
      if (times - onetime > 2000)
        break;
    }
    #else
      delay(2000);
    #endif
  */
  delay(2000);
}

String BGWiFiConfig::retRUNTAG() {
  if (runTAG != "") {
    return runTAG;
  }
  return "获取失败,该函数必需放于begin()函数之后";
}

String BGWiFiConfig::retWiFiSET() {
  String str = FS_R();
  str.trim();
  if (str.length() > 8)
    return str.substring(8);
  return "未查询到配置信息";
}

void BGWiFiConfig::begin() {
  Serial.println();
  Serial.println();
  if (SPIFFS.begin()) {
    StrCL(FS_R());
    if (TAG == "OFF" && !booloffconnectwifi) {
      if (UMSGnum > 0 && UMSGnum < 13)
        StrCL_UMSG(FS_R_UMSG());
      if (boolConFailResetTag) {
        STA_M3_FailReset();
        debugPZ();
      } else {
        if (MODE == "2") {
          STA_M2(SSID, PWD, IP, GATEWAY, SUBNET, DNS);
          debugPZ();
        } else {
          STA_M1(SSID, PWD);
          debugPZ();
        }
      }
    } else if (TAG == "OFF" && booloffconnectwifi) {
      mySerial("....user wificode start....", true);
      if (UMSGnum > 0 && UMSGnum < 13)
        StrCL_UMSG(FS_R_UMSG());
      debugPZ();
    } else {
      mySerial("欢迎使用BGWiFiConfig配网程序！！", true);
      Serial.println();
      APstart();
      Serial.println();
#if DEFPORT == 80
      WFconfigserver.on("/result", HTTP_GET, WRresult);
      WFconfigserver.on("/result", HTTP_POST, WRresult);
      WFconfigserver.on("/api", HTTP_POST, WRapi);
      WFconfigserver.on("/api", HTTP_GET, WRapi);
      WFconfigserver.on("/html", HTTP_GET, WRhtml);
      WFconfigserver.on("/html", HTTP_POST, WRhtml);
      WFconfigserver.on("/htmlresult", HTTP_GET, WRhtmlresult);
      WFconfigserver.on("/htmlresult", HTTP_POST, WRhtmlresult);
      WFconfigserver.on("/def", HTTP_GET, WRindex);
      WFconfigserver.on("/def", HTTP_POST, WRindex);
      if (DEFPORTPWMODE == 1) {
        WFconfigserver.on("/", HTTP_GET, WRindex);
        WFconfigserver.on("/", HTTP_POST, WRindex);
        WFconfigserver.onNotFound(WRindex);
        mySerial(">>当前为eeshow模式1", true);
      } else if (DEFPORTPWMODE == 2) {
        WFconfigserver.on("/", HTTP_GET, WRhtml);
        WFconfigserver.on("/", HTTP_POST, WRhtml);
        WFconfigserver.onNotFound(WRhtml);
        mySerial(">>当前为eeshow模式2", true);
      } else {
        WFconfigserver.on("/", HTTP_GET, WRindexDH);
        WFconfigserver.on("/", HTTP_POST, WRindexDH);
        WFconfigserver.onNotFound(WRindexDH);
        mySerial(">>当前为eeshow模式0", true);
      }
      WFconfigserver.begin();
      if (WFconfigDNSserver.start(53, "*", getPWIP())) {
        mySerial(">>eeshow模式启动成功", true);
      } else {
        mySerial(">>eeshow模式启动失败", true);
      }
#else
      WFconfigserver.on("/result", WRresult);
      WFconfigserver.on("/api", WRapi);
      WFconfigserver.on("/html", WRhtml);
      WFconfigserver.on("/htmlresult", WRhtmlresult);
      WFconfigserver.on("/", WRindex);
      WFconfigserver.begin();
      mySerial(">>当前为默认模式", true);
#endif
#ifdef ESP32
      mySerial("配网系统已就绪，预计配网时间为12秒，可以开始配网了<<<", true);
#else
      mySerial("配网系统已就绪，预计配网时间为15秒，可以开始配网了<<<", true);
#endif
      runTAG = "配网程序开始运行";
      if (TAG != "OFF" && onlyotaTAG) {
        Serial.println("[err:宏定义onlyota错误,用了配网函数就不能定义onlyota,请修改程序BUG]");
        delay(5000);
        ESP.restart();
      }
      while (WiFi.softAPgetStationNum() < 1) {
        delay(1000);
      }
    }
  } else {
   mySerial("配网挂载初始化中ing..", true);
    if(!SPIFFS.begin(true)){
      mySerial("配网程序启动失败！！", true);
    }else{
      ESP.restart();
    }
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

void BGWiFiConfig::debugPZ() {
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

void BGWiFiConfig::APstart() {
  if (gICompatibleM != 3) {
    if (gICompatibleM < 2) {
      WiFi.softAPConfig(getPWIP(),
                        StrToIP("192.168.22.1"),
                        StrToIP("255.255.255.0"));
    } else {
      WiFi.softAPConfig(getPWIP(),
                        getPWIP(),
                        StrToIP("255.255.255.0"));
    }
  }
  if (APssid != "")
    WiFi.softAP(APssid.c_str(), APpwd.c_str());
  else
    WiFi.softAP("WiFi配网");
  mySerial("配网信息>>>", true);
  mySerial("配网WiFi:", false);
  mySerial(WiFi.softAPSSID(), true);
#if DEFPORT == 80
  mySerial("配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial("/def", true);
  mySerial("配网接口：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial("/api", true);
  mySerial("自定义配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial("/html", true);
#else
  mySerial("配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022", true);
  mySerial("配网接口：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022/api", true);
  mySerial("自定义配网页面：", false);
  mySerial(WiFi.softAPIP().toString(), false);
  mySerial(":2022/html", true);
#endif
  mySerial("<<<<<end<<<", true);
}

void BGWiFiConfig::StrCL(String str) {
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

void BGWiFiConfig::STA_M1(String Mname, String Mssid) {
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
    delay(1000);
    mySerial(",", false);
    i++;
    mySerial(String(i), false);
    if (i > SECtime - 1) {
      Serial.println();
      if (!boolConFailResetTag) {
        runTAG = "连接失败，未成功连接WiFi[" + SSID + "]";
        mySerial("WiFi连接超时,请重试", true);
      }
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

void BGWiFiConfig::STA_M2(String Mname, String Mssid, String Mlocal_IP, String Mgateway, String Msubnet, String Mdns) {
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
    delay(1000);
    mySerial(",", false);
    i++;
    mySerial(String(i), false);
    if (i > SECtime - 1) {
      Serial.println();
      if (!boolConFailResetTag) {
        runTAG = "连接失败，未成功连接WiFi[" + SSID + "]";
        mySerial("WiFi连接超时,请重试", true);
      }
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


void BGWiFiConfig::WRindexDH() {
  /*
  String dhhtml = String("<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\">")
                  + String("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" ")
                  + String("content=\"width=device-width, initial-scale=1.0\"><title>BGWiFiConfig配网 </title > ")
                  + String("<style> .button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 8px; text-align: center; } .grad { background-color: red; background-image: linear-gradient(#e66465, #9198e5); text-align: center; } </style>")
                  + String("</head> <h1 style=\"color: rgb(87, 217, 215);\">BGWiFiConfig</h1>")
                  + String("<h2 style=\"color: wheat;\">请选择配网模式</h2>")
                  + String("<a class=\"button\" href=\"http://192.168.22.22/def\">默认网页配网</a><br>")
                  + String("<a class=\"button\" href=\"http://192.168.22.22/html\">自定义网页配网</a>")
                  + String("<h3>api配网和微信小程序配网默认已支持，无需选择。</h3>")
                  + String("</body></html>");*/

  String dhhtml = String("<!DOCTYPE html><html lang=\"zh-CN\"><head><meta charset=\"UTF-8\">")
                  + String("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" ")
                  + String("content=\"width=device-width, initial-scale=1.0\"><title>BGWiFiConfig配网 </title > ")
                  + String("<style> .button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 8px; text-align: center; } .grad { background-color: red; background-image: linear-gradient(#e66465, #9198e5); text-align: center; } </style>")
                  + String("</head> <h1 style=\"color: rgb(87, 217, 215);\">BGWiFiConfig</h1>")
                  + String("<h2 style=\"color: wheat;\">请选择配网模式</h2>")
                  + String("<a class=\"button\" href=\"http://" + getPWIPDH() + "/def\">默认网页配网</a><br>")
                  + String("<a class=\"button\" href=\"http://" + getPWIPDH() + "/html\">自定义网页配网</a>")
                  + String("<h3>api配网和微信小程序配网默认已支持，无需选择。</h3>")
                  + String("</body></html>");

  if (DEFPORTHTML != "" && DEFPORTHTML != "NULL" && DEFPORTHTML != NULL) {
    WFconfigserver.send(200, "text/html", DEFPORTHTML);
  } else {
    WFconfigserver.send(200, "text/html", dhhtml);
  }
}

void BGWiFiConfig::WRhtml() {
  WFconfigserver.send(200, "text/html", mhtml);
}
void BGWiFiConfig::WRhtmlresult() {
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
    String UMSGNAME[12] = { "umsg1", "umsg2", "umsg3", "umsg4", "umsg5", "umsg6",
                            "umsg7", "umsg8", "umsg9", "umsg10", "umsg11", "umsg12" };
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
    retStr2.replace(" ", "");
    if (FS_W_UMSG(retStr2)) {
      mySerial(">> UMSG Write OK", true);
      mySerial(">> 自定义信息已写入：", false);
      mySerial(String(UMSGnum), false);
      mySerial("条", true);
      runTAG = "自定义信息已写入：" + String(UMSGnum) + "条";
    }
  }

  if (ssid != "" && ssid != NULL) {
    if (!boolSpaceWiFiTag) {
      retStr.replace(" ", "");
    }
    if (FS_W(retStr)) {
      if (mhtmlresult != "") {
        WFconfigserver.send(200, "text/html", mhtmlresult);
        if (boolautostart) {
          delay_rst();
          ESP.restart();
        }
      } else {
        if (boolautostart) {
          WFconfigserver.send(200, "text/plain", "ok, zdyhtml, mode=" + mode + ", The board has rebooted!");
          delay_rst();
          ESP.restart();
        } else {
          WFconfigserver.send(200, "text/plain", "ok, zdyhtml, mode=" + mode + ", Please restart the board!");
        }
      }
    }
  }
}

void BGWiFiConfig::WRindex() {
  String time = "";
#ifdef ESP32
  time = "12";
#else
  time = "15";
#endif
  /**
    String ret2=String("<!DOCTYPE html> <html lang=\"zh-CN\"> <head> <meta charset=\"UTF-8\"><title>BGWiFiConfig配网</title></head><body>")
                + String("<center><form action=\"result\" method=\"post\">")
                + String( "<h3>配网</h3>")
                + String( "<h4>WiFi名称：<input type=\"text\" name=\"ssid\"/></h4>")
                + String( " <h4>WiFi密码：<input type=\"text\" name=\"pwd\"/></h4>")
                + String( " <p>&nbsp;&nbsp;>>预计需要" + time + "秒</p>")
                + String( "<input type=\"submit\" value=\" 开始配网 \"></form></center></body></html>");
  **/
  String ret = String("<!DOCTYPE html> <html lang=\"zh-CN\"> <head> <meta charset=\"UTF-8\"><title>BGWiFiConfig配网</title>")
               + String("<style> .input { border: 2px solid orange; padding: 10px; border-radius: 50px 20px; } .toptext { text-shadow: 5px 5px 5px #4e1f1f; } .button { display: inline-block; padding: 15px 25px; font-size: 24px; cursor: pointer; text-align: center; text-decoration: none; outline: none; color: #fff; background-color: #4CAF50; border: none; border-radius: 15px; box-shadow: 0 9px #999; } .button:hover { background-color: #3e8e41 } .button:active { background-color: #3e8e41; box-shadow: 0 5px #666; transform: translateY(4px); } </style>")
               + String("</head><body><center><form action=\"result\" method=\"post\">")
               + String("<h2 class=\"toptext\">配网</h2>")
               + String("<h4>WiFi名称：<input class=\"input\" type=text name=ssid /></h4>")
               + String("<h4>WiFi密码：<input class=\"input\" type=text name=pwd /></h4>")
               + String("<p>&nbsp;&nbsp;>>预计需要" + time + "秒</p>")
               + String("<input class=\"button\" type=\"submit\" value=\" 开始配网\"></form></center></body></html>");
  WFconfigserver.send(200, "text/html", ret);
}
void BGWiFiConfig::WRresult() {
  String ssid = WFconfigserver.arg("ssid");
  String pwd = WFconfigserver.arg("pwd");
  String retStr = "tag=OFF,mode=1,ssid=" + ssid + ",pwd=" + pwd;
  String ret = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
               + String("<center>")
               + String("<h2>配网写入结果</h2>")
               + String("<h2>已配置WiFi名称：" + ssid + "</h2>")
               + String("<h2>已配置WiFi密码:" + pwd + "</h2>")
               + String("<p>确认无误后，请退出页面，并重启开发板</p>")
               + String("<input type=\"submit\" value=\"返回配网页面\" onclick=\"javascript:history.back();\"></center></body></html>");

  String ret2 = String("<html><head><meta charset=\"utf-8\"><title>BGWiFiConfig配网</title></head><body>")
                + String("<center>")
                + String("<h2>配网写入结果</h2>")
                + String("<h2>已配置WiFi名称：" + ssid + "</h2>")
                + String("<h2>已配置WiFi密码：" + pwd + "</h2>")
                + String("<p>已调用autoStart()函数，已自动重启开发板，请观察串口输出！！</p>")
                + String("<input type=\"submit\" value=\"返回配网页面\" onclick=\"javascript:history.back();\"></center></body></html>");
  if (ssid != "" && ssid != NULL) {
    if (!boolSpaceWiFiTag) {
      retStr.replace(" ", "");
    }
    if (FS_W(retStr)) {
      if (boolautostart) {
        WFconfigserver.send(200, "text/html", ret2);
        delay_rst();
        ESP.restart();
      } else {
        WFconfigserver.send(200, "text/html", ret);
      }
    }
  }
}

void BGWiFiConfig::WRapi() {
  String retStr = "";
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
    String UMSGNAME[12] = { "umsg1", "umsg2", "umsg3", "umsg4", "umsg5", "umsg6",
                            "umsg7", "umsg8", "umsg9", "umsg10", "umsg11", "umsg12" };
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
    retStr2.replace(" ", "");
    if (FS_W_UMSG(retStr2)) {
      mySerial(">>UMSG Write OK", true);
      mySerial(">>自定义信息已写入：", false);
      mySerial(String(UMSGnum), false);
      mySerial("条", true);
      runTAG = "自定义信息已写入：" + String(UMSGnum) + "条";
    }
  }

  if (!boolSpaceWiFiTag) {
    retStr.replace(" ", "");
  }
  if (FS_W(retStr)) {
    if (StrApiRet[0] != NULL && StrApiRet[0] != "NULL" && StrApiRet[0] != "") {
      if (StrApiRet[1] == "addWiFi") {
        if (boolautostart) {
          WFconfigserver.send(200, "text/plain", StrApiRet[0] + ">>[" + mode + "," + ssid + "," + pwd + "]<<");
          delay_rst();
          ESP.restart();
        } else {
          WFconfigserver.send(200, "text/plain", StrApiRet[0] + ">>[" + mode + "," + ssid + "," + pwd + "]<<");
        }
      } else {
        if (boolautostart) {
          WFconfigserver.send(200, "text/plain", StrApiRet[0]);
          delay_rst();
          ESP.restart();
        } else {
          WFconfigserver.send(200, "text/plain", StrApiRet[0]);
        }
      }
    } else {
      if (boolautostart) {
        WFconfigserver.send(200, "text/plain", "ok,mode=" + mode + ",The board has rebooted!");
        delay_rst();
        ESP.restart();
      } else {
        WFconfigserver.send(200, "text/plain", "ok,mode=" + mode + ",Please restart the board!");
      }
    }
  }
}


IPAddress BGWiFiConfig::StrToIP(String str) {
  IPAddress ipadd;
  ipadd.fromString(str);
  return ipadd;
}

bool BGWiFiConfig::FS_W(String str) {
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

String BGWiFiConfig::FS_R() {
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

bool BGWiFiConfig::FS_W_UMSG(String str) {
  SPIFFS.format();
  File dataFile = SPIFFS.open("/bgwificonfig/umsg.txt", "w");
  dataFile.print(str);
  dataFile.close();
  Serial.println();
  mySerial(">>", true);
  return true;
}

String BGWiFiConfig::FS_R_UMSG() {
  if (!SPIFFS.exists("/bgwificonfig/umsg.txt")) {
    return "NULL";
  }
  File dataFile = SPIFFS.open("/bgwificonfig/umsg.txt", "r");
  String retStr;
  for (int i = 0; i < dataFile.size(); i++) {
    retStr += (char)dataFile.read();
  }
  dataFile.close();
  return retStr;
}

void BGWiFiConfig::StrCL_UMSG(String str) {
  if (str != "NULL" && str != "" && str != NULL && UMSGnum > 0 && UMSGnum < 13) {
    str.trim();
    String umsgname[13] = {
      "umsg1=", "umsg2=", "umsg3=",
      "umsg4=", "umsg5=", "umsg6=",
      "umsg7=", "umsg8=", "umsg9=",
      "umsg10=", "umsg11=", "umsg12=", "=umsg="
    };
    int umsgkey[13] = { -1 };
    for (int i = 0; i < UMSGnum + 1; i++) {
      umsgkey[i] = str.indexOf(umsgname[i]);
    }
    for (int j = 0; j < UMSGnum - 1; j++) {
      StrsUMSG[j] = str.substring(umsgkey[j] + umsgname[j].length(), umsgkey[j + 1]);
    }
    StrsUMSG[UMSGnum - 1] = str.substring(umsgkey[UMSGnum - 1] + umsgname[UMSGnum - 1].length(), str.indexOf(umsgname[12]));
  }
}

String BGWiFiConfig::readUMSG(int i) {
  if (UMSGnum > 0 && i < 13 && i > 0 && TAG == "OFF") {
    StrsUMSG[i - 1].trim();
    if (StrsUMSG[i - 1] != NULL && StrsUMSG[i - 1] != "" && StrsUMSG[i - 1] != "NULL")
      return StrsUMSG[i - 1];
  }
  return "NULL";
}

void BGWiFiConfig::offConnectWiFi(bool tag) {
  booloffconnectwifi = tag;
}

String BGWiFiConfig::readWiFi(int i) {
  String str = FS_R();
  if (i == 0) {
    return str.substring(str.indexOf(",ssid=") + 6, str.indexOf(",pwd="));
  } else if (i == 1) {
    return str.substring(str.indexOf(",pwd=") + 5, str.indexOf(",ip="));
  } else if (i == 2) {
    if (WiFi.isConnected())
      return "wifiok";
    else
      return "wifibad";
  }
  return "NULL";
}

String& BGWiFiConfig::setApiRet(int i) {
  String& str = StrApiRet[i];
  return str;
}

bool BGWiFiConfig::OK(bool tag) {
  String str = "";
  str = FS_R();
  str = str.substring(str.indexOf(",ssid=") + 6, str.indexOf(",pwd="));
  if (str != "" && str != "NULL" && str != NULL) {
    if (tag) {
      if (WiFi.isConnected())
        return true;
      else
        return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
  return false;
}


void BGWiFiConfig::setOTAhtml(String html) {
  if (html.length() > 15) {
    OTAserverIndex = html;
  }
}

void BGWiFiConfig::setOTATimeOut(int sectime) {
  if (sectime > 1) {
    OTASECtimeTAG = true;
    OTASECtime = sectime;
  }
}

String BGWiFiConfig::getOTAIP() {
  if (onlyotaTAG) {
    if ((WiFi.softAPgetStationNum() > 0) && WiFi.isConnected())
      return "[{AP:" + WiFi.softAPIP().toString() + "},{STA:" + WiFi.localIP().toString() + "}]";
    else if (WiFi.isConnected())
      return WiFi.localIP().toString();
    else if (WiFi.softAPgetStationNum() > 0)
      return WiFi.softAPIP().toString();
  } else {
    if (OK(true))
      return WiFi.localIP().toString();
  }
  return "WiFi就绪后，才能获取";
}

void BGWiFiConfig::OTAbegin() {
  if (TAG == "OFF" || onlyotaTAG) {
    Serial.println();
    mySerial("欢迎使用BGWiFiConfig-OTA程序！！", true);
    int ii = 0;
    if (onlyotaTAG == true) {
      while ((WiFi.status() != WL_CONNECTED) && (WiFi.softAPgetStationNum() < 1)) {
        delay(1000);
        Serial.println("OTA等待WiFi就绪...");
        ii++;
        if ((ii > OTASECtime) && OTASECtimeTAG) {
          Serial.println("WiFi就绪超时~~");
          break;
        }
      }
    } else {
      while (!OK(true) && TAG == "OFF") {
        delay(1000);
        Serial.println("OTA等待WiFi就绪...");
        ii++;
        if ((ii > OTASECtime) && OTASECtimeTAG) {
          Serial.println("WiFi就绪超时~~");
          break;
        }
      }
    }
    runTAG = "OTA启动成功";
    if ((WiFi.waitForConnectResult() == WL_CONNECTED) || (WiFi.softAPgetStationNum() > 0)) {
      OTAserver.on("/", HTTP_GET, []() {
        OTAserver.sendHeader("Connection", "close");
        OTAserver.send(200, "text/html", OTAserverIndex);
      });
      OTAserver.on(
        "/otaupdate", HTTP_POST, []() {
          OTAserver.sendHeader("Connection", "close");
          OTAserver.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
          ESP.restart();
        },
        []() {
          HTTPUpload& upload = OTAserver.upload();
          if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("OTA上传文件名: %s\n", upload.filename.c_str());
            uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if (!Update.begin(maxSketchSpace)) {
            }
          } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            }
          } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) {
              OTAserver.send(200, "text/plain", "Upload successfully: " + String(upload.totalSize) + "byte\nRestart to run the new program...");
              Serial.printf("成功上传: %u字节\n正在重启运行上传的程序...\n", upload.totalSize);
            } else {
              OTAserver.send(200, "text/plain", "ota update failed, please try again");
              Serial.printf("OTA上传失败，请重试！！");
            }
          }
          yield();
        });
      OTAserver.begin();
      String Tmsg = "";
      mySerial("BGWiFiConfig-OTA启动成功~~~", true);
      if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        Tmsg = "请访问http://" + WiFi.localIP().toString() + "进行OTA升级";
        Serial.println(Tmsg);
      }
      if (WiFi.softAPgetStationNum() > 0) {
        Tmsg = "请访问http://" + WiFi.softAPIP().toString() + "进行OTA升级";
        Serial.println(Tmsg);
      }
    } else {
      mySerial("BGWiFiConfig-OTA启动失败~~~", true);
    }
  }
}

void BGWiFiConfig::OTALoop() {
  if (TAG == "OFF" || onlyotaTAG) {
    OTAserver.handleClient();
  }
}

void BGWiFiConfig::setOTAWiFiSTA(String ssid, String pwd) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pwd.c_str());
}

void BGWiFiConfig::clOTAPIP(String ip) {
  String ipstrs[4];
  ipstrs[0] = ip.substring(0, ip.indexOf("."));
  ipstrs[1] = ip.substring(ipstrs[0].length() + 1, ip.indexOf(".", ipstrs[0].length() + 1));
  ipstrs[2] = ip.substring(ipstrs[0].length() + ipstrs[1].length() + 2, ip.indexOf(".", ipstrs[0].length() + ipstrs[1].length() + 2));
  ipstrs[3] = ip.substring(ipstrs[0].length() + ipstrs[1].length() + ipstrs[2].length() + 3, ip.indexOf(".", ipstrs[0].length() + ipstrs[1].length() + ipstrs[2].length() + 3));
  String mip = ip;
  mip.replace(" ", "");
  String gateway = "192.168.33.33";
  gateway = ipstrs[0] + "." + ipstrs[1] + " ." + ipstrs[2] + ".1";
  gateway.replace(" ", "");
  String subnet = "255.255.255.0";
  ipstrs[0].replace(" ", "");
  if (ipstrs[0].toInt() >= 0 && ipstrs[0].toInt() <= 127)
    subnet = "255.0.0.0";
  else if (ipstrs[0].toInt() >= 128 && ipstrs[0].toInt() <= 191)
    subnet = "255.255.0.0";
  else
    subnet = "255.255.255.0";

  otapip[0] = StrToIP(mip);
  otapip[1] = StrToIP(gateway);
  otapip[2] = StrToIP(subnet);
}

void BGWiFiConfig::setOTAWiFiAP(String ssid, String pwd, String ip) {
  if (onlyotaTAG) {
    clOTAPIP(ip);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(otapip[0], otapip[1], otapip[2]);
    WiFi.softAP(ssid.c_str(), pwd.c_str());
  } else {
    Serial.println();
    Serial.println("[未定义onlyota,不能使用setOTAWiFiAP函数]");
  }
}

void BGWiFiConfig::setOTAWiFiAPSTA(String APssid, String APpwd, String STAssid, String STApwd, String ip) {
  if (onlyotaTAG) {
    clOTAPIP(ip);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(otapip[0], otapip[1], otapip[2]);
    WiFi.softAP(APssid.c_str(), APpwd.c_str());
    WiFi.begin(STAssid.c_str(), STApwd.c_str());
  } else {
    Serial.println();
    Serial.println("[未定义onlyota,不能使用setOTAWiFiAPSTA函数]");
  }
}

void BGWiFiConfig::setConFailReset(int xReConNum, int xReConTime, bool xIsReboot) {
  boolConFailResetTag = true;
  gReConNum = xReConNum;
  SECtime = xReConTime;
  boolConFailResetIsReboot = xIsReboot;
}

void BGWiFiConfig::STA_M3_FailReset() {
  int iReConNum = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (MODE == "2") {
      STA_M2(SSID, PWD, IP, GATEWAY, SUBNET, DNS);
    } else {
      STA_M1(SSID, PWD);
    }
    iReConNum++;
    if (WiFi.isConnected()) {
      break;
    } else if (iReConNum > gReConNum) {
      boolConFailResetTag = false;
      mySerial(">>重置配网信息中，请等待...", true);
      runTAG = "重连仍失败，重置配网信息中，请等待";
      clearWiFi();
      runTAG = "连接失败，未成功连接WiFi[" + SSID + "]，已重置配网";
      if (boolConFailResetIsReboot) {
        mySerial("WiFi连接失败，已重置配网并将重启，请重新配网<<<", true);
        ESP.restart();
      } else {
        mySerial("WiFi连接失败，已重置配网，请重启开发板并配网<<<", true);
      }
      break;
    }
    runTAG = "连接失败，正在第[ " + String(iReConNum + 1) + " ]次连接WiFi[" + SSID + "]";
    mySerial("连接失败，正在第[ " + String(iReConNum + 1) + " ]次连接WiFi[" + SSID + "]", true);
  }
}

void BGWiFiConfig::useSpaceWiFi(bool xSpaceWiFiTag) {
  boolSpaceWiFiTag = xSpaceWiFiTag;
}

IPAddress BGWiFiConfig::getPWIP() {
  IPAddress rtIP = StrToIP("192.168.22.22");
  switch (gICompatibleM) {
    case 0:
      rtIP = StrToIP("192.168.22.1");
      break;
    case 1:
      rtIP = StrToIP("192.168.22.22");
      break;
    case 2:
      rtIP = StrToIP("192.168.4.1");
      break;
    case 3:
      rtIP = WiFi.softAPIP();
      break;
  }
  return rtIP;
}

String BGWiFiConfig::getPWIPDH() {
  String rtIP = "192.168.22.22";
  switch (gICompatibleM) {
    case 0:
      rtIP = "192.168.22.1";
      break;
    case 1:
      rtIP = "192.168.22.22";
      break;
    case 2:
      rtIP = "192.168.4.1";
      break;
    case 3:
      rtIP = WiFi.softAPIP().toString();
      break;
  }
  return rtIP;
}

void BGWiFiConfig::setCompatibleModeTest(bool xCMTag) {
  if (xCMTag)
    gICompatibleM = 2;
  else
    gICompatibleM = 3;
}

void BGWiFiConfig::setCompatibleMode(bool xCMTag) {
  if (xCMTag)
    gICompatibleM = 0;
  else
    gICompatibleM = 1;
}