/*
// Y.A.K.E
// https://www.yge.me
*/


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <Servo.h>  

  const char* ssid     = "openwrt_546659";
  const char* password = "012345678900aabb";
  
  // web服务器端口  80
  WiFiServer server(80);
  Servo myservo; 
  
 
  String header;
  
  // 默认状态
  String output5State = "off";
  String output4State = "off";
  String output3State = "off";
  String output2State = "off";



  
  int pos1 = 0;    // 这两个舵机控制角度 关 0°
  int pos2 = 90;   //开 90°







  
  // 定义引脚
  const int output5 = D5;
  const int output4 = D4;
  const int output3 = D3;
  const int output2 = D2; 





  
  void setup() {
    Serial.begin(115200);
    // 初始化
    pinMode(output5, OUTPUT);
    pinMode(output4, OUTPUT);
    pinMode(output3, OUTPUT);
    
    //舵机为D2引脚
    myservo.attach(D2);

    
    // 默认灯是关闭状态
    digitalWrite(output3, LOW);
    digitalWrite(output4, LOW);
    digitalWrite(output5, LOW);
   
    // 在串口中输出WIFI连接状态
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }





#ifndef LWIP_IPV6

    //调试输出

      //输出IPv4 
      Serial.print("Connected! IPv4: ");
      Serial.println(WiFi.localIP());
    
  
#else

  //添加host
  //如果开启了IPV6那么这个功能会失效,这是一个官方尚未完善的功能
  //https://github.com/esp8266/Arduino/issues/6135#issuecomment-495361807
  


  
  for (bool configured = false; !configured;) {
    for (auto addr : addrList)
      if ((configured = !addr.isLocal()
          )) {
        break;
      }
    

    
      Serial.print('.');
           
    delay(500);
  }


  for (auto a : addrList) {

    //调试输出
 
      Serial.printf("IF='%s' IPv6=%d local=%d hostname='%s' addr= %s",
                 a.ifname().c_str(),
                 a.isV6(),
                 a.isLocal(),
                 a.ifhostname(),
                 a.toString().c_str());
        
    


    if (a.isLegacy()) {
      //调试输出
        Serial.printf(" / mask:%s / gw:%s",
                   a.netmask().toString().c_str(),
                   a.gw().toString().c_str());
                        
    }

  }

#endif    

    

    //  esp8266 为host名字
    //IPV6模式下是无效,这是官方不支持
    if (!MDNS.begin("esp8266")) {
      Serial.println("Error setting up MDNS responder!");
      while (1) {
        delay(1000);
      }
    }

    
    server.begin();
    MDNS.addService("http", "tcp", 80);
  }
  
  void loop() {
    MDNS.update();
  WiFiClient client = server.available();   
    if (client) {                             
      Serial.println("New Client.");          
      String currentLine = "";                
      while (client.connected()) {            
        if (client.available()) {             
          char c = client.read();             
          Serial.write(c);                    
          header += c;
          if (c == '\n') {
            if (currentLine.length () == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
  
              //URL是 /5/on 则 5打开
              //URL是 /5/off 则5关闭 
              //其他几个类似
              if (header.indexOf("GET /5/on") >= 0) {
                Serial.println("GPIO 5 on");
                output5State = "on";
                digitalWrite(output5, HIGH); //打开
              } else if (header.indexOf("GET /5/off") >= 0) {
                Serial.println("GPIO 5 off");
                output5State = "off";
                digitalWrite(output5, LOW);  //关闭
              }
              
              if (header.indexOf("GET /4/on") >= 0) {
                Serial.println("GPIO 4 on");
                output4State = "on";
                digitalWrite(output4, HIGH);
              } else if (header.indexOf("GET /4/off") >= 0) {
                Serial.println("GPIO 4 off");
                output4State = "off";
                digitalWrite(output4, LOW);
              }
                           
              if (header.indexOf("GET /3/on") >= 0) {
                Serial.println("GPIO 3 on");
                output3State = "on";
                digitalWrite(output3, HIGH);
              } else if (header.indexOf("GET /3/off") >= 0) {
                Serial.println("GPIO 3 off");
                output3State = "off";
                digitalWrite(output3, LOW);
              }


              //舵机控制
              
              if (header.indexOf("GET /2/on") >= 0) {
                Serial.println("GPIO 2 on");
                output2State = "on";
                myservo.write(pos1);  //舵机转到指定角度(pos1) 
              } else if (header.indexOf("GET /2/off") >= 0) {
                Serial.println("GPIO 2 off");
                output2State = "off";
                myservo.write(pos2);  //舵机转到指定角度(pos2) 
              }        
              

              //输出网页内容
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta charset=\"UTF-8\"/>");
              client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #77878A;}</style></head>");
  

              client.println("<body><h1>Y.A.K.E 8266网页开关测试</h1>");
              client.println("<br /><p> <div> 8266接线 D2 接舵机, D3-D5接LED或其他开关设备</div></p><br />");

              client.println("<p>LED 5  - 状态 " + output5State + "</p>");

              if (output5State == "off") {
                client.println("<p><a href=\"/5/on\"><button class=\"button\">开</button></a></p>");
              } else {
                client.println("<p><a href=\"/5/off\"><button class=\"button button2\">关</button></a></p>");
              }




              client.println("<p>LED 4 - 状态 " + output4State + "</p>");
              if (output4State == "off") {
                client.println("<p><a href=\"/4/on\"><button class=\"button\">开</button></a></p>");
              } else {
                client.println("<p><a href=\"/4/off\"><button class=\"button button2\">关</button></a></p>");
              }
              

              client.println("<p>LED 3 - 状态 " + output3State + "</p>");
              if (output3State == "off") {
                client.println("<p><a href=\"/3/on\"><button class=\"button\">开</button></a></p>");
              } else {
                client.println("<p><a href=\"/3/off\"><button class=\"button button2\">关</button></a></p>");
              }




              
              client.println("<p>舵机 - 状态 " + output2State + "</p>");
              if (output2State == "off") {
                client.println("<p><a href=\"/2/on\"><button class=\"button\">开</button></a></p>");
              } else {
                client.println("<p><a href=\"/2/off\"><button class=\"button button2\">关</button></a></p>");
              }
  
  
              client.println("</body></html>");
              client.println();
              break;
            } else { 
              currentLine = "";
            }
          } else if (c != '\r') {  
              currentLine += c; 
          }
        }
      }

      header = "";

      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
