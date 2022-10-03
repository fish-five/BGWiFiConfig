/**
    BGWiFiConfig的eeshow模式简述
    开发板：esp8266和esp32全系列
    开发文档：https://www.cbug.top/1app/bgwificonfig
    Gitee地址：https://gitee.com/fish_five/BGWiFiConfig
    GitHub地址：https://github.com/fish-five/BGWiFiConfig
    微信公众号：蚍蜉荣光  官方网站：www.cbug.top
--------------------------------------------------------
  1.eeshow模式的开启和关闭？
    1.1.通过宏定义开启：
      #define eeshow
      #define eeshowM 0 
      #include <BGWiFiConfig.h>
      tag:程序内定义eeshow,再通过eeshowM指定弹起页面(0.导航页面,1.默认页面,2.自定义页面)
    1.2.通过BGWiFiConfig配置助手开启
      #define exe
      #include <BGWiFiConfig.h> 
      tag:程序内定义exe,再运行配置助手进行配置       
  2.BGWiFiConfig配置助手如何下载？
    答:https://www.cbug.top/article/25.html
  3.eeshow模式实现了什么需求？
    答：解决部分手机因自身网络优化造成配网WiFi自动断开的问题，以及实现了连接配网WiFi后自动跳转到配网页面的功能。
  4.开启eeshow模式后功能有什么变化？
    答：开启eeshow模式后，配网链接会发生变化，需要省略2022端口，具体请看串口信息或问题5。
  5.eeshow模式下的配网链接？
    配网页面：http://192.168.22.22/def
    配网接口：http://192.168.22.22/api
    自定义配网页面：http://192.168.22.22/html
    配网导航页面：http://192.168.22.22
----------------------------------------------------------  
**/
