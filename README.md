# BGWiFiConfig
一个为esp8266和esp32设计的简单易用Arduino配网库，可通过api配置WiFi网络，支持GET和POST请求配置；可通过微信小程序配网(小程序名：配网库)；可通过默认网页配置WiFi网络；可自定义配网页面。


## [查看开发文档【必看】](https://www.cbug.top/1app/bgwificonfig/#/)
### [BGWiFiConfig配置助手【开启和关闭eeshow模式】](https://www.cbug.top/article/25.html)

## 配网api
### 1.由DHCP分配ip信息：
- mode：1
- ssid：你要配置的WiFi名称
- pwd：你要配置的WiFi密码
### 2.静态ip：
- mode：2
- ssid：你要配置的WiFi名称
- pwd：你要配置的WiFi密码
- ip：你要配置的WiFi的ip
- gateway：你要配置的WiFi的网关
- subnet：你要配置的WiFi的子网掩码
- dns：你要配置的WiFi的dns

## 通过api配网：
```
连接开发板生成的WiFi(用户名:WiFi配网，密码为空）
```
### 1. GET请求配网
```
GET http://192.168.22.22:2022/api?mode=1&ssid=cam2&pwd=12345678
```
### 2. POST请求配网
```
POST http://192.168.22.22:2022/api
Content-Type: application/x-www-form-urlencoded

mode=2&ssid=cam2&pwd=12345678&ip=192.168.168.69&gateway=192.168.168.219&subnet=255.255.255.0&dns=192.168.168.219
```
## 通过微信小程序配网
微信小程序搜索【配网库】

![配网库](https://www.cbug.top/1img/pwkxcx.jpg)


## 通过网页配网
```
连接开发板生成的WiFi(用户名:WiFi配网，密码为空），浏览器进入http://192.168.22.22:2022
```

## 自定义网页配网
```
见开发文档和参考示例代码
```
## 自定义配网数据组
```
UMSG机制共可存取12组自定义数据，在api配网或自定义网页配网下使用，配网存储关键字[umsg1 ~ umsg12], 读取存储值[wifipw.readUMSG(int i)]
```
## eeshow模式
```
解决部分手机因自身网络优化造成配网WiFi自动断开的问题，以及实现了连接配网WiFi后自动跳转到配网页面的功能，需使用BGWiFiConfig配置助手进行配置。
```
## 友情提示
```
小伙伴们在使用BGWiFiConfig库时，一定要多看文档，为什么呢？因为BGWiFiConfig库很多新功能是根据小伙伴们的需求开发的，只有通过看文档，了解新的功能函数，你才知道哪些需求被采纳并实现了，从而才能更好地实现自己的需求。举例来说，自定义数据储存已经实现了一个多月了？eeshow怎么使用？自定义api返回值用哪个函数？这些你知道吗？？
```
## 补充约定
```
你可以通过BGWiFiConfig库提供的函数（参考开发文档和demo程序）任意使用本库（允许商用），但禁止对除demo程序（.ino结尾的文件）外的其他源代码及文件，进行任何形式的修改及二次开发。
``` 