#ifndef _BGWiFiConfig_H_
#define _BGWiFiConfig_H_
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

class BGWiFiConfig
{
  private:
    String TAG, MODE, SSID, PWD, IP, GATEWAY, SUBNET, DNS;
    String APssid, APpwd;
    static String mhtml, mhtmlresult, runTAG;
    int SECtime = 30;
    bool  booloutwifiset = false;
    static bool booloffSerial;
    static void mySerial(String str, bool nend);
    void debugPZ();
    void APstart();
    void StrCL(String str);
    void STA_M1(String Mname, String Mssid);
    void STA_M2(String Mname, String Mssid, String Mlocal_IP, String Mgateway, String Msubnet, String Mdns);
    static void WRhtml();
    static void WRhtmlresult();
    static void WRindex();
    static void WRresult();
    static void WRapi();
    IPAddress StrToIP(String str);
    static bool FS_W(String str);
    String FS_R();

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
    String retRUNTAG();
    String retWiFiSET();
};
#endif
