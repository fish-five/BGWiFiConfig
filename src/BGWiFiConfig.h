#ifndef _BGWiFiConfig_H_
#define _BGWiFiConfig_H_
#include <Arduino.h>
#include "DEFPORT.h"
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#endif

class BGWiFiConfig
{
  private:
    String TAG, MODE, SSID, PWD, IP, GATEWAY, SUBNET, DNS;
    String APssid, APpwd;
    static String StrsUMSG[13];
    static String StrApiRet[2];
    static String mhtml, mhtmlresult, runTAG;
    int SECtime = 30;
    static int UMSGnum;
    bool  booloutwifiset = false;
    bool  booloffconnectwifi = false;
    static bool boolautostart;
    static bool booloffSerial;
    static void mySerial(String str, bool nend);
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
};
#endif
