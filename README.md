# BGWiFiConfig
一个为esp8266和esp32设计的简单易用Arduino配网库，可通过api配置WiFi网络，支持GET和POST请求配置，可通过默认网页配置WiFi网络，可自定义配网页面。


## [查看开发文档【必看】](https://www.cbug.top/1app/bgwificonfig/#/)

## 配网api
### 1.由DHCP分配ip信息：
- mode：1
- ssid：你要配置的WiFi名
- pwd：你要配置的WiFi密码
### 2.静态ip：
- mode：2
- ssid：你要配置的WiFi名
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

## 通过网页配网
```
连接开发板生成的WiFi(用户名:WiFi配网，密码为空），浏览器进入http://192.168.22.22:2022
```

## 自定义网页配网
```
见开发文档和参考示例代码
```
