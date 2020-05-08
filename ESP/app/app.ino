#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

//12345678&msg=on246810
SoftwareSerial swSer(D3, D5, false, 256);   //rx  tx
#define TCP_SERVER_ADDR "bemfa.com"
#define TCP_SERVER_PORT "8344"

//********************需要修改的部分*******************//

//WIFI名称，区分大小写，不要写错
#define DEFAULT_STASSID  "lzy"
//WIFI密码
#define DEFAULT_STAPSW   "123456789"
//用户私钥，可在控制台获取,修改为自己的UID
#define UID  "4d115fac4d99bf71605d29c65ee5b25a"
//主题名字，可在控制台新建
#define TOPIC  "BYSJ"
//单片机LED引脚值
const int LED_Pin = D7;

//**************************************************//



//最大字节数
#define MAX_PACKETSIZE 512
//设置心跳值30s
#define KEEPALIVEATIME 30*1000



//tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";
String UID1 = "4d115fac4d99bf71605d29c65ee5b25a";
String TOPIC1 = "BYSJ";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long uart_preTick = 0;
unsigned long preHeartTick = 0;//心跳
unsigned long preTCPStartTick = 0;//连接
bool preTCPConnected = false;
byte count = 0;
int i = 0;
String count_flag = "";
String count_flag1 = "";
String count_flag2 = "";
String count_flag3 = "";
String comdata = "";
String comdata1 = "";
unsigned int uart_BuffIndex = 0;
//相关函数初始化
//连接WIFI
void doWiFiTick();
void startSTA();

//TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

//led 控制函数
void turnOnLed();
void turnOffLed();



/*
  *发送数据到TCP服务器
 */
void sendtoTCPServer(String p){
  
  if (!TCPclient.connected()) 
  {
    Serial.println("Client is not readly");
    return;
  }
  TCPclient.print(p);
  Serial.println("[Send to TCPServer]:String");
  Serial.println(p);
}


/*
  *初始化和服务器建立连接
*/
void startTCPClient(){
  if(TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT))){
    Serial.print("\nConnected to server:");
    Serial.printf("%s:%d\r\n",TCP_SERVER_ADDR,atoi(TCP_SERVER_PORT));
    char tcpTemp[128];
    sprintf(tcpTemp,"cmd=1&uid=%s&topic=%s\r\n",UID,TOPIC);

    sendtoTCPServer(tcpTemp);
    preTCPConnected = true;
    preHeartTick = millis();
    TCPclient.setNoDelay(true);
  }
  else{
    Serial.print("Failed connected to server:");
    Serial.println(TCP_SERVER_ADDR);
    TCPclient.stop();
    preTCPConnected = false;
  }
  preTCPStartTick = millis();
}


/*
  *检查数据，发送心跳
*/
void doTCPClientTick(){
 //检查是否断开，断开后重连
   if(WiFi.status() != WL_CONNECTED) return;

  if (!TCPclient.connected()) {//断开重连

  if(preTCPConnected == true){

    preTCPConnected = false;
    preTCPStartTick = millis();
    Serial.println();
    Serial.println("TCP Client disconnected.");
    TCPclient.stop();
  }
  else if(millis() - preTCPStartTick > 1*1000)//重新连接
    startTCPClient();
  }
  else
  {
    if (TCPclient.available()) {//收数据
      char c =TCPclient.read();
      TcpClient_Buff +=c;
      TcpClient_BuffIndex++;
      TcpClient_preTick = millis();
      
      if(TcpClient_BuffIndex>=27 - 1){
        TcpClient_BuffIndex = 27-2;
        TcpClient_preTick = TcpClient_preTick - 200;
      }
      preHeartTick = millis();
    }
    if(millis() - preHeartTick >= KEEPALIVEATIME){//保持心跳
      preHeartTick = millis();
      Serial.println("--Keep alive:");
      sendtoTCPServer("cmd=0&msg=keep\r\n");
    }
  }
  if((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick>=200))
  {//data ready
    TCPclient.flush();
    Serial.println("Buff");
    Serial.println(TcpClient_Buff);
    if((TcpClient_Buff.indexOf("&msg=on") > 0)) {
      turnOnLed();
    }else if((TcpClient_Buff.indexOf("&msg=off") > 0)) {
      turnOffLed();
    }
   TcpClient_Buff="";
   TcpClient_BuffIndex = 0;
  }
}

void startSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);

}



/**************************************************************************
                                 WIFI
***************************************************************************/
/*
  WiFiTick
  检查是否需要初始化WiFi
  检查WiFi是否连接上，若连接成功启动TCP Client
  控制指示灯
*/
void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    startSTA();
    Serial.printf("Heap size:%d\r\n", ESP.getFreeHeap());
  }

  //未连接1s重连
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  //连接成功建立
  else {
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}
//打开灯泡
void turnOnLed(){
  Serial.println("Turn ON");
  digitalWrite(LED_Pin,LOW);
}
//关闭灯泡
void turnOffLed(){
  Serial.println("Turn OFF");
    digitalWrite(LED_Pin,HIGH);
}


void uart_re(){
  if(swSer.available()>0)
  {
    comdata += char(swSer.read());  
    delay(2);
    uart_BuffIndex++;
    uart_preTick = millis();
    if(uart_BuffIndex >= 511) {
      uart_BuffIndex = 510;
      uart_preTick = uart_preTick - 200;
    }
  }
  if((comdata.length() >= 1) && (millis() - uart_preTick>=200))
  {
      swSer.flush();
      Serial.println(comdata);
      if((comdata.indexOf("&msg=on") > 0)) {
        
        count++;
        String upstr = "";
        count_flag = comdata.substring(15 , 25);
        count_flag1 = count_flag.substring(0 , 3);
        count_flag2 = count_flag.substring(3 , 6);
        count_flag3 = count_flag.substring(6 , 9);
        upstr = "cmd=2&uid="+UID1+"&topic="+TOPIC1+"&msg=#"+count_flag1+"#"+count_flag2+"#"+count_flag3+"#\r\n";
        sendtoTCPServer(upstr);
        count_flag = "";
        count_flag1 = "";
        count_flag2 = "";
        count_flag3 = "";
        upstr = "";
      }
      comdata="";
      uart_BuffIndex = 0;
  }

}

// 初始化，相当于main 函数
void setup() {
  Serial.begin(115200);
  pinMode(LED_Pin,OUTPUT);
  digitalWrite(LED_Pin,HIGH);
  swSer.begin(115200);
  Serial.println("\nSoftware serial test started 2222");

}



//循环
void loop() {
  doWiFiTick();
  doTCPClientTick();
  uart_re();
}
